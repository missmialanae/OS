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


/*Process Count*/
int processcnt;

/*Soft-block count*/
int softblock;

/*ready queue*/
pcb_t *readyQueue;

/*setting current process */
pcb_t *currentproc; 

/*device semaphores */
int devices[i]; 

/* methods mikey said we needed */

extern void uTLB RefillHandler();
extern void test();

int main(){

/******************** END OF DECLARE OF GLOBAL VARIABLES *********************/

/******************** PASS UP VECTOR *****************************************/
	void passUpVector(){
	                     /*populate processor 0 pass up vector*/
	                     /*processor 0 is located at 0x0FFF.F900*/

	}


/******************** END OF PASS UP VECTOR **********************************/

/******************** INITALIZATION OF PHASE 1 *******************************/
	initPCB();
	initASL(); 

/******************** END OF INITALIZATION OF PHASE 1 ***********************/

/******************** INITALIZATION OF NUCLEUS VARIABLES *********************/
	/*Process Count*/
	int processcnt = 0;

	/*Soft-block count*/
	int softblock = 0;

	/*ready queue*/
	pcb_t *readyqueue = mkemptyProc();

	/*setting current process */
	pcb_t *currentproc = NULL; 

	/*device semaphores */
	int devices[i] = 0; 

/******************** END OF DECLARE OF NUCLEUS VARIABLES ********************/

/******************** LOAD INTERVAL TIMER ************************************/

	intervalTimer = 100; /*set interval time to 100 milliseconds*/

/******************** END OF DECLARE OF INTERVAL TIMER ***********************/

/******************** INSTANTIATE SINGLE PROCESS *****************************/
/***stuff pandos tells us to do page 18*****/

	pcb_t -> newPcb = allocPcb(); /**initiate process**/


	newPcb -> readyqueue; /**place its pcb in ready queue**/
		

	/*for loop to initiate a single purpose queue, place its pcb in ready queue 
	and increment process count**/

		int i;

		for(int i = 0; i < 49; i++){ 
			/*yes, for loop serve me nothing*/
		}

	  newPcb -> p_s.s_pc = (&(test));

	  /**when we assign a value to pc we have to assign the same value to the gen pupose register t9 here**/

	  newPcb -> p_s.s_sp = RAMTOP;	/**enable kernal mode?**/

	/**set the previous bits**/

	  newPcb -> p_s.s KUp = 0; 

	  newPcb -> p.s.s IEp = 1;

	  newPcb -> p_time = 0;
	 
/******************** END OF INSTANTIATE SINGLE PROCESS **********************/
	newPcb -> p_supportStruct = NULL;

/******************** SCHEDULER **********************************************/
	scheduler();
/******************** END OF SCHEDULER ***************************************/

/******************** GenExceptionHandler() **********************************************/
/*looks at the cause register (stored by BIOS) and points to which syscall it is*/
	void GenExceptionHandler(){

	}

/******************** END OF GenExceptionHandler() ***************************************/


/******************** END OF INIT.C ******************************************/
}