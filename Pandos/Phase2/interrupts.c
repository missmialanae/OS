#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"
#include "../h/init.h"
#include "../h/scheduler.h"
#include "../h/exception.h"
#include "/usr/local/include/umps3/umps/libumps.h"



/********************************************************************************
 * 
 * This file contains all methods and instruction for interrupts. This file 
 * controls context switches and deals with the mapping of all the devices
 * through. Deals with Non-timer interrupts, PLT interrupts, the system wide 
 * interval timer and pseudoclock. 
 * 
 *
 /********************************************************************************/

/*externs*/
HIDDEN void traphH();
HIDDEN void plt(cpu_t stopTOD);
HIDDEN void pseudoInterrupts();
HIDDEN void IOHandler(int num);


/*add these just in case*/
int deviceNum;

int interruptLine;

cpu_t semClock; 

/*bitmapping*/
int bitMap; 

void trapH(){

	cpu_t time;
	cpu_t interruptCause; /*cause_t?*/
	STCK(time);
	sliceCount = getTIMER();

	/*put biosdatapage into state*/
	(state_t *) BIOSDATAPAGE; 
    interruptCause = ((state_t *)BIOSDATAPAGE) -> s_cause;

	
	/*determining cause of interrupt*/
	if(interruptCause & 0x00000200 != 0){
		plt(time);
	}
	if(interruptCause & 0x00000400 != 0){
		pseudoInterrupts();
	}
	if(interruptCause & 0x00000800 != 0){
		IOHandler(DISKINT); 
	}
	if(interruptCause & 0x00001000 != 0){
		IOHandler(FLASHINT);
	}
	if(interruptCause & 0x00004000 != 0){
		IOHandler(PRNTINT);
	}
	if(interruptCause & 0x00008000 != 0){
		IOHandler(TERMINT);
	}

} /*end of TrapH*/

void plt(cpu_t stopTOD){
	/*Handles the procedures to handle CPU when it generates a clock interrupt.*/
	/*Deals with Quantum. */
	if(currentproc != NULL){

		currentproc -> p_time = stopTOD - startTOD; /*change the time to the current process*/

		moveState(BIOSDATAPAGE, &(currentproc->p_s)); /*store the processor state using move state*/

		scheduler(); /*when its the end of processor's time (sliceCount), invoke the schedular*/

		/*do we actually need to invoke the scheduler. Mikey says that and sometimes he be lyin*/

		insertProcQ(&(readyQueue), currentproc); /* call insertProc*/

		scheduler(); /*call switch */

	}
	
	if(currentproc = NULL){ /* if there is no current process running call panic*/
		
		PANIC();
	}
}

void pseudoInterrupts(){
	/*This function handles the pseudo clock tick. This performs a V operation every 100 milliseconds on *
	 * nucleus maintained sema4 list called the pseueoclockTick. */

	/*variables*/
	pcb_t *remove;

	/* ACK interrupt using LDIT; this is the pseudoclock tick */

	LDIT(100000);

	/*need to unblock all pcbs*/

	/*begin to remove it*/
	while(remove != NULL){ /* while loop for when p isnt null*/

		/*need to place it on to the readyQueue*/
		insertProcQ(&(readyQueue), remove);

		/*remove it*/
		removeBlocked(remove->p_semAdd);

		/*decrease the softBlock count*/
		softBlock -= 1;
	}

	/*reset the pseudo to 0*/
	semClock = 0;

	/*need to check and make sure there is a current proc to go back too */
	if(currentproc == NULL){

		/*no current proc you have to wait */
		WAIT();
	}

	/*Yes current proc then return control to the currentproc*/
	contextSwitch(currentproc); 
}

void IOHandler(int num){

	/*variables*/

	devregarea_t *deviceReg;

	/*number for interrupt*/
	int deviceNum;

	/*sema4 number*/
	int deviceSem;

	int intStat;
	int intLine;

	/*points to pcb*/
	pcb_t *ready;

	/*what is this for*/
	int interrupt_dev;

	/*establish addressing*/
	deviceReg = (devregarea_t *) RAMBASEADDR;  
	bitMap = deviceReg->interrupt_dev[(intLine - DISKINT)]; 


	/*bitmapping*/
	if(bitMap &DEV0ON != 0 ){
			deviceNum = 0;
		
		if(bitMap &DEV1ON != 0 ){
			deviceNum = 1;
		}
		if(bitMap &DEV2ON != 0 ){
			deviceNum = 2;
		}
		if(bitMap &DEV3ON != 0 ){
			deviceNum = 3;
		}
		if(bitMap &DEV4ON != 0 ){
			deviceNum = 4;
		}
		if(bitMap &DEV5ON != 0 ){
			deviceNum = 5;
		}
		if(bitMap &DEV6ON != 0 ){
			deviceNum = 6;
		}
		if(bitMap &DEV7ON != 0 ){
			deviceNum = 7;
		}
		
	}
		/*determine sem for device*/
	deviceSem= ((intLine - DISKINT) * DEVPERINT) + deviceNum;

		/*case for terminal*/
	if(intLine = TERMINT){

		/*intStat = plt(deviceSem); /*what is timerIntH*/
	     /*intStat = timerInt(deviceSem);*/
		
		
	}else{
		
		/*intStat = (deviceReg->devreg[deviceSem]).d_status;
		intStat = (deviceReg->devreg[deviceSem]).d_command = ACK;
		*/
		}/*end of special case*/


	    /*v the sem*/
		devices[deviceSem] += 1; 

		/*wait for IO already done*/
		if(deviceSem <= 0){ 

			ready = removeBlocked(&(devices[deviceSem]));
		}
		if(ready != NULL){

			
			ready = intStat;

			insertProcQ(&readyQueue, ready);

			softBlock -=1;
	        /*save stat till sys8*/
			intStat = devices[deviceNum]; 

			/*new proc*/
			if(currentproc == NULL){ 

				scheduler();
			}
		}

}


/*****End of IOHandler*****/