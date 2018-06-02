/*
 * io_handler.c
 *
 * Created: 5/30/2018 11:33:55 PM
 * Author : samla
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "scheduler.h"
#include "timer.h"
#include "synth_module.h"
#include "potentiometer.h"	//includes menu functions
#include "menu.h"
#include "io.c"

//GLOBAL VARIABLES
task tasks[1];						//menuTask
unsigned char menuValues[4];
unsigned char previousMenuState;	//help reduce number of states
unsigned char osc[4], amp[4], filt[4];
//forward function declerations
void initExtern();	//used to initialize external variables
unsigned char menuTask(unsigned char);

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;	//potentiometer inputs
	DDRC = 0xFF; PORTC = 0x00;	//LCD data out
	DDRD = 0xFF; PORTD = 0x00;	//bits 6 and 4 input Rx in || everything else is output. Tx and LCD control
	
	initExtern();
	
	unsigned char i = 0x00;
	unsigned char taskCount = 1;
	task menu;
	tasks[0] = menu;
	
	tasks[0].period = 50;
	tasks[0].elapsedTime = 0;
	tasks[0].TickFunction = &menuTask;
	
	ADC_init();		//init ADC
	LCD_init();		//init LCD
	TimerSet(5);	//set timer interrupt cycle period
	TimerOn();		//enable timer
	
    while (1) 
    {
		for(i = 0; i < taskCount; i++){
			if(tasks[i].elapsedTime >= tasks[i].period){
				tasks[i].state = tasks[i].TickFunction(tasks[i].state);
				tasks[i].elapsedTime = 0;
			}
			tasks[i].elapsedTime += 5;
		}
		TimerFlag = 0;
		while(!TimerFlag);
    }
}

void initExtern(){
	TimerFlag = 0;
	_avr_timer_cntcurr = 0;
	_avr_timer_M = 0;
}

//menuTask handles the main I/O
unsigned char menuTask(unsigned char currentState){
	
	switch(currentState){	//state transition calculations
		case(INIT):
			currentState = calcMenu();
			break;
		
		case(RELEASE):						//wait for button release
			if(isPressed()){
				return currentState; //NO ACTIONS PREFORMED IF BUTTON IS STILL PRESSED, can return immediately
			}
			else if(previousMenuState == OSC1WAIT){
				currentState = OSC1MAIN;	
			}
			else if(previousMenuState == OSC1MAIN){
				currentState = calcMenu();
			}
			else if(previousMenuState == FILTWAIT){
				currentState = FILTMAIN;
			}
			else if(previousMenuState == FILTMAIN){
				currentState = calcMenu();
			}
			else if(previousMenuState = AMPWAIT){
				currentState = AMPMAIN;
			}
			else if(previousMenuState = AMPMAIN){
				currentState = calcMenu();
			}
			break;
		
		case(OSC1PREP)://OSC_1 PREP
			currentState = OSC1WAIT;
			break;
		
		case(OSC1WAIT)://OSC_1 WAIT
			if(calcMenu() != OSC1PREP){			//data knob priority over button press
				currentState = calcMenu();
			}
			else if(isPressed()){
				previousMenuState = OSC1WAIT;
				currentState = RELEASE;	//OSC_1 Release
			}
			break;
			
		case(OSC1MAIN):
			if(isPressed()){
				previousMenuState = OSC1MAIN;
				currentState = RELEASE; //WAIT FOR RELEASE
			}
			break;
			
		case(FILTPREP):
			currentState = FILTWAIT;
			break;
			
		case(FILTWAIT):
			if(calcMenu() != FILTPREP){			//data knob priority over button press
				currentState = calcMenu();
			}
			else if(isPressed()){
				previousMenuState = FILTMAIN;
				currentState = RELEASE;	//OSC_1 Release
			}
			break;
			
		case(FILTMAIN):
			if(isPressed()){
				previousMenuState = FILTMAIN;
				currentState = RELEASE; //WAIT FOR RELEASE
			}
			break;
			
		case(AMPPREP):
			currentState = AMPWAIT;
			break;
			
		case(AMPWAIT):
			if(calcMenu() != AMPPREP){			//data knob priority over button press
				currentState = calcMenu();
			}
			else if(isPressed()){
				previousMenuState = AMPWAIT;
				currentState = RELEASE;	//OSC_1 Release
			}
			break;
			
		case(AMPMAIN):
			if(isPressed()){
				previousMenuState = AMPMAIN;
				currentState = RELEASE; //WAIT FOR RELEASE
			}
			break;
		default:
			LCD_DisplayString(1, "ERROR default state");
	}
	
	switch(currentState){
		case(OSC1PREP):
			LCD_DisplayString(1, "OSCILLATOR 1");
			
		case(OSC1MAIN):
			
			updateParameterValue(menuValues);
			
	}
	return currentState;
	
	
	
	return 0x01;
}