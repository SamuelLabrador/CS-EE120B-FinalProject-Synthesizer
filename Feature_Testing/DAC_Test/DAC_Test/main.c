/*
 * DAC_Test.c
 *
 * Created: 5/24/2018 2:15:47 PM
 * Author : samla
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;	// 1,000,000 ticks per second

	OCR1A = 125;	// Output compare register -- when OCR1A == TCNT1 || an interrupt will occur
	
	TIMSK1 = 0x02;	// bit1: OCIE1A -- enables compare match interrupt -- 

	TCNT1=0;		//init TCNT1

	_avr_timer_cntcurr = _avr_timer_M;
	
	SREG |= 0x80;	// Set bit 7 to high to enable interrupts
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

#define DAC_SCALAR 13107
#define SAMPLE_RATE 41000

int main(void)
{
    DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
    
	
	//2000	= 1 tick == 10000 second
	//1000	= 1 tick == 1 millisecond
	//500	= 1 tick == 500 millisecond
	//1		= 1 tick == 1 micro-second 
	
	TimerSet(1000);	//2 micro second period
	TimerOn();
	
	unsigned long output = 0;
	double amplitude = 5;		//in V
	double frequency = 261.63;	//in Hz
	double phase = 0;			//in degrees
	unsigned char cycles = 0;		//in micro-seconds
	double timePassed = 0.0;
	
	unsigned char right = 0x00;
	unsigned char left = 0x00;
	
	while (1) 
    {	
		/*
		output = round(amplitude * (frequency) * fmod((timePassed + phase), (1/frequency))) * DAC_SCALAR;

		timePassed += 0.000016;
		cycles++;
		if(cycles >= 62500){	//125000 is the amount it takes for a second to pass 25 * 5000 == 125000
			cycles = 0;
			timePassed = 0;
		}
		
		right = (char)(output);
		left = (char)(output >> 8);
		*/
		
		right = ~right;
		PORTA = right;
		PORTB = left;
		PORTC = right;
		
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

