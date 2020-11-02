#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"
#include "../h/init.h"
#include "../h/scheduler.h"
#include "../h/exception.h"
#include "../h/interrupts.h"
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
HIDDEN void plt(cpu_t stopTOD);
HIDDEN void pseudoInterrupts();
HIDDEN void IOHandler(int num);
HIDDEN void TraphH();


/* the stupid global variable so it will stop giving me errors*/

/*Process Count*/
int processcnt;

/*Soft-block count*/
int softBlock;

/*ready queue*/
pcb_t *readyQueue;

/*setting current process */
pcb_t *currentproc = NULL; 

/*add these just in case*/
int deviceNum;

int interruptLine;

semd_t *semClock; 

/*device semaphores */
int devices[DEVICECNT + DEVPERINT + 1];

/*time unit*/
cpu_t startTOD;

/*amt till time slice*/
cpu_t *sliceCount; /*do I need this now*/

int bitMap; 

void plt(cpu_t stopTOD){
	/*Handles the procedures to handle CPU when it generates a clock interrupt.*/
	/*Deals with Quantum. */
	if(currentproc != NULL){
		currentproc -> p_time = stopTOD - startTOD; /*change the time to the current process*/

		moveState(BIOSDATAPAGE, currentproc->p_s); /*store the processor state using move state*/

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
	pcb_t * remove

	/* ACK interrupt using LDIT; this is the pseudoclock tick */

	LDIT(PSEUDO);

	/*need to unblock all pcbs*/

	/*begin to remove it*/
	while(remove != NULL){ /* while loop for when p isnt null*/

		/*need to place it on to the readyQueue*/
		insertProcQ(&(readyQueue), remove);

		/*remove it*/
		removeBlocked(remove->s_semAdd);

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
	int deviceReg;
	int deviceSem;
	int deviceNum;
	int deviceSemNum;
	int intStat;
	int intLine;
	pcb_t *readyqueue;
	int devices[DEVICECNT + DEVPERINT + 1];
	int interrupt_dev;

	/*establish addressing*/
	/*deviceReg = devregrea_t -> RAMBASEADDR;  */
	deviceReg = RAMBASEADDR;  
	bitMap = deviceReg &interrupt_dev[(lineNum - DISKINT)]; 


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
	deviceSemNum = ((lineNum - DISKINT) * DEVPERINT) + deviceNum;

		/*case for terminal*/
	if(intLine = TERMINT){

		intStat = timerInt(deviceSemNum);
	     /*intStat = timerInt(deviceSem);*/
		
		
	}else{
		
		intStat = (deviceReg &deviceReg[deviceSemNum]).d_status;
		intStat = (deviceReg &deviceReg[deviceSemNum]).d_command = ACK;
		
		}/*end of special case*/


	    /*v the sem*/
		deviceSem[deviceSemNum] <= 0; 

		/*wait for IO already done*/
		if(deviceSemNum <= 0){ 

			p = removeBlocked(&(deviceSem[deviceSemNum]));
		}
		if(p != NULL){

				/*p -> p.s.s.v0 = intStat;  /*save status in ACK */
			p = intStat;

			insertProcQ(&readyqueue, p);

			softBlock -=1;
	        /*save stat till sys8*/
			intStat = deviceStat[deviceSemNum]; 

			/*new proc*/
			if(currentproc == NULL){ 

				scheduler();
			}
		}

}

void TrapH(){

	cpu_t time;
	cpu_t interruptCause; /*cause_t?*/
	STCK(time);
	sliceCount = getTimer();
	/*put biosdatapage into state*/
	(state_t *) BIOSDATAPAGE; 
    interruptCause = ((state_t *)BIOSDATAPAGE) -> s_cause;

	
	/*determining cause of interrupt*/
	if(interruptCause & 0x00000200 != 0){
		localTimer(time);
	}
	if(interruptCause & 0x00000400 != 0){
		timerInt();
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

/*****End of IOHandler*****/