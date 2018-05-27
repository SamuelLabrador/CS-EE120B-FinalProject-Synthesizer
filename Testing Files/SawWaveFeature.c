/*
 * Test_Timing.c
 *
 * Created: 5/26/2018 3:33:07 PM
 * Author : samla
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B  = 0x0A;
	OCR1A = 100;	//should call to ISR every 100 microseconds 
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

void fillArray(unsigned short *array);

#define SAMPLE_RATE (unsigned short)(1000)						//Rate at which ports A and B will be updated
#define SAMPLE_PERIOD (unsigned short)(10000 / SAMPLE_RATE)		//ISR triggers every 100 microseconds
#define VOLTAGE_SCALER (unsigned short)(13107)

int main(void)
{
    DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned short *waveArray[SAMPLE_RATE];
	fillArray(waveArray);
	
	TimerOn();
	TimerSet(SAMPLE_PERIOD);
	
	unsigned short output = 0x00;
	unsigned char lights = 0xFF;
	unsigned short i = 0;
	
    while (1) 
    {
		i++;
		if(i == SAMPLE_RATE){
			i = 0;
			lights = ~lights;
		}
		output = waveArray[i];
		
		PORTC = lights;
		PORTA = (char)(output);
		PORTB = (char)(output >> 8);
		
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

void fillArray(unsigned short *array){
	
	double frequency = 2;
	//double period = 1.0 / frequency;
	double amplitude = 5.0;
	double timePassed = 0.0;
	double phase = 0;
	unsigned short i = 0x00;
	
	
	for(i = 0; i < SAMPLE_RATE; i++){
		array[i] = (short)(amplitude * frequency * fmod((timePassed + phase), (1/frequency)) * VOLTAGE_SCALER);
		timePassed += (double)(1.0 / (double)(SAMPLE_RATE));
	}
	return;
}

