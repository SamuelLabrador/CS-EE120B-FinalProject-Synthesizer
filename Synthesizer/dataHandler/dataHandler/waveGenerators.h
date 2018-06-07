#ifndef WAVEGENERATORS
#define WAVEGENERATORS

#define SAMPLES 20000
#define VOLTAGE_SCALER 13107
#define SAMPLE_PERIOD (double)(1.0 / SAMPLES)
#define PI 3.141592

void generateSaw(unsigned short frequency, unsigned short* waveArray, unsigned short * arraySize){
	unsigned short i = 0;
	double j = 0;
	double x = 0;
	for(i = 0; i < (SAMPLES /frequency); i++){
		waveArray[i] = (short)(abs(fmod(frequency * j + 1, 2) - 1) * VOLTAGE_SCALER);	//interesting wave
		
		j += SAMPLE_PERIOD;
	}
	*arraySize = i;
}

void generateSqr(unsigned short frequency, unsigned short* waveArray, unsigned short * arraySize){
	unsigned short i = 0;
	unsigned char pwm = 2;
	for(i = 0; i < (SAMPLES / frequency); i++){
		if(i < (SAMPLES/frequency/pwm)){
			waveArray[i] = 0x01 * VOLTAGE_SCALER;
		}
		else{
			waveArray[i] = 0x00 * VOLTAGE_SCALER;	
		}
	}
	*arraySize = i;
}

void generateSin(unsigned short frequency, unsigned short* waveArray, unsigned short * arraySize){
	unsigned short i = 0;
	double j = 0;
	double x = 0;
	for(i = 0; i < (SAMPLES /frequency); i++){
		waveArray[i] = (short)(abs(fmod(frequency * j + 1, 2) - 1) * VOLTAGE_SCALER);	//interesting wave
		
		j += SAMPLE_PERIOD;
	}
	*arraySize = i;
}

void GenerateWaveTable(	unsigned char * osc,
						unsigned short frequency,
						unsigned short * waveArray,
						unsigned short * arraySize){
	switch(osc[0]){
		case(0):
		generateSaw(frequency, waveArray, arraySize);
		break;
		
		case(1):
		generateSqr(frequency, waveArray, arraySize);
		break;
		
		case(2):
		generateSin(frequency, waveArray, arraySize);
		break;
	}
}

#endif