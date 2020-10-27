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


/* the stupid global variable so it will stop giving me errors*/

/*Process Count*/
int processcnt;

/*Soft-block count*/
int softBlock;

/*ready queue*/
pcb_t *readyQueue;

/*setting current process */
pcb_t *currentproc; 

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
	int lineNum = num; /*setting the number it recieves to be the line number*/

	/*Establish addressing portion */

	/* setting up the device register */
	votatile devregarea_t *deviceReg = (devregarea_t *) RAMBASEADDR;    /*establish addressing here*/

	/*setting up the bit map to locate the proper device with the proper line number*/
	bitMap = deviceReg -> interrupt_dev[(lineNum - DISKINT)]; /* this is second part of addressing*/

	/* While loop to determine the lowest number bit */
	while(bitMap != 0){

		if(DEV0ON != 0 ){
			deviceNum = 0;
		}

		if(DEV1ON != 0){
			deviceNum = 1;
		}

		if(DEV2ON != 0){
			deviceNum = 2;
		}

		if(DEV3ON != 0){
			deviceNum = 3;
		}

		if(DEV4ON != 0){
			deviceNum = 4;
		}

		if(DEV5ON != 0){
			deviceNum = 5;
		}

		if(DEV6ON != 0){
			deviceNum = 6;
		}

		deviceNum = 7;

	} /*end of while loop*/

	/*have to determine the sema4 for this device*/
	int sema = ((lineNum - DISKINT) * DEVPERINT) * deviceNum; 

	/* handles the special case for terminal interrupt*/
	if(line = TERMINT){

		intStat 
			
		/**more stuff goes in here **/

	} /*end of special case*/

	/* have to V the semaphore*/
	devices[sem] += 1; 

	/*sys 5 to perfrom*/

	if(deviceSemNum <= 0){ /*waiting for the IO that is already performed here */

		p = removeBlocked(&(deviceSem[deviceSemNum])); 

		if(p != NULL){

			p -> p.s.s.v0 = intStat; /* save status in ACK */

			insertBlocked(&(readyQueue), p);

			softblock -=1;

		}

		devStatus [deviceSemNum] = intStat; /* save the status until syscall 8 */

		if(currentproc = NULL){ /**get a new proc **/

			scheduler();
		}

	}


} /*****End of IOHandler*****/