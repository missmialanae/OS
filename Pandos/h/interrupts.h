#ifndef INTERRUPTS
#define INTERRUPTS

/************************* Interrupts.H *****************************
*
*  The externals declaration file for the interrupts file
*    Module.
*
*  
*/

#include "../h/types.h"

/*externs*/
HIDDEN void plt(cpu_t stopTOD);
HIDDEN void pseudoInterrupts();
HIDDEN void IOHandler(int num);

extern int p; 
/*extern pcb_t *currentproc = NULL;*/ 
extern deviceStat;
/*extern bitMap;*/
extern semClock;
extern int intLine;



#endif