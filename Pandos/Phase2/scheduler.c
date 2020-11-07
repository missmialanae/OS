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
extern void intervalSwitch(cpu_t time);



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
	debuggerA(30);
	currentproc = current;
	debuggerA(31);

	/*call loadstate*/
	LDST(&(current->p_s));
	debuggerA(32);

}

void scheduler(){
	/*takes the next process on the ready queue and makes it the current process. Basically this will switch */
	/*the processes for us. So when it is time to switch the process then call scheduler*/

	/*local variables*/
	debuggerA(70);
	int currentStatus;
	debuggerA(71);
	pcb_t *next;
	debuggerA(72);


	/********* if the ready queue is empty ************/

	/*make sure the ready queue is not empty*/
	if(!emptyProcQ(&readyQueue)){
		debuggerA(73);
		/*if the readyQueue is not empty*/
		/*if I found one I need to remove it from the ready queue*/
		pcb_t *next = removeProcQ(&readyQueue);
		debuggerA(74);

		/*start the clocks*/
		intervalSwitch(QUANTUM);
		debuggerA(75);

		/*context switch*/
		contextSwitch(next);
		debuggerA(76);

	}

	if(emptyProcQ(&readyQueue)){
		debuggerA(77);
		/*if the readyQueue is empty*/

		/*if there are no more processes then die*/
		if(processcnt == 0){
			debuggerA(78);
			/*invoke the HALT BIOS and now you are done*/
			HALT();
			debuggerA(79);
		}

		if(processcnt > 0 && softBlock > 0){
			debuggerA(80);
			/*if we have something but it is busy*/

			currentproc = NULL;
			/*set the local timer to be a big number*/
			debuggerA(81);
			setTIMER(MAXINT); /*do we already know this or do we need to define it?*/

			/*need to fix the status now*/
			currentStatus = ALLOFF | IEPON | IMON | TEBITONL;
			debuggerA(82);
			setSTATUS(currentStatus);
			debuggerA(83); 

			/*enter a wait state*/
			WAIT();
		}

		/*anything else just panic*/
		debuggerA(84);
		PANIC();
	}
	debuggerA(88);
	return; 
}

void intervalSwitch(cpu_t time){
	/*basically this should prepare the interval timer for given pcb. You should be able*/
	/*to call this when you need to load a new process, and for clock and i/o interrupts*/

	/*call the STCK macro to read the clock*/
	STCK(startTOD);

	/*set the timer to the time given*/
	setTIMER(time);

	/*do I always need to context switch here?*/
}

/******************* END ******************************************************/