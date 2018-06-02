/*
 * Blink LED.c
 *
 * Created: 6/1/2018 2:38:19 PM
 * Author : samla
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B  = 0x0A;		//00001010		16,000,000 Ticks per second / 8 = 2,000,000 Ticks per second
	OCR1A = 20;			//ISR is called to every 0.00001 second
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



int main(void)
{
    DDRA = 0xFF; PORTA = 0x00;
	//DDRB = 0xFF; PORTB = 0x00;
	
	TimerOn();
	TimerSet(100000);
	
	unsigned char lights = 0x00;
	
	while (1) 
    {
		lights = ~lights;
		PORTA = lights;
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

