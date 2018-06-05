/*
 * dataHandler.c
 *
 * Created: 6/4/2018 11:36:09 PM
 * Author : samla
 */ 

#include <avr/io.h>
#include "usart.h"
#include <avr/interrupt.h>

int main(void)
{
	initUSART();
    DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0x00; PORTD = 0xFF;
	
	unsigned char lights = 0x00;
    while (1) 
    {
		PORTC = USART_Receive();
		
    }
}

