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
		return 5;		//FILT PREP state
	}
	else{
		return 9;		//AMP PREP state
	}
}

#endif