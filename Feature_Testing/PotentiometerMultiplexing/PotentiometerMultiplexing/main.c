/*
 * PotentiometerMultiplexing.c
 *
 * Created: 5/29/2018 8:39:54 PM
 * Author : samla
 */ 


#include "io.c"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B  = 0x0B;		//1000000 Ticks Per Second
	OCR1A = 125;			//should call to ISR every 10 microseconds
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

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

unsigned long scaleTo100();
unsigned long invertScaleTo100();

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	ADC_init();
	
	char output = 0x00;
	char knob_1 = 0x00;
	char knob_2 = 0x00;
	char knob_3 = 0x00;
	char knob_4 = 0x00;
	char knobs[] = {knob_1, knob_2, knob_3, knob_4};
	char str[2];
	char count = 0x00;
	
	TimerSet(10);
	TimerOn();
	
	LCD_init();
	LCD_DisplayString(1, "A   D   S   R");
	
	ADMUX = 0x01;
	
	while (1)
	{	
		output = scaleTo100();
		if(output != knobs[count]){
			
			str[0] = (char)(output/10) + 48;
			str[1] = output % 10 + 48;
			
			LCD_Cursor(17 + count * 4);
			LCD_WriteData(str[0]);
			LCD_Cursor(18 + count * 4);
			LCD_WriteData(str[1]);
			LCD_Cursor(0);
			knobs[count] = output;
		}
		
		count++;
		if(count > 3){
			count = 0;
		}
		
		ADMUX = count + 1;
		
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

unsigned long scaleTo100(){
	return (ADC - 32.0)/9.77;
}

unsigned long invertScaleTo100(){
	return (100 - (ADC - 31.0)/9.77);
}
