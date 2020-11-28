#ifndef initial
#define initial

/************************** Initial.h******************************
*
*  The externals declaration file for intial/main
*  
*
*  
*/

#include "../h/types.h"

extern int main();


extern pcb_t *currentproc;
extern int processcnt; 
extern int softBlock; 
extern pcb_t *readyQueue;
extern int devices[DEVCNT+DEVPERINT+1];
extern cpu_t startTOD;
extern cpu_t timeSlice;
extern unsigned int saveState[DEVCNT+DEVPERINT];

#endif
