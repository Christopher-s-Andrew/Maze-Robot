#include "commands.h"

//c includes
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


//helpful constants
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>

//drivers
#include "driverlib/adc.h"
#include <driverlib/pin_map.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/pwm.h>

//command fuction array
void (*command_funcs[NUM_OF_COMMANDS + 1])() = {
                         GreenLEDToggle,	// 0 - Green LED Toggle
						 BlueLEDToggle,	// 1 - Blue LED Toggle
						 RedLEDToggle,	// 2 - Red LED Toggle
						 ReadFrontADCValue,
						 ReadSideADCValue,
						 motorRStart,
						 motorLStart,
						 motorRStop,
						 motorLStop,
						 motorRF,
						 motorRS,
						 motorLF,
						 motorLF,
						 PIDOff,
                         PIDOn,
						 collectDataEnable,
						 collectDataDisable,
						 InvalidCommand // numOfCommands+1
                     };

//command name lookup array
char command_lookup[NUM_OF_COMMANDS][2] = {
                         "GT",	// 0 - Green Toggle
                         "BT",	// 1 - Blue Toggle
                         "RT",	// 2 - Red Toggle
						 "FR",	// 3 - Front ADC Read
						 "SR",	// 3 - Side ADC Read
						 "RO", //right on
						 "LO", //left on
						 "RH", //right off
						 "LH", //left off
						 "RF", //right faster
						 "RS", //right slower
						 "LF", //left faster
						 "LS", // left slower
						 "OF", // wall disable
						 "ON", // wall enabled
						 "CE", //data collection enable
						 "CD" //data collection disable
                     };


/*******  Command Table and Lookup Functions *********/
int findCommandNum(char str[2]){
	int i;
	for(i=0; i<NUM_OF_COMMANDS; i++){
		if( str[0]==command_lookup[i][0] && str[1]==command_lookup[i][1])
			break;
	}

	return i;
}

/******** User Command Functions ***********/

//toggle green led
void GreenLEDToggle(){
	uint8_t pinstatus = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	pinstatus ^= 8;
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, pinstatus);
}

//toggle blue led
void BlueLEDToggle(){
	uint8_t pinstatus = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	pinstatus ^= 4;
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, pinstatus);
}

//toggle red led
void RedLEDToggle(){
	uint8_t pinstatus = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	pinstatus ^= 2;
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, pinstatus);
}

//read front distance sensor in cm
void ReadFrontADCValue(){
    char output[12];
    uint32_t adcReadVal = ADCvalues[0];

    float distanceValue = (0.000004) * adcReadVal * adcReadVal - 0.0241 * adcReadVal + 38.815;
    sprintf(output, "%f", distanceValue);

    int i;
	for( i=0; output[i] != 0 ; i++)
		UARTCharPut(UART5_BASE, output[i]);
	UARTCharPut(UART5_BASE, '\r');
	UARTCharPut(UART5_BASE, '\n');
}

//read side distance sensor in adc units
void ReadSideADCValue(){
    char output[12];
    uint32_t adcReadVal = ADCvalues[1];
    sprintf(output, "%d", adcReadVal);

    int i;
	for( i=0; output[i] != 0 ; i++)
		UARTCharPut(UART5_BASE, output[i]);
	UARTCharPut(UART5_BASE, '\r');
	UARTCharPut(UART5_BASE, '\n');
}

//start right motor
void motorRStart()
{
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1000); //PB6

}

//start left motor
void motorLStart()
{
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 1000); // PB7
}

//stops the right motor
void motorRStop()
{
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1); //PB6
}

//stops the left motor
void motorLStop()
{
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 1); //PB7
}

//makes the right motor faster
void motorRF()
{
	if(PWMPulseWidthGet(PWM0_BASE, PWM_OUT_0) <= 900)
	{
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMPulseWidthGet(PWM0_BASE, PWM_OUT_0)+100);
	}
	else
	{
		char respStr[] = "MaxSpeedRightReached\r\n";
			int i;
			for( i=0; respStr[i] != 0 ; i++)
				UARTCharPut(UART5_BASE, respStr[i]);
	}
}

//makes the right motor slower
void motorRS()
{
	if(PWMPulseWidthGet(PWM0_BASE, PWM_OUT_0) >= 100)
	{
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMPulseWidthGet(PWM0_BASE, PWM_OUT_0)-100);
	}
	else
	{
		char respStr[] = "MinSpeedRightReached\r\n";
			int i;
			for( i=0; respStr[i] != 0 ; i++)
				UARTCharPut(UART5_BASE, respStr[i]);
	}
}

//makes the left motor faster
void motorLF()
{
	if(PWMPulseWidthGet(PWM0_BASE, PWM_OUT_1) <= 900)
		{
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMPulseWidthGet(PWM0_BASE, PWM_OUT_1)+100);
		}
		else
		{
			char respStr[] = "MaxSpeedLeftReached\r\n";
				int i;
				for( i=0; respStr[i] != 0 ; i++)
					UARTCharPut(UART5_BASE, respStr[i]);
		}
}

//makes the left motor slower
void motorLS()
{
	if(PWMPulseWidthGet(PWM0_BASE, PWM_OUT_1) >= 100)
	{
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMPulseWidthGet(PWM0_BASE, PWM_OUT_1)-100);
	}
	else
	{
		char respStr[] = "MinSpeedRightReached\r\n";
			int i;
			for( i=0; respStr[i] != 0 ; i++)
				UARTCharPut(UART5_BASE, respStr[i]);
	}
}

//turns on the robot
void PIDOn()
{
	On_Flag = 1;
}

//turns off the robot
void PIDOff()
{
	On_Flag = 0;
}

//wrong command responce
void InvalidCommand(){
	char respStr[] = "Invalid Command\r\n";
	int i;
	for( i=0; respStr[i] != 0 ; i++)
		UARTCharPut(UART5_BASE, respStr[i]);
}

//turn on data collection and transmion
void collectDataEnable()
{
	thinLineFound = 1;
}

//turn off data collection and transmion
void collectDataDisable()
{
	thinLineFound = 0;
}
