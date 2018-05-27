/*
 * TimerInterrupt.c
 *
 * Created: 5/25/2018 11:18:26 PM
 * Author : samla
 */ 

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

unsigned long scaleTo100();
unsigned long invertScaleTo100();

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
	
	ADC_init();
    
	unsigned long output = 0x0000;
	
	while (1) 
    {
		output = invertScaleTo100();
		PORTC = (char)(output);
		PORTB = (char)(output >> 8);
    }
}

unsigned long scaleTo100(){
	return (ADC - 31.0)/9.77;
}

unsigned long invertScaleTo100(){	
	return (100 - (ADC - 31.0)/9.77);
}
