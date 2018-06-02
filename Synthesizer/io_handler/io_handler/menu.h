#ifndef MENU_H
#define MENU_H
typedef enum Menu {INIT, RELEASE, OSC1PREP, OSC1WAIT, OSC1MAIN, FILTPREP, FILTWAIT, FILTMAIN, AMPPREP, AMPWAIT, AMPMAIN} Menu;
#define MENU_ITEMS 3


Menu calcMenu(){
	unsigned char value = captureSingleKnob(0)/MENU_ITEMS;
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

void updateParameterValue(unsigned char * array){
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

#endif