#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct task{
	unsigned long period;
	unsigned long elapsedTime;
	unsigned char state;
	unsigned char (*TickFunction)(unsigned char);
	} task;
	
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}

#endif