#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"
#include "../h/init.h"
#include "../h/exception.h"
#include "../h/interrupts.h"

#include "/usr/local/include/umps3/umps/libumps.h"



/********************************************************************************
 * 
 * This file contains all methods and instruction for scheduler. This file 
 * controls the ready queue so that it can guarantee the execution of 
 * instructions. This is a round-robin scheduling algorithm. When it is called it
 * removes a PCB from the head of the queue and stores the pointer to a PCB in 
 * the current process in the ready queue. Loads a milliseconds on the PLT.
 * Performs a load processor state on the processor state stored in PCB of the 
 * Current Process. 
 *
 ********************************************************************************/
/*external fucntions*/

extern void moveState(state_PTR source, state_PTR final);
extern void contextSwitch(pcb_t *currentproc);
extern void scheduler();
extern void intervalSwitch(cpu_t time, pcb_t* proc);



void moveState(state_PTR source, state_PTR final){
	int i; 

	/*search through the states to find the prooper state*/
	for(i=0; i < STATEREGNUM; i++){
		final->s_reg[i] = source->s_reg[i];
	}

	/*once it is found set the source to the final*/
	final->s_cause = source->s_cause;
	final->s_entryHI = source->s_entryHI;
	final->s_status = source->s_status;
	final->s_pc = source->s_pc;

}

void contextSwitch(pcb_t *current){
	/*switches to the process that needed to be in control. Basically it is our loadstate caller.*/

	/*need to make sure the current proc is set*/
	
	currentproc = current;

	/*call loadstate*/
	LDST(&(current->p_s));
	
}

void scheduler(){
	/*takes the next process on the ready queue and makes it the current process. Basically this will switch */
	/*the processes for us. So when it is time to switch the process then call scheduler*/

	/*local variables*/
	unsigned int currentStatus;
	pcb_t *next;

	/********* if the ready queue is empty ************/

	/*make sure the ready queue is not empty*/
	if(readyQueue != NULL){

		/*if I found one I need to remove it from the ready queue*/
		next = removeProcQ(&readyQueue);

		intervalSwitch(QUANTUM, next);
	}

	/*if there are no more processes then die*/
	if(processcnt == 0){
		
		/*invoke the HALT BIOS and now you are done*/
		HALT();
		
	}

	if(processcnt > 0 && softBlock > 0){
		
		/*if we have something but it is busy*/

		currentproc = NULL;
		/*set the local timer to be a big number*/
		
		setTIMER(MAXINT); /*do we already know this or do we need to define it?*/

		/*need to fix the status now*/
		currentStatus = ALLOFF | IECON | IMON | TEBITONL;
		
		setSTATUS(currentStatus);

		/*enter a wait state*/
		WAIT();
	}

	if(processcnt > 0 && softBlock == 0){
		/*anything else just panic*/
		debuggerA(84);
		PANIC();
	}


	return; 
}

void intervalSwitch(cpu_t time, pcb_t* proc){
	/*basically this should prepare the interval timer for given pcb. You should be able*/
	/*to call this when you need to load a new process, and for clock and i/o interrupts*/

	/*call the STCK macro to read the clock*/
	STCK(startTOD);

	/*set the timer to the time given*/
	setTIMER(time);

	contextSwitch(proc);

}

/******************* END ******************************************************/