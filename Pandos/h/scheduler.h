#ifndef scheduler
#define scheduler

/******************Scheduler.h******************************

the external declaration file for the scheduler*/



#include "../h/types.h"

extern void scheduleProcess();
extern void contextSwitch(pcb_t *process);
extern void intervalSwitch(cpu_t specificTime);

#endif
