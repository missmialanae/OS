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
int deviceNum;/*for the device number*/

int interruptLine;

cpu_t stopTOD; 

/*bitmapping*/
unsigned int bitMap; 

void trapH(){

	cpu_t interruptCause; /*cause_t?*/


	STCK(stopTOD);
	sliceCount = getTIMER();

	/*put biosdatapage into state*/
	(state_t *) BIOSDATAPAGE; 
    interruptCause = ((state_t *)BIOSDATAPAGE) -> s_cause;

	
	/*determining cause of interrupt*/
	if((interruptCause & 0x00000200) != 0){
		plt(stopTOD);
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
	if((interruptCause & 0x00008000) != 0){
		IOHandler(TERMINT);
	}

	/*return turn to  current running proc*/

	/*make sure it is not NULL*/
	if(currentproc != NULL){
		/*assign the time to the currentproc*/
		currentproc->p_time += (stopTOD - startTOD);

		moveState((state_t *)BIOSDATAPAGE, &(currentproc->p_s));
		intervalSwitch(sliceCount);

		contextSwitch(currentproc);
	}

	/*if there is none then we Halt*/
	HALT();

} /*end of TrapH*/

void plt(cpu_t stopTOD){
	/*Handles the procedures to handle CPU when it generates a clock interrupt.*/
	/*Deals with the ending of Quantum. */
	if(currentproc != NULL){

		currentproc -> p_time = stopTOD - startTOD; /*change the time to the current process*/

		moveState((state_t *)BIOSDATAPAGE, &(currentproc->p_s)); /*store the processor state using move state*/

		insertProcQ(&(readyQueue), currentproc); /* call insertProc*/
		currentproc = NULL;

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

	LDIT(PSEUDO);

	/*need to unblock all pcbs*/
	remove = removeBlocked(&(devices[DEVICECNT+DEVICECNT]));

	/*begin to remove it*/
	while(remove != NULL){ /* while loop for when p isnt null*/

		/*need to place it on to the readyQueue*/
		insertProcQ(&(readyQueue), remove);

		/*decrease the softBlock count*/
		softBlock -= 1;

		/*remove it*/
		remove = removeBlocked(&(devices[DEVICECNT+DEVICECNT]));

	}

	/*reset the pseudo to 0*/
	devices[DEVICECNT+DEVICECNT] = 0;

	/*need to check and make sure there is a current proc to go back too */
	if(currentproc == NULL){

		scheduler();
	}

	/*Yes current proc then return control to the currentproc*/
	contextSwitch(currentproc); 
}

void IOHandler(int num){
	/*this function will handle any devices that are going to interrupt using the constant DEVON to map it to a proper 
	correcsponding device */

	/*variables*/
	volatile devregarea_t *deviceReg;
	volatile devregarea_t *termReg; /*need a register to hold status for the special case*/
	int deviceSem;
	unsigned int intStat; /*keeps record of the device status*/
	/*num keeps track of the line number*/
	debuggerA(num);
	/*unsigned int intLine; keeps hold of the line number*/
	pcb_t *ready; /*points to pcb*/

	/*establish addressing*/
	deviceReg = (devregarea_t *) RAMBASEADDR;
	
	bitMap = deviceReg->interrupt_dev[(num - DISKINT)];
	
	/*bitmapping*/
	if(&(bitMap) == NULL){
		debuggerA(4);
		PANIC();
		debuggerA(5);
	} 

	if((bitMap & DEV0ON) != 0){
		debuggerA(6);
		deviceNum = 0;
		debuggerA(7);
	}

	else if((bitMap & DEV1ON) != 0){
		debuggerA(8);
		deviceNum = 1;
		debuggerA(9);
	}

	else if((bitMap & DEV2ON) != 0){
		debuggerA(10);
		deviceNum = 2;
		debuggerA(11);
	}

	else if((bitMap & DEV3ON) != 0){
		debuggerA(12);
		deviceNum = 3;
		debuggerA(13);
	}

	else if((bitMap & DEV4ON) != 0){
		debuggerA(14);
		deviceNum = 4;
		debuggerA(15);
	}

	else if((bitMap & DEV5ON) != 0){
		debuggerA(16);
		deviceNum = 5;
		debuggerA(17);
	}

	else if((bitMap & DEV6ON) != 0){
		debuggerA(18);
		deviceNum = 6;
		debuggerA(19);
	}

	else if((bitMap & DEV7ON) != 0){
		debuggerA(20);
		deviceNum = 7;
		debuggerA(21);
	}
	/*finish bitmapping*/

	/*determine sem for device*/
	deviceNum += ((num - DISKINT) * DEVPERINT);
	debuggerA(22);

	/*case for terminal*/
	if(num == TERMINT){
		debuggerA(23);
		/*need to first check to see if the terminal is transmit or recieve*/

		termReg = (devregarea_t *)RAMBASEADDR; /*set up termReg*/
		debuggerA(24);
		intStat = termReg->devreg[(deviceNum)].t_transm_status; /*get status from register*/
		debuggerA(25);
		/*need to and it to the 0x0F(which is a constant called BITS)*/

		/*if it is 1 then it is recieving*/
		if((intStat & BITS) == 1){
			debuggerA(26);
			intStat = termReg->devreg[(deviceNum)].t_recv_status; /*making a copy of the receive status*/
			debuggerA(27);
			termReg->devreg[(deviceNum)].t_recv_command = ACK; /*ack the recieve*/
			debuggerA(28);
			(deviceSem)+=DEVPERINT; /*increment the device sem4 number by DEVPERINT*/
			debuggerA(29);
		}

		debuggerA(30);
		termReg->devreg[(deviceNum)].t_transm_command = ACK;
		debuggerA(31);
	}

	else{
		debuggerA(32);
		intStat = deviceReg->devreg[(deviceNum)].d_status;
		debuggerA(33);
		deviceReg->devreg[(deviceNum)].d_command = ACK;
		debuggerA(34);
	}
	/*v the sem*/
	devices[deviceNum] += DEVPERINT;
	debuggerA(35);

	/*wait for IO*/
	if(devices[deviceNum] <= 0){
		debuggerA(36);
		ready = removeBlocked(&(devices[deviceNum]));
		debuggerA(37);

		/*there is a process that needs to be unblocked and given status*/
		if(ready != NULL){
			debuggerA(38);
			ready->p_s.s_v0 = intStat; /*set the status*/
			debuggerA(39);

			/*insert the ready pcb on the readyQueue*/
			insertProcQ(&readyQueue, ready);
			debuggerA(40);

			/*decrease the softblock count*/
			softBlock -=1;
			debuggerA(41);
		}
	}

	else{
		debuggerA(42);
		/*save stat till sys8*/
		devices[deviceNum] = intStat;

	}

	/*new proc*/
	debuggerA(43);
	if(currentproc == NULL){
		debuggerA(44);
		scheduler();
	}
	
} /*end of IO*/
