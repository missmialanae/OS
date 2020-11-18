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
/*int deviceNum;/*for the device number*/

int interruptLine;

/*bitmapping*/
/*unsigned int bitMap; */

void trapH(){

	cpu_t stopTOD;
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
	else if(interruptCause & 0x00000400 != 0){
		pseudoInterrupts();
	}
	else if(interruptCause & 0x00000800 != 0){
		IOHandler(DISKINT); 
	}
	else if(interruptCause & 0x00001000 != 0){
		IOHandler(FLASHINT);
	}
	else if(interruptCause & 0x00004000 != 0){
		IOHandler(PRNTINT);
	}
	else if((interruptCause & 0x00008000) != 0){
		IOHandler(TERMINT);
	}
	if(currentproc != NULL){
		currentproc->p_time += (stopTOD - startTOD);

		moveState((state_t *) BIOSDATAPAGE, &(currentproc-> p_s));
		intervalSwitch(sliceCount);

		contextSwitch(currentproc);

	}
	else{

		HALT();
	}

} /*end of TrapH*/

void plt(cpu_t stopTOD){
	/*Handles the procedures to handle CPU when it generates a clock interrupt.*/
	/*Deals with Quantum. */
	if(currentproc != NULL){

		currentproc -> p_time = stopTOD - startTOD; /*change the time to the current process*/

		moveState((state_t *) BIOSDATAPAGE, &(currentproc->p_s)); /*store the processor state using move state*/

		/*do we actually need to invoke the scheduler. Mikey says that and sometimes he be lyin*/

		insertProcQ(&(readyQueue), currentproc); /* call insertProc*/

		currentproc = NULL;

		scheduler(); /*call switch */

	}
	
	if(currentproc == NULL){ /* if there is no current process running call panic*/
		debuggerA(1);
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

	/*begin to remove it*/
	while(remove != NULL){ /* while loop for when p isnt null*/

		/*need to place it on to the readyQueue*/
		insertProcQ(&(readyQueue), remove);

		/*decrease the softBlock count*/
		softBlock -= 1;

		/*remove it*/
		removeBlocked(remove->p_semAdd);

	}

	/*reset the pseudo to 0*/
	devices[DEVICECNT + DEVPERINT] = 0;

	/*need to check and make sure there is a current proc to go back too */
	if(currentproc == NULL){

		/*no current proc you have to wait */
		scheduler();
	}

	/*Yes current proc then return control to the currentproc*/
	contextSwitch(currentproc); 
}

void IOHandler(int num){

	unsigned int bitMap;

	int deviceNum;

	/*this function will handle any devices that are going to interrupt using the constant DEVON to map it to a proper 
	correcsponding device */

	/*variables*/
	debuggerA(50);
	volatile devregarea_t *deviceReg;

	/*need a register to hold status for the special case*/
	volatile devregarea_t *termReg;

	/*sema4 number
	int deviceSem;*/

	/*keeps record of the device status*/
	unsigned int intStat;

	/*keeps hold of the line number*/
	unsigned int intLine = num;

	/*points to pcb*/
	pcb_t *ready;

	/*establish addressing*/
	deviceReg = (devregarea_t *) RAMBASEADDR;  
	bitMap = deviceReg->interrupt_dev[(intLine - DISKINT)]; 

	if(&(bitMap) == NULL){
		debuggerA(2);
		PANIC();
	}

	/*bitmapping*/
	if((bitMap &DEV0ON)!= 0 ){
			deviceNum = 0;
	}

	else if((bitMap &DEV1ON) != 0 ){
		deviceNum = 1;
	}

	else if((bitMap &DEV2ON) != 0 ){
		deviceNum = 2;
	}

	else if((bitMap &DEV3ON) != 0 ){
		deviceNum = 3;
	}

	else if((bitMap &DEV4ON) != 0 ){
		deviceNum = 4;
	}

	else if((bitMap &DEV5ON) != 0 ){
		deviceNum = 5;
	}

	else if((bitMap &DEV6ON) != 0 ){
		deviceNum = 6;
	}

	else if((bitMap &DEV7ON) != 0 ){
		deviceNum = 7;
	}/*finish bitmapping/

	/*determine sem for device*/
	deviceNum += ((intLine - DISKINT) * DEVPERINT); 

	/*case for terminal*/
	if(intLine == TERMINT){
		/*need to first check to see if the terminal is transmit or recieve*/

		/*set up termReg*/
		termReg = (devregarea_t *)RAMBASEADDR;

		/*get status from register*/
		intStat = termReg->devreg[(deviceNum)].t_transm_status;

		/*need to and it to the 0x0F(which is a constant called BITS)*/

		/*if it equals 1 then it is receiving*/
		if((intStat & BITS) == 1){
			/*make a copy of the receive status*/
			intStat = termReg->devreg[(deviceNum)].t_recv_status;
			/*ack the recieve*/
			termReg->devreg[(deviceNum)].t_recv_command = ACK;

			/*increment the device sem4 number by DEVPERINT*/
			(deviceNum)+= DEVPERINT;

		}

		/*If it's 0 then it is transmitting*/
		else{

		/*ack the transmit*/
		termReg->devreg[(deviceNum)].t_transm_command = ACK;

		}

	}/*end of special terminal case*/

	else{
	/*copy the status*/
	intStat = deviceReg->devreg[(deviceNum)].d_status;

	/*ACK it*/
	deviceReg->devreg[(deviceNum)].d_command = ACK;

	}

    /*v the sem*/
	devices[deviceNum] += 1; 


	/*wait for IO */
	if(devices[deviceNum] <= 0){ 

		ready = removeBlocked(&(devices[deviceNum]));

		/*there is a process that needs to be unblocked and given status*/
		if(ready != NULL){

			/*set the status*/
			ready->p_s.s_v0 = intStat;

			/*insert the ready pcb on the readyQueue*/
			insertProcQ(&readyQueue, ready);

			/*decrease the softblock count*/
			softBlock -=1;
		}

    }else{
	 /*save stat till sys8*/
		saveStat[deviceNum] = intStat;
	}

		/*new proc*/
		if(currentproc == NULL){ 

			scheduler();
		}

}/*****End of IOHandler*****/