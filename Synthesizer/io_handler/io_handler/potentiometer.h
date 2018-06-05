#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <util/delay.h>
#define F_CPU 16000000UL

void ADC_init() {
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE);	//0x06 MIGHT FIX PROBLEM
}

unsigned char scaleTo100(){		
	return (ADC - 32.0)/9.77;	//THIS EQUATION IS UNIQUE TO THE POTENTIOMETERS I USED
}

unsigned char captureSingleKnob(unsigned char pos){
	ADMUX = 0b00011111 & pos;	//MIGHT CAUSE ADC PROBLEMS
	asm("nop");
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

void copyValues(unsigned char * copy, unsigned char * target){
	for(unsigned char i = 0; i < 4; i++){
		copy[i] = target[i];
	}
}

void updateValues(unsigned char * parameters, unsigned char * oldValues, unsigned char * module, unsigned char * isUpdated){
	for(unsigned char i = 0; i < 4; i++){
		if((*isUpdated & (1 << i)) == (1 << i)){
			module[i] = parameters[i];
		}
		else if(oldValues[i] != parameters[i]){
			module[i] = parameters[i];
			*isUpdated |= (1 << i);
		}
	}
}

#endif