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
task tasks[1];			//ADC multiplexing, LCD_MENU, USART, MIDI 

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B  = 0x0B;		//250,000 Ticks Per Second
	OCR1A = 250;		//ISR is called to every 1 millisecond
	TIMSK1 = 0x02;		//enable CTC mode
	TCNT1 = 0;			//Timer count register
	_avr_timer_cntcurr = _avr_timer_M;
	sei();				//enable interrupts
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void getPotentiometerSnapshot(unsigned char * array);

unsigned char menuTick(unsigned char state);	//forward decleration

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;	//potentiometer inputs
	DDRC = 0xFF; PORTC = 0x00;	//LCD data out
	DDRD = 0xFF; PORTD = 0x00;	//bits 6 and 4 input Rx in || everything else is output. Tx and LCD control
	
	unsigned char i = 0x00;
	unsigned char taskCount = 1;
	task menu;
	tasks[0] = menu;
	
	tasks[0].period = 50;
	tasks[0].elapsedTime = 0;
	tasks[0].state = 0x00;
	tasks[0].TickFunction = &menuTick;
	
	ADC_init();		//init ADC
	LCD_init();		//init LCD
	TimerSet(5);	//set timer interrupt cycle
	TimerOn();		//enable timer
	
    while (1) 
    {
		for(i = 0; i < taskCount; i++){
			if(tasks[i].elapsedTime >= tasks[i].period){
				tasks[i].state = tasks[i].TickFunction(tasks[i].state);
				tasks[i].elapsedTime = 0;
			}
			tasks[i].elapsedTime += 100;
		}
		TimerFlag = 0;
		while(!TimerFlag);
    }
}

//main, sub_prep, sub_main
//states
//0 = INIT
//1 = OSC_1 PREP
//2 = OSC_1 WAIT
//3 = OSC_1 RELEASE
//4 = OSC_1 MAIN
//6 = FILT PREP
//7 = FILT WAIT
//8 = FILT RELEASE
//9 = FILT MAIN
//10 = AMP PREP
//11 = AMP WAIT
//12 = AMP RELEASE
//13 = AMP MAIN

unsigned char previousState;
unsigned char menuTick(unsigned char state){
	
	switch(state){	//state transition calculations
		case(INIT):
			state = calcMenu();
			break;
		case(OSC1PREP)://OSC_1 PREP
			state = OSC1WAIT;	
			break;
		case(OSC1WAIT)://OSC_1 WAIT
			if(calcMenu() != OSC1PREP){	//data knob priority over button press
				state = calcMenu();
			}
			else if(isPressed()){
				previousState = OSC1WAIT;
				state = RELEASE;	//OSC_1 Release
			}
			break;
		case(RELEASE):
			if(isPressed()){
				return state;		//NO ACTIONS PREFORMED ON WAIT STATE
			}
			if(previousState == OSC1WAIT){
				state = OSC1MAIN;
			}
			else if(previousState == OSC1MAIN){
				state = calcMenu();
			}
			else if(previousState == FILTWAIT){
				state = FILTMAIN;
			}
			else if(previousState == FILTMAIN){
				state = calcMenu();
			}
			else if(previousState = AMPWAIT){
				state = AMPMAIN;
			}
			else if(previousState = AMPMAIN){
				state = calcMenu();
			}
			break;
		case(OSC1MAIN):
			if(isPressed()){
				state = RELEASE; //WAIT FOR RELEASE
			}
			break;
		case(5):
			if(isPressed()){
				return 5;
			}
			state = 
			
			
			
			
	}
	unsigned char knobs[10];
	getPotentiometerSnapshot(knobs);
	unsigned char button = PORTA & 0x10;
	char str[2];
	
	unsigned char i;
	for(i = 0; i < 5; i++){
		str[0] = knobs[i] / 10 + 48;
		str[1] = knobs[i] % 10 + 48;
		
		LCD_Cursor(1 + i * 4); //used to be 17
		LCD_WriteData(str[0]);
		LCD_Cursor(2 + i * 4);	//used to be 18
		LCD_WriteData(str[1]);
	}
	
	LCD_Cursor(0);
	
	return 0x01;
}