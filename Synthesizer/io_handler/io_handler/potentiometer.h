#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <util/delay.h>
#define F_CPU 16000000UL

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

unsigned char scaleTo100(){		
	return (ADC - 32.0)/9.77;	//THIS EQUATION IS UNIQUE TO THE POTENTIOMETERS I USED
}

unsigned char captureSingleKnob(unsigned char pos){
	ADMUX = pos;
	_delay_us(500);		//DONT USE asm("nop") -- causes read error
	return scaleTo100();
}

void getPotentiometerSnapshot(unsigned char * array){	//5 potentiometers
	unsigned char i;
	for(i = 0; i < 4; i = i + 1){
		array[i] = captureSingleKnob(i + 1);
	}
}

unsigned char isPressed(){
	return (~PINA & 0x20);
}


#endif