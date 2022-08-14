
#ifndef __commands_h

#define __commands_h

//include statements
#include <stdint.h>

//define statements
#define NUM_OF_COMMANDS 17 //update everytime a command is added

//function declares
void GreenLEDToggle();
void BlueLEDToggle();
void RedLEDToggle();
//distance sensor commands
void ReadFrontADCValue();
void ReadSideADCValue();
//motor
void motorRStart();
void motorLStart();
void motorRStop();
void motorLStop();
void motorRF();
void motorRS();
void motorLF();
void motorLS();
void PIDOn();
void PIDOff();
void collectDataEnable();
void collectDataDisable();

void InvalidCommand();

int findCommandNum(char str[2]);

//command list
extern char command_lookup[NUM_OF_COMMANDS][2];

//command functions
extern void (*command_funcs[NUM_OF_COMMANDS + 1])();

//other varribles
uint32_t ADCvalues[4];
int On_Flag;
int thinLineFound;

//forward declerations for uart, should move uart to its own files latter on

#endif
