#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"

/********************************************************************************
 * 
 * This file contains all methods and instruction for exception handling. This file 
 * controls the population in the pass up vector with the address of the nucleus 
 * exception handler and the stack page. This contains all of the SYSCALL Info. 
 *
 ********************************************************************************/

/*initializing mutex */
int mutex; 

/******************** SYS 1 *****************************************************/

/******************** END SYS 1 *************************************************/

/******************** SYS 2 *****************************************************/

/******************** END SYS 2 *************************************************/

/******************** SYS 3 *****************************************************/
void passeren(){
	/* sys3; Wait; performs the p operation on sema4s. Requested by calling 
	processes by placing the value 3 in a0, the physical address of the 
	sema4 to be P'ed in a1, and then executing the SYSCALL instructions */

	mutex --;
	if (mutex < 0){
		insertBlock(&mutex; currentproc);
		scheduler();

		LDST(return /*control to currentproc*/);
	}
}
/******************** END SYS 3 *************************************************/

/******************** SYS 4 *****************************************************/
void verhogen(){
	/*sys4; signal; service is requested by calling process by placing
	the value 4 in a0, the physical address of the sema4 in a1, and then
	ececuting the SYSCALL instructions */

	mutex ++;
	if (mutex <= 0){
		temp = removeBlocked(&mutex);
		insertProcQ(&readyQueue, temp);
	}
	LDST(return /*control to currentproc*/);
}
/******************** END SYS 4 *************************************************/

/******************** SYS 5 *****************************************/

/******************** END SYS 5 *************************************/


/******************** SYS 6 *****************************************/

/******************** END SYS 6 *************************************/


/******************** SYS 7 *****************************************/

/******************** END SYS 7 *************************************/


/******************** SYS 8 *****************************************/

/******************** END SYS 8 *************************************/