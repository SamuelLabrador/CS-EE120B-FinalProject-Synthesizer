#ifndef MENU_H
#define MENU_H
typedef enum Menu {INIT, RELEASE, OSC1PREP, OSC1WAIT, OSC1MAIN, FILTPREP, FILTWAIT, FILTMAIN, AMPPREP, AMPWAIT, AMPMAIN} Menu;
#define MENU_ITEMS 3


Menu calcMenu(){
	unsigned char value = captureSingleKnob(0);
	if(value < 33){
		return OSC1PREP;		//OSC_1 PREP state
	}
	else if(value < 66){
		return FILTPREP;		//FILT PREP state
	}
	else{
		return AMPPREP;			//AMP PREP state
	}
}

void updateFilterParameter(unsigned char* array){
	for(unsigned char i = 0; i < 2; i++){
		LCD_Cursor(17 + i * 4);
		LCD_WriteData(array[i] / 10 + 48);
		LCD_Cursor(18 + i * 4);
		LCD_WriteData(array[i] % 10 + 48);
		
	}
	LCD_Cursor(0);
}

void updateAmpParameter(unsigned char * array){
	unsigned char i;
	char str[2];
	for(i = 0; i < 4; i++){
		str[0] = array[i] / 10 + 48;
		str[1] = array[i] % 10 + 48;
		
		LCD_Cursor(17 + i * 4);
		LCD_WriteData(str[0]);
		LCD_Cursor(18 + i * 4);	
		LCD_WriteData(str[1]);
	}
	LCD_Cursor(0);
}

void updateOscParameter(unsigned char * array){
	unsigned char str[3];
	
	if(array[0] < 33){
		str[0] = 0x53;
		str[1] = 0x41;
		str[2] = 0x57;
	}	
	else if(array[0] < 66){
		str[0] = 0x53;
		str[1] = 0x51;
		str[2] = 0x52;
	}
	else{
		
		str[0] = 0x54;
		str[1] = 0x52;
		str[2] = 0x49;
	}

	for(unsigned char i = 0; i < 3; i++){
		LCD_Cursor(17 + i);
		LCD_WriteData(str[i]);
	}	
	LCD_Cursor(0);
	
	/*
	LCD_Cursor(21);			// Level Output
	LCD_WriteData(array[1] / 10 + 48);
	LCD_Cursor(22);
	LCD_WriteData(array[1] % 10 + 48);
	*/
}
#endif