#ifndef WAVEGENERATORS
#define WAVEGENERATORS

#define SAMPLES 20000
#define VOLTAGE_SCALER 13107
#define SAMPLE_PERIOD (double)(1.0 / SAMPLES)
#define PI (double)(3.141592)
#define e (double) (2.71828)
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
	double period = 1 / frequency;
	double j = 0;
	double sinMultiplier = 2.0 * PI / (1.0 / frequency);
	for(i = 0; i < (SAMPLES / frequency); i++){
		waveArray[i] = (short)((sin(sinMultiplier * j) * 0.5 + 0.5) * 150);
		j += SAMPLE_PERIOD;
	}
	*arraySize = i;
}

void GenerateWaveTable(	unsigned char * osc,
unsigned short frequency,
unsigned short * waveArray,
unsigned short * outputArray,
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
	
	copyArray(outputArray, waveArray, arraySize);
}

void copyArray(unsigned short * target, unsigned short* base, unsigned short size){
	for(unsigned short i = 0; i < size; i++){
		target[i] = base[i];
	}
}

void lowPassFrequency(unsigned short* output, unsigned short* input, unsigned short arraySize, double cutoff, double resonance){
	
	
	//set feedback amount given f and q between 0 and 1
	double fb = resonance + resonance/(1.0 - cutoff);
	
	double buf0 = 0;
	double buf1 = 0;
	for(unsigned short i = 0; i < arraySize; i++){
		//for each sample...
		buf0 = buf0 + cutoff * (input[i] - buf0 + fb * (buf0 - buf1));
		buf1 = buf1 + cutoff * (buf0 - buf1);
		output[i] = buf1;
	}
}

unsigned char convertOsc(unsigned char * array){
	if(array[0] < 33){
		return 0;
	}
	else if(array[0] < 66){
		return 1;
	}
	else {
		return 2;
	}
}
#endif