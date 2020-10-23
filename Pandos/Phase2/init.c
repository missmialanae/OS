/*including all necessary files*/
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
cpu_t sliceCount; /*do I need this now*/


int main(){
	int i; 
	pcb_t *ram = allocPcb();
	unsigned int RAMTOP; /*something for the ram*/



/******************** PASS UP VECTOR *****************************************/
	passupvector_t *passup =(passupvector_t *)PASSUPVECTOR;
	passup->tlb_refill_handler = (memaddr) uTLB_refill;
	passup->tlb_refill_stackPTR = KERNALSTACK;
	passup->exception_handler = (memaddr)_genException; 
	passup->exception_stackPTR = KENERALSTACK;

/******************** INITALIZATION OF PHASE 1 *******************************/
	initPCB();
	initASL(); 

/******************** INITALIZATION OF NUCLEUS VARIABLES *********************/
	/*Process Count*/
	processcnt = 0;

	/*Soft-block count*/
	softblock = 0;

	/*ready queue*/
	readyQueue = mkemptyProc();

	/*setting current process */
	currentproc = NULL; 

	/*initialize I/O and clock semaphores*/

	semClock = 0;
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
	state_PTR oldState;
	int reason;

	/*setting the variables*/
	oldState = (state_PTR)BIOSDATAPAGE;
	reason = (oldState-s_cause + GETEXECCOD) >> CAUSESHIFT;

	/*if it is one of these send it to one of these*/
	if(reason == IOINTERRUPTS){
		TrapH();
	}
	if (reason == TLB){
		tlbTrapH();
	}
	if(reason == SYSEXCEPTION){
		sysTrap();
	}

	/*anything else*/
	pgmTrapH();

}