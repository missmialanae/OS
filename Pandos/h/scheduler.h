#ifndef scheduler
#define scheduler

/******************Scheduler.h******************************

the external declaration file for the scheduler*/



#include "../h/types.h"

extern void scheduleProcess();
extern void contextSwitch(pcb_t *next);
extern void intervalSwitch(cpu_t getTime);

#endif
