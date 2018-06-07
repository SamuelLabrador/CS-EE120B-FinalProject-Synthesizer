#include <avr/io.h>
#include "usart.h"
#include "timer.h"
#include "waveGenerators.h"
#include "pitches.h"
#include <avr/interrupt.h>

unsigned char osc[4], filt[4], amp[4]; 
unsigned char note, noteOn;
unsigned short waveArray[2000];//1291];	//lowest period has a total of 1290 values
unsigned short outputArray[2000];
//should be good needs testing;

unsigned short arraySize = 0x00;
volatile unsigned short pos = 0x00;
unsigned char amplitude = 3;
unsigned short output = 0x00;


ISR(TIMER1_COMPA_vect){
	output = outputArray[pos] * 5;
	PORTC = (char)(output);
	PORTB = (char)(output >> 8);
	pos++;
	if(pos == arraySize){
		pos = 0;
	}
}

unsigned char getPacket();

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0x00; PORTD = 0xFF;

	pos = 0;
	osc[0] = 2;
	//GenerateWaveTable(osc, 50, waveArray, outputArray, &arraySize);
	//lowPassFrequency(outputArray, waveArray, arraySize, 0.99, 0);
	initUSART();
	TimerOn();
	TimerOff();
 
	
	TimerOn();
    while (1) 
    {
		PORTA = note;//converOsc(osc);
		
		if(USART_HasReceived()){
			getPacket();
			TimerOff();
			if(note == 0x00){
				
			}
			else{\
				if(note < 35 || note > 88){
					note = 0x00;
				}
				GenerateWaveTable(osc, sNotePitches[note - 35], waveArray, outputArray, &arraySize);
				lowPassFrequency(outputArray, waveArray, arraySize, filt[0], filt[1]);
				pos = 0;
				TimerOn();
			}
		}
		
    }	
}	

unsigned char getPacket(){
	TimerOff();
	note = USART_Receive();
	USART_Flush();
	
	loadArray(osc, 1);
	loadArray(filt, 2);
	loadArray(amp, 4);
	osc[0] = convertOsc(osc);
}