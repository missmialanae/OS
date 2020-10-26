#ifndef EXCEPTION
#define EXCEPTION
/************************* Exception.H *****************************
*
*  The externals declaration file for the exception
*  
*
* 
*/
#include "../h/types.h"



extern void pgmTrapH();
extern void tlbTrapH();
extern void sysTrapH();
extern void blockCurrent(int *blockSem);
extern void removeProcess(pcb_t *proc);
HIDDEN void passUpOrDie(int except);
extern void createProcess();
extern void passeren();
extern void verhogen();
extern void waitIO();
extern void waitClock();
extern void supportPtr();

#endif