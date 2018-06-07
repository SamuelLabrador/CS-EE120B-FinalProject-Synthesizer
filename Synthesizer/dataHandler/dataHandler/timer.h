#ifndef TIMER_H
#define TIMER_H

#include <avr/interrupt.h>

extern volatile unsigned char TimerFlag = 0;

extern unsigned long _avr_timer_M = 1;
extern unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B  = 0x0A;		//2000000 Ticks Per Second
	OCR1A = 100;			//should call to ISR every 10 microseconds
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;	//enable interrupts via SREG
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}
/*
ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
*/
void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


#endif