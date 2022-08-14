/*
 *  ======= main ========
 *
 *  Created on: Sep 1, 2021
 *  Author:    Patrick Kelling, Christopher Andrew
 *
 *  Pins:
 *  - PF1, PF2, PF3 : Onboard LEDs
 *  - PE4, PE5 : Bluesmirf Tx/Rx (UART)
 *  - PB4, PB5 : ADC Inputs
 *  - PB6, PB7 : PWM Outputs
 */

/* Standard libs */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Constants */
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>

/* XDC */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Log.h>                //needed for any Log_info() call
#include <xdc/runtime/Timestamp.h>
#include <xdc/cfg/global.h>

/* DriverLib */
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"

/* TI-RTOS BIOS  */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Peripherals */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/* Our Files */
#include "commands.h"
#include "robot_hwinit.h"
#include "Songs.h"

//Function Declarations
void delay(uint32_t i);
void GreenLEDOn();
void GreenLEDOff();

// ======== main ========
Int main(void)
{
	//GreenLEDOn();
    Board_Init();
    //PID Default state
    On_Flag = 0;
    thinLineFound = 0; // thin black line flag
    BIOS_start();
    return (0);
}


/*******  Interrupt Handlers ********/
char userCommand[2];
/*
 * recives command from the user, saves them in 2 character arrays, then fires the uart_swi to run the command
 */
void UART_handleInterrupt(){
	// Interrupt occurs when 2 chars are put into FIFO

	UARTIntClear(UART5_BASE, UART_INT_RX);
	if( UARTCharsAvail(UART5_BASE)){

		// Get Data and Respond
		userCommand[0] = UARTCharGetNonBlocking(UART5_BASE);
		userCommand[1] = UARTCharGetNonBlocking(UART5_BASE);

		Swi_post(uart_swi);
	}
}

/*
 * adc interrupt is triggered when adc finishes front and side distance sensor reads
 * adc is fired every 50 ms via wide timer 0 time out
 */
void ADC_handleInterrupt(){
	ADCIntClear(ADC0_BASE, 0); // clear interrupt
	TimerIntClear(WTIMER0_BASE, TIMER_TIMA_TIMEOUT); // clear timer interrupt
	ADCSequenceDataGet(ADC0_BASE, 0, ADCvalues);

	Swi_post(adc_swi);
}



/********* SWI Functions **********************/
/*
 * takes the the 2 character command, outputs that a command was recived, then searches and runs the relavent command
 */
void uart_swiHandler(void){

	// Print Response via BlueTooth
	char respStr[] = " Command Received\r\n";
	UARTCharPut(UART5_BASE, userCommand[0]);
	UARTCharPut(UART5_BASE, userCommand[1]);
	int i;
	for( i=0; respStr[i] != 0 ; i++)
		UARTCharPut(UART5_BASE, respStr[i]);


	// Call Function for command
	(*command_funcs[ findCommandNum(userCommand) ])();

}

int totalSummation = 0;
int wall_errorPrev = 0;
int turningFlag = 0;
int Wall_PID_On_Flag = 1;

int PingPongSwitch = 0;
int PingPongIncr = 0;
int PingBuffer[20];
int PongBuffer[20];
int everyOtherPID = 1;

int mazeTimer = 0;
/**
 * main swi, runs the PID to control robot movement
 * runs data sampling code when required
 * runs turn around when turnaround is triggered
 */
