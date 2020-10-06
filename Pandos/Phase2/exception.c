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
void CREATEPROCESS(state_t *statep, support_t * supportp, int v){

	pcb_t *created; 

	/*allocate a new pcb*/

	/*set the new pcb to be created*/

	/*initiliaze the fields*/

	/*p_s from a1 */

	/*p_supportStruct from a2 */

	/*process queue fields*/

	/*NEED TO FIX THESE LATER*/
	created->p_next = insertProcQ(); 
	created->p_prev = insertProcQ(); 

	/*process tree fields*/
	created->p_child = insertChild(); 

	/*p_semADD is set to NULL*/
	created -> p_semADD == NULL; 

}
/******************** END SYS 1 *************************************************/

/******************** SYS 2 *****************************************************/
void terminateProcess(){

	/*set a0 TO 2*/

}
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
void WaitIO(){

}
/******************** END SYS 5 *************************************/

/******************** SYS 6 *****************************************/
void GetCPU(){

}

/******************** END SYS 6 *************************************/

/******************** SYS 7 *****************************************/
void WaitClock(){

}
/******************** END SYS 7 *************************************/

/******************** SYS 8 *****************************************/
void GETSUPPORTPTR(){

}

/******************** END SYS 8 *************************************/

/******************** SYS 9 *****************************************/

/******************** END SYS 9 *************************************/

int sycallHandler(){
	/*this function will deal with the main calling of the sycall
	it will decided which syscall it will use*/

	/*need to check kernel mode status*/

	/*if( in kernel mode){ do the calls down below}*/

	/*request a SYS1 */
	int retValue = SYSCALL(CREATEPROCESS, state_t *statep, 
		support_t * supportp, 0);

	/*request SYS2*/
	SYSCALL (terminateProcess, 0, 0, 0);

	/*request SYS 3*/
	SYSCALL (PASSEREN, int *semaddr, 0, 0);

	/*request SYS 4*/
	SYSCALL (VERHOGEN, int *semaddr, 0, 0);

	/*request SYS 5*/ 

	/*request SYS 6*/
	cpu_t cpuTime = SYSCALL (GetCPU, 0, 0, 0);

	/*request SYS 7*/
	support_t *sPtr = SYSCALL (WaitClock, 0, 0, 0);

	/*request SYS 8*/
	support_t *sPtr = SYSCALL (GETSUPPORTPTR, 0, 0, 0);

	/*if( in user mode){call program trap}



}
