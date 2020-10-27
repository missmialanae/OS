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


void moveState(state_PTR source, state_PTR final){
	int i; 

	/*search through the states to find the prooper state*/
	for(i=0; i < STATEREGNUM; i++){
		final->s_reg[i] = source->s_reg[i]
	}

	/*once it is found set the source to the final*/
	source->s_cause = final->s_cause;
	source->entryHI = final->s_entryHI;
	source->s_status = final->s_status;
	source->s_pc = source ->s_pc;

}

void contextSwitch(pcb_t *currentproc){

	/*switches to the process that needed to be in control*/

	/*need to make sure the current proc is set*/
	pcb_t *currentproc = currentproc;

	/*call loadstate*/
	LDST(&currentproc->p_s);

}

void scheduler(){
	/*takes the next process on the ready queue and makes it the current process*/

	/*if I found one I need to remove it from the ready queue*/
	pcb_t *removed = removeProcQ(&readyQueue);

	/*if I find one I need to switch to it*/
	if(removed != NULL){
		/*just like in exceptions --- is this LDST*/

		STCK(startTOD);
		/*give it time to put on the clock*/

		/*switch context*/
		contextSwitch(currentproc);
	}

	/********* if the ready queue is empty ************/

	/*make sure the ready queue is empty*/
	if(emptyProcQ(&readyQueue)){

		/*if there are no more processes then die*/
		if(processcnt == 0){
			/*invoke the HALT BIOS and now you are done*/
			HALT();
		}

		if(processcnt < 0 && softBlock > 0){

			/*There is some other things that idk how to do*/


			/*set the local timer to be a big number*/

			/*enter a wait state*/
			WAIT();
		}
		/*anything else just panic*/
		PANIC();
	}

	PANIC();
}


/******************* END ******************************************************/