void adc_swiHandler(void){
	uint32_t pb5Val = ADCvalues[0]; // Front
	uint32_t pb4Val = ADCvalues[1]; // Right (wall sensor)

	uint16_t Speed = 1000; // can later be set with front PID?

	//only run when on command is sent
	if(On_Flag){
		mazeTimer++;
		//		Running PID		Not close to front or no wall on right
		if( Wall_PID_On_Flag && (pb5Val < 3000 || pb4Val < 1400) ){

			//******* Wall Distance PID **********//

			// Settings
			int setpoint_wall = 2000; // ~6cm

			//these work float P_wall = 0.4, I_wall = 0.001, D_wall = 0.005;
			float P_wall = 0.5, I_wall = 0, D_wall = 0.005;

			int wall_error = setpoint_wall - pb4Val; // Varies by about -500 -> 500 (in track)
			totalSummation += wall_error;
			int diff = wall_error - wall_errorPrev;
			wall_errorPrev = wall_error;

			// PID for motor diff: 	P				 +			I			   +	D;
			int motorDiff = (int)(P_wall * wall_error + I_wall * totalSummation + D_wall * diff);

			if( motorDiff > 0){
				// too far left
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, Speed - abs(motorDiff)); // right
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, Speed); // left
			}
			else {
				// too far right (lower left motor)
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, Speed); // right
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, Speed - abs(motorDiff)); // left
			}

			// Grab error data and save
			if( thinLineFound & everyOtherPID){
				// Fill Correct Buffer
				if(PingPongSwitch == 0)
					PingBuffer[PingPongIncr] = pb4Val;
				else
					PongBuffer[PingPongIncr] = pb4Val;

				PingPongIncr++;

				// If Full, switch to other buffer
				if(PingPongIncr == 20){
					PingPongSwitch ^= 1; // switch to other buffer *before posting semaphore
					PingPongIncr = 0;
					Semaphore_post(BufferAvailableSema);
				}
				everyOtherPID ^= 1;
			}
			else if(thinLineFound & !everyOtherPID)
			{
				everyOtherPID ^= 1;
			}

		}
		else if(!turningFlag && pb5Val >= 3000){
			// Robot at dead end, execute turn around
			turningFlag = 1;
			Wall_PID_On_Flag = 0; //PID off
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 1); // left motor reverse (PWM off, GPIO pin on)
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0x10); // Turn on PF4
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1000); // right motor on (forward)
		}
		else if(!turningFlag){
			// PID off and not turning
			Wall_PID_On_Flag = 0; // remove later?
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1); // motor off
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 1); // motor off
		}


		if(turningFlag){
			if(pb5Val < 1000){
				//turn finished (front wall is far enough away)
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0); // Turn off PF4
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1); // motor off
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 1); // motor off
				turningFlag = 0;
				Wall_PID_On_Flag = 1;
			}
		}
	}else{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0); // Turn off PF4
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1); // motor off
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 1); // motor off
	}
}


// ======== Clock Functions  ========
int blackThreshold = 4; //how thick is a thick line
int blackCount = 0; //how many times a black line was found
void scanForBlackLine(void){

	   uint32_t startTime, endTime, pinValue, decayTime;

	   // Charge Capacitor
	   GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);         // Make PortD pin1 output to charge the sensor
	   GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);      // Start charing the sensor
	   delay(1);                                         // Wait to finish charging

	   // Read Delay Time
	   startTime = Timestamp_get32();            // Capture startTime
	   GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);          // Make PortD pin1 input to time the decaying
	   pinValue = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1);        // Assign the value when sensor is in fully charged state
	   while (pinValue & GPIO_PIN_1) {                             // Loop to compare to the current state to fully-charged state
	       pinValue = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1);
	       delay(1); //this needs a delay otherwise rtos dies? if GPIOPinRead is constantly called
	   }
	   endTime = Timestamp_get32();              // Capture endTime when fully decayed
	   decayTime = (endTime - startTime) / 80;

	   if( decayTime > 3500){                           // Above black surface if discharge time > 7000 microseconds

		   //GreenLEDOn();
		   blackCount++;
		   //count number of black found
	   }
	   else{
		  // GreenLEDOff();

		   if(blackCount >= blackThreshold)
		   {
			   //thick black line found
			   PIDOff();
			   //data output turn off
			   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
			   thinLineFound = 0;
			   //Semaphore_post(BufferAvailableSema);

				UARTCharPut(UART5_BASE, '\r');
				UARTCharPut(UART5_BASE, '\n');

			   char outputChar[15];
			   sprintf(outputChar, "%d", (mazeTimer / 20));
				//output
			   int i;
			   for(i=0; i<5; i++)
				   UARTCharPut(UART5_BASE, outputChar[i]);

				UARTCharPut(UART5_BASE, 's');
				UARTCharPut(UART5_BASE, '\r');
				UARTCharPut(UART5_BASE, '\n');

		   }
		   else if(blackCount >= 1 && blackCount < blackThreshold)
		   {
			   //thin black line found
			   	thinLineFound ^= 1; // switch line found state
			   	BlueLEDToggle();

			   	if(thinLineFound == 0){
			   		// send data of unfilled buffer
			   		PingPongSwitch ^= 1;
					Semaphore_post(BufferAvailableSema);
			   	}
		   }


		   blackCount = 0; //reset blackCount if white is found
		   //count number of white found

		   //reset counters if too many white are found
	   }

}


int audoNoTimeOut = 0;
// ======== tasks  ========
/**
 * bonus feature, plays sonic green hill theme while the robot is running the maze
 */
