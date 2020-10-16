#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"

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

extern void moveState();
extern void switch();
extern void prep();
extern void finalmsg(); /*do we need this*/

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
/*first want to check if the readyqueue is empty*/
void switch(){

	pcb_t *p;
	pcb_t *currentproc;

	if(p != null){
		/*call prep*/
		prep();

	}

	if(emptyProcQ(&readyqueue)){
		if (processcnt = 0){
			/*invoke the HALT BIOS and you are done*/
			void HALT();
		}

		if(processcnt > 0 && softblock > 0){
			/*enter a wait state which is supported */

			currentproc == NULL;

			/*do I need to set and store the status*/ 

			/*need to load cpu timer with larger number to stop it */

			void WAIT(); 

		}

		if(processcnt > 0 && softblock = 0){
			/*deadlock*/

			/*dedlock detected*/

			/*invoke PANIC BIOS*/
			void PANIC(); 
		}
	}
}

/* if the ready queue is not ready */

/*remove the pcb from the head of the Ready queue*/
removePCB(&readyqueue); 

/*store the pointer to current process*/
pcb_t currentproc = head of the ready queue;

/*load 5 miliseconds to the PLT */



/*perform LDST on the processor state stored in the pcb of the current process*/
LDST(currentproc->s_status); 

/******************* END ******************************************************/