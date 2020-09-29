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
pcb_t readyQueue;

/*setting current process */
pcb_t currentproc; 

/*device semaphores */
int devices[i]; 

int main(){

/******************** END OF DECLARE OF GLOBAL VARIABLES *********************/

/******************** PASS UP VECTOR *****************************************/

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
	pcb_t readyqueue = mkemptyProc();

	/*setting current process */
	pcb_t currentproc = NULL; 

	/*device semaphores */
	int devices[i] = 0; 

/******************** END OF DECLARE OF NUCLEUS VARIABLES ********************/

/******************** LOAD INTERVAL TIMER ************************************/

/******************** END OF DECLARE OF INTERVAL TIMER ***********************/

/******************** INSTANTIATE SINGLE PROCESS *****************************/

/******************** END OF INSTANTIATE SINGLE PROCESS **********************/

/******************** SCHEDULER **********************************************/
	scheduler();
/******************** END OF SCHEDULER ***************************************/

/******************** GenExceptionHandler() **********************************************/
/*looks at the cause register (stored by BIOS) and points to which syscall it is*/

/******************** END OF GenExceptionHandler() ***************************************/

}
/******************** END OF INIT.C ******************************************/