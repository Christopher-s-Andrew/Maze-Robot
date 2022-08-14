/*
 * robot_hwInit.h
 *
 *  Created on: Oct 13, 2021
 *      Author: Patrick Kelling, Christopher Andrew
 */

#ifndef ROBOT_HWINIT_H_
#define ROBOT_HWINIT_H_

/* Standard libs */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Constants */
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>
#include <inc/hw_pwm.h>

/* XDC */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Log.h>                //needed for any Log_info() call
#include <xdc/cfg/global.h>

/* DriverLib */
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "driverlib/pwm.h"

/* TI-RTOS BIOS  */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Peripherals */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>




void Board_Init() {
	/* Initializes HW for Robot
	 * Pins:
	 * 	LEDs - PF1,PF2,PF3
	 * 	UART - tx:PE5, rx: PE4
	 * 	ADCs - PB4, PB5
	 * 	PWMs - PB6,PB7
	 * 	GPIO left motor - PF4
	 **/

    // ****** GPIO LEDs Init ******
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, 0);

    // ****** UART Init ****** //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART5)){}

    // Configure UART pins
    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // Set UART Config (baud: 115200, 8 bits, no parity, 1 stop bit)
    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), 115200,
    		(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // Enable Interrupt (TX doesn't matter, RX interrupt after 2 chars)
    UARTFIFOLevelSet(UART5_BASE, UART_FIFO_TX7_8, UART_FIFO_RX1_8);
    UARTIntEnable(UART5_BASE, UART_INT_RX);


    // ******* ADC Init ************//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)){}

	// ADC PB4 (AIN10)
	GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_4);

	// ADC PB5 (AIN11)
	GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_5);

	//Over-sampling factor of 2 (options: 0,2,4,8,16,32,64) *DITHER bit?
	ADCHardwareOversampleConfigure(ADC0_BASE, 64); // Tested and it helps

	//Configure Sequencer- SeqNum 0, Timer Trigger, Priority 0 (unique)
	ADCSequenceConfigure(ADC0_BASE,0,ADC_TRIGGER_TIMER,0);

	// Set Sequence (up to 7) - PB5 -> PB4 -> more??
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH11); // Get Ch11 (PB5) first
	ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH10|ADC_CTL_IE|ADC_CTL_END); // Get Ch10 (PB4) - trigger interrupt
	ADCSequenceEnable(ADC0_BASE, 0);

	// Enable Interrupt
	ADCIntEnable(ADC0_BASE, 0);


	// ******** Setup ADC Timer Trigger ***********//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_WTIMER0)){}
	TimerConfigure(WTIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC );
	TimerLoadSet(WTIMER0_BASE, TIMER_A, 4000000);
	TimerControlTrigger(WTIMER0_BASE, TIMER_A, 1);
	TimerEnable(WTIMER0_BASE, TIMER_A);


	// ******** Setup PWM ***********//
	// Setup PB6 for M0PWM0, and PB7 for M0PWM1
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);	// Configures PB6 for typical PWM settings
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
	GPIOPinConfigure(GPIO_PB6_M0PWM0);	// Set PB6 Alt function - find options in includes-> Tivaware C Series -> driverlib -> pin_map.h (under TM4C123GH...)
	GPIOPinConfigure(GPIO_PB7_M0PWM1);


	// Setup PWM0 with 0% duty cycle
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0); // Enable PWM Module 0
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0)){}

	// Setup Generator 0
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 1000); 	// Set the period 1000 ticks
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);				// starts timers in gen0
	PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);	// enables the outputs
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1); 		// Set duty cycle to 0/ticks_in_period ~= 0%

	// Setup Generator 1
	PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 1000); 	// Set the period 1000 ticks
	PWMGenEnable(PWM0_BASE, PWM_GEN_1);				// starts timers in gen0
	PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);	// enables the outputs
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 1); 		// Set duty cycle to 0/ticks_in_period ~= 0%


	// *********** Audio PWM ******************* //
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}

	// Setup PB6 for M0PWM0, and PB7 for M0PWM1
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);	// Configures PD0 for typical PWM settings
	GPIOPinConfigure(GPIO_PD0_M1PWM0);	// Set PD0 Alt function

	// Setup PWM Module 1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1); // Enable PWM Module 1
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1)){}

	// Setup PWM1 Generator 0
	PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 10000); 	// Set the period
	PWMGenEnable(PWM1_BASE, PWM_GEN_0);				// starts timers in gen0
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);	// enables the outputs
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 5000); 		// Set duty cycle to 0/ticks_in_period ~= 0%

}


#endif /* ROBOT_HWINIT_H_ */
