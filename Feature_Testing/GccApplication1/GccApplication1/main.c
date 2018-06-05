/*
 * GccApplication1.c
 *
 * Created: 6/3/2018 7:22:27 PM
 * Author : samla
 */ 

#include <avr/io.h>

#include "io.c"
int main(void)
{
	DDRC = 0xFF; PORTC = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	
	LCD_init();
	LCD_ClearScreen();
	LCD_DisplayString(1, "Hello World");
    while (1) 
    {
    }
}

