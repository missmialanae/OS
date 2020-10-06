#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"
#include "../h/init.h"
#include "../h/scheduler.h"
#include "../h/exception.h"
#include "../h/interrupts.h"

/********************************************************************************
 * 
 * This file contains all methods and instruction for interrupts. This file 
 * controls context switches and deals with the mapping of all the devices
 * through. Deals with Non-timer interrupts, PLT interrupts, the system wide 
 * interval timer and pseudoclock
 * 
 *
 ********************************************************************************/

/******************************** PROGRAM TRAP **********************************/
/* if( in user mode){
	perform pass up and die
} */