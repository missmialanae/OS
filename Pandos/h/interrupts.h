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
HIDDEN void trapH();




#endif