void audioTask_func(UArg arg0, UArg arg1) {
	int AudioFlag = 1;
	int currSong = 0;
	int SongPos = 0;
	int numOf16ths = 0;
	int SongSize = SongSizes[currSong];
	int* melody = songs[currSong];

    while (true) {
    	// changes every 16th note at ~110bmp metronome

    	if(On_Flag){
    		if( SongPos < SongSize && numOf16ths == 0){
    			// New note, turn of PWM before changing freq
    			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 1);

    			Task_sleep(8); // 5ms pause between notes

    			uint32_t NoteFreq = melody[SongPos];
    			int playTime = melody[SongPos + 1];
    			switch(playTime){
					case 1: numOf16ths = 16; break;
    				case 2: numOf16ths = 8; break;
    				case 4: numOf16ths = 4; break;
    				case 8: numOf16ths = 2; break;
    				case 16: numOf16ths = 1; break;
    				case -1: numOf16ths = 24; break;
    				case -2: numOf16ths = 12; break;
    				case -4: numOf16ths = 5; break;
    				case -8: numOf16ths = 3; break;
    				default: numOf16ths = 0;
    			}

    			int ticksPeriod;
    			int halfPeriod;
    			if( NoteFreq == 0){
    				ticksPeriod = 1000; // doesn't matter what value this is
    				halfPeriod = 1; 	// basically 0% duty cycle
    			}
    			else{
    				ticksPeriod = 80000000 / NoteFreq;
    				halfPeriod = ticksPeriod / 2;
    			}
    			PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ticksPeriod); 	// Set the period (freq)
    			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, halfPeriod); 		// Set duty cycle to 50% (square wave)

    			SongPos+=2;
    		}else if(numOf16ths > 0){
    			numOf16ths--;
    		}
    		else{
    			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 1); // PWM DC of 0%

    			currSong++;
    			if(currSong >0) //makes it loop the first song
    				SongPos = 0;
    		}
    	}

    	Task_sleep(70); // 50ms
    	audoNoTimeOut++;
    }
}

/**
 * Transmits the error data when on semaphore post in mod bus format
 */
void sendErrorData_func(UArg arg0, UArg arg1) {
    while (true) {
    	Semaphore_pend(BufferAvailableSema,BIOS_WAIT_FOREVER);
    	GreenLEDOn(); //turn green led on during transmit, very hard to see due to speed and blue led on during data collection
    	// Convert data (opposite buffer of current one)

    	int lastBufferIndex = 20;
    	if(thinLineFound == 0){
    		// only occurs for last data output
    		lastBufferIndex = PingPongIncr;
    		PingPongIncr = 0; //not necessary unless
    	}

    	int i = 0;
    	//output header
    	UARTCharPut(UART5_BASE,':');
    	UARTCharPut(UART5_BASE,'1');
    	UARTCharPut(UART5_BASE,'8');

    	//output data
    	for(i = 0; i<lastBufferIndex;i++) //Assumed buffers are 20 entries in size
    	{
    		int data = 0;

    		//bufferselect is what buffer should be printed from, NOT THE ONE BEING NOW WRITTEN to
    		if(PingPongSwitch == 1)
    		{
    			data = PingBuffer[i]; //replace with whatever buffer is used
			}
			else
			{
				data = PongBuffer[i]; //replace with other buffer
			}


			//converts to a floating point for of the distance in cm
			//then casts to int
			//then reduces down to 4 bit max via mask of 15
    		int convertDist = ((int)(fabs(6.615 - ((0.000004) * data * data - 0.0241 * data + 38.815)))) & 15;

			//output form for sprintf use
			//unsure if sprint f will deal with a single char right so null terminate string and convert to hex
			char outputChar[] = {"e\0"};
			sprintf(outputChar, "%x",convertDist);

			//output
			UARTCharPut(UART5_BASE, outputChar[0]);
    	}
    	//output footer
    	UARTCharPut(UART5_BASE,'1');
    	UARTCharPut(UART5_BASE,'8');
    	UARTCharPut(UART5_BASE, '\r');
    	UARTCharPut(UART5_BASE, '\n');

    	GreenLEDOff();
    }
}


// ======= Extra Functions ================
/**
 * creates a ~0.16 [ms] per i
 */
void delay(uint32_t i)
{
	 SysCtlDelay(13400 * i);		// creates ~0.16ms * i delay - TivaWare fxn

}
//turns green led on
void GreenLEDOn(){
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
}

//turns green led off
void GreenLEDOff(){
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
}
