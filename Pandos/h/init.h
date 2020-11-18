#ifndef INIT
#define INIT
/************************* Init.H *****************************
*
*  The externals declaration file for the inital/ main
*  
*
* 
*/
#include "../h/types.h"

extern void uTLB_RefillHandler ();
extern void test();
extern void GenExceptionHander();
extern int debuggerA(int a);
extern int debuggerB(pcb_t *t);


/*Process Count*/
extern int processcnt;

/*Soft-block count*/
extern int softBlock;

/*ready queue*/
extern pcb_t *readyQueue;

/*setting current process */
extern pcb_t *currentproc; 

/*device semaphores */
extern int devices[DEVICECNT + DEVPERINT + 1];

/*time unit*/
extern cpu_t startTOD;

/*amt till time slice*/
extern cpu_t *sliceCount; /*do I need this now*/

extern unsigned int saveStat[ DEVICECNT + DEVPERINT];


#endif
