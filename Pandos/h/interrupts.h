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
HIDDEN void localTimer(cpu_t stopTOD);
HIDDEN void timerInt();
HIDDEN void IOHandler(int lineNum);

#endif