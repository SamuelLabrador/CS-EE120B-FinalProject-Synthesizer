/*
 * softwareReset.c
 *
 * Created: 6/3/2018 8:54:51 PM
 * Author : samla
 */ 

#include <avr/io.h>


int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	
    /* Replace with your application code */
    while (1) 
    {
		PORTB = 0xFF;
    }
}

