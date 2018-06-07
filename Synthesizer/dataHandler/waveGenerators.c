#ifndef WAVEGENERATORS
#define WAVEGENERATORS
#include <>
#define SAMPLES 40000
#define SAMPLE_PERIOD (double)(1.0 / (double)(SAMPLES))
#define VOLTAGE_SCALAR 13107

void generateSaw(unsigned short frequency, unsigned char* waveArray, unsigned short * arraySize){
	double wavePeriod = 1.0 / (double)(frequency);
	unsigned short j = 0x00;
	for(unsigned double i = 0; i < wavePeriod; i += SAMPLE_PERIOD){
		waveArray[j] = (short)( frequency * fmod((i), (1/frequency)) * VOLTAGE_SCALAR);
		j++;
	}
	*arraySize = j;
}

void generateSquare(unsigned short frequency, unsigned char* waveArray, unsigned short * arraySize){
	double wavePeriod = 1.0 / (double)(frequency);
	unsigned short j = 0x00;
	for(unsigned double i = 0; i < wavePeriod/2; i += SAMPLE_PERIOD){
		waveArray[j] = 0x01;
		j++;
	}
	for(unsigned double i = wavePeriod/2; i < wavePeriod; i += SAMPLE_PERIOD){
		waveArray[j] = 0x00;
		j++;
	}
	*arraySize = j;
}

void generateSin(unsigned short frequency, unsigned char* waveArray, unsigned short* arraySize){
	double wavePeriod = 1.0 / (double)(frequency);
	unsigned short j = 0x00;
	for(unsigned double i = 0; i < wavePeriod; i += SAMPLE_PERIOD){
		waveArray[j] = sin(i * 2 * 3.14 / wavePeriod)
		j++;
	}
	*arraySize = j;
}








void GenerateWaveTable(	unsigned char * osc, 
						unsigned short frequency, 
						unsigned char * waveArray, 
						unsigned char * arraySize){
	switch(osc[0]){
		case(0):
			generateSaw(frequency, waveArray, arraySize);
			break;
			
		case(1):
			generateSqr(frequency, waveArray, arraySize);
			break;
			
		case(2):
			generateTri(frequency, waveArray, arraySize);
			break;
	}
}

#endif