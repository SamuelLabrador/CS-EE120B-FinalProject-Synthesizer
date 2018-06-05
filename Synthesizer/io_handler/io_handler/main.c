/*
 * io.c
 *
 * Created: 6/3/2018 9:07:01 PM
 * Author : samla
 */ 


#include <avr/io.h>
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
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;	//LCD data out
	DDRD = 0xFF; PORTD = 0x00;	//bits 6 and 4 input Rx in || everything else is output. Tx and LCD control
	
	initExtern();
	
	unsigned char i = 0x00;
	unsigned char taskCount = 1;
	task menu;
	
	tasks[0] = menu;
	tasks[0].period = 1;
	tasks[0].state = 0;
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


unsigned char isUpdated = 0x00; //0000 0000
unsigned char oldValues[4];

//menuTask handles the main I/O
unsigned char menuTask(unsigned char currentState){
	
	unsigned char parameters[4];
	getPotentiometerSnapshot(parameters);
	
	switch(currentState){	//state transition calculations
		
		case(INIT):
			osc[0] = 1;		//waveform = saw
			filt[0] = 0;	//lp filter amt = 0
			filt[0] = 0;	//resonance = 0
			amp[0] = 0;		//attack = 0
			amp[1] = 0;		//decay = 0
			amp[2] = 99;	//sustain = 99
			amp[3] = 0;		//release = 0
		
			currentState = calcMenu();
			break;
		
		case(RELEASE):						//wait for button release
			
			if(isPressed()){
				return currentState; //NO ACTIONS PREFORMED IF BUTTON IS STILL PRESSED, can return immediately
			}
			isUpdated = 0x00;
			switch(previousMenuState){
				case(OSC1WAIT):
					currentState = OSC1MAIN;
					break;

				case(OSC1MAIN):
					currentState = calcMenu();
					break;
			
				case(FILTWAIT):
					currentState = FILTMAIN;
					break;
					
				case(FILTMAIN):
					currentState = calcMenu();
					break;
			
				case(AMPWAIT):
					currentState = AMPMAIN;
					break;
			
				case(AMPMAIN):
					currentState = calcMenu();
					break;
			
				default:
					LCD_DisplayString(1, "NO BACK STATE FOUND!");
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
				LCD_DisplayString(1, "WAV"); //LVL ");//OCT DET");
				copyValues(oldValues, parameters);
				updateOscParameter(osc);
			
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
				previousMenuState = FILTWAIT;
				currentState = RELEASE;	//OSC_1 Release
				LCD_DisplayString(1, "AMT RES"); //MOD TYPE");
				copyValues(oldValues, parameters);
				updateFilterParameter(filt);
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
				LCD_DisplayString(1, "A   D   S   R");
				copyValues(oldValues, parameters);
				updateAmpParameter(amp);
			}
			break;
		
		case(AMPMAIN):
			if(isPressed()){
				previousMenuState = AMPMAIN;
				currentState = RELEASE; //WAIT FOR RELEASE
			}
			break;
		
		default:
			LCD_DisplayString(1, "ERROR no defined state transition");
	}
	
	switch(currentState){	//state actions
		case(OSC1PREP):
			LCD_DisplayString(1, "  OSCILLATOR 1");
			LCD_Cursor(0);
			break;
		
		case(OSC1MAIN):
			updateValues(parameters, oldValues, osc, &isUpdated);
			updateOscParameter(osc);
			break;
		
		case(FILTPREP):
			LCD_DisplayString(1, "     FILTER");
			LCD_Cursor(0);
			break;
		
		case(FILTMAIN):
			updateValues(parameters, oldValues, filt, &isUpdated);
			updateFilterParameter(filt);
			break;
		
		case(AMPPREP):
			LCD_DisplayString(1, "   AMPLIFIER");
			LCD_Cursor(0);
			break;
		
		case(AMPMAIN):
			updateValues(parameters, oldValues, amp, &isUpdated);
			updateAmpParameter(amp);
			break;
	}
	return currentState;
}

