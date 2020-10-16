/*including all necessary files*/
#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"

/********************************************************************************
 * 
 * This file contains all methods and instruction for init. This file is 
 * basically the main(). Here is where we declare all variables, the population
 * of the pass up vector, the initalization of phase 1,begin the timers, and 
 * instaniate single process ready queue and call the scheduler to end control 
 * from the main().  
 *
 ********************************************************************************/

/******************** DECLARE OF GLOBAL VARIABLES ******************************/

/* methods mikey said we needed; put in .h */

extern void uTLB RefillHandler();
extern void test();
extern void GenExceptionHander();

/*I/O state saverarea*/
unsigned int devStat[DEVICECNT + DEVPERINT];

/*Process Count*/
int processcnt;

/*Soft-block count*/
int softblock;

/*ready queue*/
pcb_t *readyQueue;

/*setting current process */
pcb_t *currentproc; 

/*device semaphores */
int devices[DEVICECNT + DEVPERINT + 1];

/*time unit*/
cpu_t startTOD;

/*amt till time slice*/
cpu_t sliceCount; /*do I need this now*/


int main(){


/******************** PASS UP VECTOR *****************************************/
	passup =(passupvector_t *)PASSUPVECTOR;
	passup->tlb_refill_handler = (memaddr) uTLB_refill;
	passup->tlb_refill_stackPTR = KERNALSTACK;
	passup->exception_handler = (memaddr)_genException; 
	passup->exception_stackPTR = KENERALSTACK;

/******************** INITALIZATION OF PHASE 1 *******************************/
	initPCB();
	initASL(); 

/******************** INITALIZATION OF NUCLEUS VARIABLES *********************/
	/*Process Count*/
	int processcnt = 0;

	/*Soft-block count*/
	int softblock = 0;

	/*ready queue*/
	pcb_t *readyqueue = mkemptyProc();

	/*setting current process */
	pcb_t *currentproc = NULL; 

	/*initialize I/O and clock semaphores*/

	semClock = 0;
	for(i = 0; i < DEVICECNT; i++){
		devices[i] = 0;
	}

/******************** LOAD INTERVAL TIMER ************************************/

	LDIT(5000); /*set interval time to 100 milliseconds*/


/******************** INSTANTIATE SINGLE PROCESS *****************************/
/***stuff pandos tells us to do page 18*****

	Do I need this shit? I'm gonna ask Umang

	pcb_t -> newPcb = allocPcb(); /**initiate process 


	newPcb -> readyqueue; /**place its pcb in ready queue**/
		

	/*for loop to initiate a single purpose queue, place its pcb in ready queue 
	and increment process count*

		int i;

		for(int i = 0; i < 49; i++){ 
			/*yes, for loop serve me nothing
		}

	  newPcb -> p_s.s_pc = (&(test));

	  /**when we assign a value to pc we have to assign the same value to the gen pupose register t9 here

	  newPcb -> p_s.s_sp = RAMTOP;	/**enable kernal mode?

	/**set the previous bits*

	  newPcb -> p_s.s KUp = 0; 

	  newPcb -> p.s.s IEp = 1;

	  newPcb -> p_time = 0;
	 
	newPcb -> p_supportStruct = NULL; */

/******************** SCHEDULER **********************************************/
	int i; 
	pcb_t *ram = allocPcb();
	unsigned int RAMTOP; /*something for the ram*/


	/*RAMTOP set up*/
	RAMTOP(topofRAM);
	if(ram != NULL){
		ram->p_s.s_pc = p->p_s.s_t9 = (memaddr)
		ram->p_s.s_status = ALLOFF | IEPON | IMON | TEBITONL;
		ram->p_s.s_sp = topofRAM; /*setting the stack pointer*/
		processcnt += 1;
		insertProcQ(&readyQueue, ram);

		scheduler();
	}

	else{

		PANIC();
	}

	/*idk Mikey said so*/
	return 0;
}/*end of main*/

/******************** GenExceptionHandler() **********************************************/

void GenExceptionHander(){
	/*looks at the cause register (stored by BIOS) and points to which syscall it is*/

	/*variables*/
	state_PTR state;
	int reason;

	/*setting the variables*/
	oldState(state_PTR)BIOSDATAPAGE;
	reason = (oldState-s_cause + GETEXECCOD) >> CAUSESHIFT;

	/*if it is one of these send it to one of these*/
	if(reason == IOINTERRUPTS){
		int TrapH();
	}
	if (reason == TLB){
		tlbTrapH();
	}
	if(reason == SYSEXCEPTION){
		sysTrapH();
	}

	/*anything else*/
	pgmTrapH();

}