#ifndef TIMER_H
#define TIMER_H

#include <avr/interrupt.h>

extern volatile unsigned char TimerFlag = 0;

extern unsigned long _avr_timer_M = 1;
extern unsigned long _avr_timer_cntcurr = 0;

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

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerFlag = 1;
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

#endif