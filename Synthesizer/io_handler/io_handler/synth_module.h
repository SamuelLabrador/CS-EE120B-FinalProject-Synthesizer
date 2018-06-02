#ifndef SYNTH_MODULE_H
#define SYNTH_MODULE_H
#include "potentiometer.h"

typedef struct Oscillator{		//
	unsigned char waveForm;
	unsigned char octave;
	unsigned char detune;
	unsigned char level;	
	} OSC;

typedef struct Amplifier{		//envelope
	unsigned char attack;
	unsigned char decay;
	unsigned char sustain;
	unsigned char release;
	} AMP;
	
typedef struct Filter{
	unsigned char amount;
	unsigned char resonance;
	unsigned char envAmount;	
	unsigned char type;		//highpass or lowpass
	} FILT;

#endif