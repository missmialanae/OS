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
extern int debuggerA();

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

/*initialize I/O and clock semaphores*/
cpu_t semClock;

int main(){
	int i; /*for device for loop*/
	pcb_t *current; /*name of first process*/
	memaddr topOfRAM; /*used for RAMTOP*/
	passupvector_t *passup; /*passup vector*/

/******************** PASS UP VECTOR *****************************************/
	passup =(passupvector_t *)PASSUPVECTOR;
	passup->tlb_refll_handler = (memaddr) uTLB_RefillHandler; /*why not uTLB_RefullHandler?*/
	passup->tlb_refll_stackPtr = KERNAL;
	passup->execption_handler = (memaddr) GenExceptionHander; 
	passup->exception_stackPtr = KERNAL;

/******************** INITALIZATION OF PHASE 1 *******************************/
	debuggerA(1);
	initPcbs();
	debuggerA(2);
	initASL();
	debuggerA(3);

/******************** INITALIZATION OF NUCLEUS VARIABLES *********************/
	/*Process Count*/
	processcnt = 0;
	debuggerA(4);
	/*Soft-block count*/
	softBlock = 0;
	debuggerA(5);
	/*ready queue*/
	readyQueue = mkEmptyProcQ();
	debuggerA(6);

	/*setting current process */
	currentproc = NULL; 
	debuggerA(7);

	/*initialize I/O and clock semaphores*/
	cpu_t semClock = 0; 
	debuggerA(8);

	/*creating the semd devices*/

	devices[DEVICECNT+DEVICECNT] = 0; /*use for semClock*/
	debuggerA(9);

	for(i = 0; i < (DEVICECNT + DEVPERINT); i++){
		debuggerA(10);
		devices[i] = 0;
		debuggerA(11);
	}

	LDIT(PSEUDO); /*set interval time to 100 milliseconds*/
	debuggerA(12);

/******************** SCHEDULER **********************************************/

	/*creating the first process*/
	current = allocPcb();
	debuggerA(13);
	/*using C macro to set up the location of topOfRAM*/
	RAMTOP(topOfRAM);
	debuggerA(14);
	if(current != NULL){
		/*start the test*/
		debuggerA(15);
		current->p_s.s_t9 = (memaddr) test;
		debuggerA(16);
		current->p_s.s_pc = (memaddr) test;
		debuggerA(17);
		current->p_s.s_status = ALLOFF | IEPON | IMON | TEBITONL; /* turning bits on*/
		debuggerA(18);
		current->p_s.s_sp = topOfRAM; /*setting the stack pointer*/
		debuggerA(19);
		processcnt += 1; /*need to add this process to the count*/
		debuggerA(20);
		insertProcQ(&(readyQueue), current);
		debuggerA(21);
		scheduler();
		debuggerA(22);
	}

	else{
		debuggerA(23);
		PANIC();
	}

	/*Mikey said so*/
	debuggerA(24);
	return 0;

}/*end of main*/

/******************** GenExceptionHandler() **********************************************/

void GenExceptionHander(){
	/*looks at the cause register (stored by BIOS) and points to which syscall it is*/

	/*going to need reason and state which is declared at global so I can use it in the debug statements*/
	/*for genexceptionhandler*/ 
	state_t *state = (state_t *)BIOSDATAPAGE; 

	/*used to find reason for genexceptionhandler*/ 
	int reason = (int) ((state->s_cause & GETCAUSE) >> 2);

	/*if it is one of these send it to one of these*/
	while(reason >= 0 && reason <= 3){
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

	return;
}

/*debug functions*/
int debuggerA(int a){
	a = 0;
}
