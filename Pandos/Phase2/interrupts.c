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
 /********************************************************************************/


/******************************** PROGRAM TRAP **********************************/
/* if( in user mode){
	perform pass up and die
} */


int p; /* the local variable */


void localTimer(){

if(currentproc != NULL){
	currentproc -> p_time = stopTOD - startTOD; /*change the time to the current process*/

	moveState(currentproc); /*store the processor state using move state*/

	scheduler(); /*when its the end of processor's time (sliceCount), invoke the schedular*/

	insertProc(readyqueue, currentproc); /* call insertProc*/

	switch(); /*call switch */
	/* also why is she red, do we not have switchProcess method?*/

}

else if(currentProc = NULL){ /* if there is no current process running call panic*/
	
	PANIC();
}

} /***END OF localTime****/


void timerInt(){

	/* ACK interrupt using LDIT */

	semClock = 0; /*Wake everyone that is on wait, (p-clock)*/ 

	removeBlocked(p); /*set p to removeBlocked*/

	while(p != NULL){ /* while loop for when p isnt null*/

	insertProc();

	softblock -= 1;

	p = removeBlocked(&semClock); 

}

semClock = 0; //reset semaphore clock

currentproc = NULL;
switch();   /*it returns to the running process here*/



} /***END OF TimerInt****/


void IOHandler(){

/*deviceReg = RAMBASEADDR;*/    /*establish addressing here*/

bitMap = deviceReg -> dev_interrupt[(lineInt - DISKINT)]; /* this is second part of addressing*/

/* While loop to determine the lowest number bit */

while(bitMap &DEVON != 0 ){

if(DEV0ON <= DEV6ON){ /* if the bits are greater than 0 and equal 6 

	/** HOW DO I DO THIS WHAT DO I SET THIS TO **/

} else 

 deviceNum = 7; 


}

deviceSem = ((lineInt - DISKINT) * DEVPERINT) deviceNum; /*determine semaphore for device */

/* special case for terminal interrupt-- idk if he added this or if we need it frfr? */

if(intLine = TERMINT){

intStat = timerInt(deviceSem);

/**more stuff goes in here **/

} /*end of special case*/

deviceSem[deviceSemNum] += 1; /* have to V the semaphore*/

if(deviceSemNum <= 0){ /*waiting for the IO that is already performed here */

	p = removeBlocked(&(deviceSem[deviceSemNum]));

	if(p != NULL){

		p -> p.s.s.v0 = intStat; /* save status in ACK */

		insertBlocked();

		softblock -=1;

	}

	devStatus [deviceSemNum] = intStat; /* save the status until syscall 8 */

	if(currentproc = NULL){ /**get a new proc **/

		switch();
	}

}


} /*****End of IOHandler*****/