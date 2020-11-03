/*including all necessary files*/
#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"
#include "../h/scheduler.h"
#include "../h/exception.h"
#include "../h/interrupts.h"

#include "/usr/local/include/umps3/umps/libumps.h"


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

extern void uTLB_RefillHandler();
extern void test();
extern void GenExceptionHander();

/*Process Count*/
int processcnt;

/*Soft-block count*/
int softBlock;

/*ready queue*/
pcb_t *readyQueue;

/*setting current process */
pcb_t *currentproc; 

/*device semaphores */
int devices[DEVICECNT + DEVPERINT + 1];

/*time unit*/
cpu_t startTOD;

/*amt till time slice*/
cpu_t *sliceCount; /*do I need this now*/


int main(){
	int i; 
	pcb_t *ram = allocPcb();
	unsigned int RAMTOP; /*something for the ram*/



/******************** PASS UP VECTOR *****************************************/
	passupvector_t *passup =(passupvector_t *)PASSUPVECTOR;
	passup->tlb_refll_handler = (memaddr) uTLB_RefillHandler; /*why not uTLB_RefullHandler?*/
	passup->tlb_refll_stackPtr = KERNAL;
	passup->execption_handler = (memaddr) GenExceptionHander; 
	passup->exception_stackPtr = KERNAL;

/******************** INITALIZATION OF PHASE 1 *******************************/
	initPcbs();
	initASL(); 

/******************** INITALIZATION OF NUCLEUS VARIABLES *********************/
	/*Process Count*/
	processcnt = 0;

	/*Soft-block count*/
	softBlock = 0;

	/*ready queue*/
	readyQueue = mkEmptyProcQ();

	/*setting current process */
	currentproc = NULL; 

	/*initialize I/O and clock semaphores*/

	cpu_t semClock = 0; /*is this actually a clock or is it an int?*/
	for(i = 0; i < DEVICECNT; i++){
		devices[i] = 0;
	}

/******************** LOAD INTERVAL TIMER ************************************/

	LDIT(5000); /*set interval time to 100 milliseconds*/


/******************** INSTANTIATE SINGLE PROCESS *****************************/


/******************** SCHEDULER **********************************************/

	/*RAMTOP set up*/

	memaddr topOfRAM; 

	RAMTOP(topOfRAM);
	if(ram != NULL){
		ram->p_s->s_t9 = (memaddr) test;
		ram->p_s->s_pc = ram->p_s->s_t9;
		ram->p_s->s_status = ALLOFF | IEPON | IMON | TEBITONL;
		ram->p_s->s_sp = topOfRAM; /*setting the stack pointer*/
		processcnt += 1;
		insertProcQ(&readyQueue, ram);

		/*are we actually calling scheduler or do we need to call contextSwitch?*/
		scheduler();
	}

	else{

		PANIC();
	}

	/*Mikey said so*/
	return 0;

}/*end of main*/

/******************** GenExceptionHandler() **********************************************/

void GenExceptionHander(){
	/*looks at the cause register (stored by BIOS) and points to which syscall it is*/

	/*variables*/
	state_t *state;
	int reason;

	/*setting the variables*/
	state = (state_PTR)BIOSDATAPAGE;
	reason = (state->s_cause); 

	/*if it is one of these send it to one of these*/
	while(reason >= 0 && reason <= 13){
		if(reason == 0){ 
			/*IO interrupts*/
			trapH();
		}

		if(reason >= 1 && reason <= 7){
			tlbTrapH();
		}

		if(reason == 8 ){
			/*syscalls*/
			sysTrapH();
		}

		/*anything else*/
		pgmTrapH();
	}

}