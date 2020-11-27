#ifndef exceptions 
#define exceptions

/********************Exceptions.h************************************
*
* external declarations for exception
*
*
*/


#include "../h/types.h"

extern void pgmTrapH();
extern void tlbTrapH();
extern void passUpOrDie(int exceptNum);
extern void sysTrapH();
extern void moveState(state_t *source, state_t *copy);

#endif
