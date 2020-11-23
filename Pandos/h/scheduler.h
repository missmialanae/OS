#ifndef SCHEDULER
#define SCHEDULER
/************************* Scheduler.H *****************************
*
*  The externals declaration file for the scheduler
*  
*
* 
*/
#include "../h/types.h"

extern void moveState(state_PTR source, state_PTR final);
extern void contextSwitch(pcb_t *currentproc);
extern void scheduler();
extern void intervalSwitch(cpu_t time, pcb_t* proc);

#endif