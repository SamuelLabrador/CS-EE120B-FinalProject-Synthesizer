#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct task{
	unsigned long period;
	unsigned long elapsedTime;
	unsigned char state;
	unsigned char (*TickFunction)(unsigned char);
	} task;
	
#endif