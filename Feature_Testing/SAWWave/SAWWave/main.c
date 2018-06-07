/*
 * Test_Timing.c
 *
 * Created: 5/26/2018 3:33:07 PM
 * Author : samla
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B  = 0x0B;		//2000000 Ticks Per Second
	OCR1A = 250;			//should call to ISR every 10 microseconds
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

void fillArray(unsigned short * array);

#define SAMPLE_RATE (unsigned long)(5000)						//Rate at which ports A and B will be updated
#define SAMPLE_PERIOD (unsigned long)( / SAMPLE_RATE)		//1 second = 100000 ticks
#define VOLTAGE_SCALER (unsigned short)(13107)
#define frequency (double)(50)

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned short waveArray[SAMPLE_RATE];
	fillArray(waveArray);
	
	TimerOn();
	TimerSet(SAMPLE_PERIOD);
	
	unsigned short output = 0x00;
	unsigned char lights = 0xFF;
	unsigned short timer = 0x00;
	unsigned short i = 0;
	unsigned short arrSize = sizeof(waveArray)/sizeof(waveArray[0]);
	
	while (1)
	{
		
		i++;
		if(i == SAMPLE_RATE){
			i = 0;
		}
		output = (unsigned short)waveArray[i];
		
		timer++;
		if(timer == SAMPLE_PERIOD){		//TODO CHANGE
			timer = 0;
			lights = ~lights;
		}
		
		PORTC = (char)(output);
		PORTB = (char)(output >> 8);
		PORTA = lights;
		
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

void fillArray(unsigned short *array){

	double amplitude = 5.0;
	double timePassed = 0.0;
	double phase = 0;
	double delta_T = (double)(1.0 / (double)(SAMPLE_RATE));
	unsigned short i = 0x00;

	for(i = 0; i < SAMPLE_RATE; i++){	//sample rate is used
		array[i] = (short)(amplitude * frequency * fmod((timePassed + phase), (1/frequency)) * VOLTAGE_SCALER);
		timePassed += delta_T;
	}
	return;
}

