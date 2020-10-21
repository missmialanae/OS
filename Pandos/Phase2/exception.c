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

/*external functions*/
extern void pgmTrapH();
extern void tlbTrapH();
extern void sysTrapH();
extern void blockCurrent();
extern void removeProcess();
HIDDEN void passUpOrDie();

/*global variable*/
pcb_t *currentproc = NULL;

void pgmTrapH(){
	
	passUpOrDie(GENERALEXCEPT);
}

void tlbTrapH(){
	
	passUpOrDie(PGFAULTEXCEPT);
}

void sysTrap(){

	int sys; /* current sys */
	state_PTR state = (state_PTR)BIOSDATAPAGE;
	cpu_t currentTime;

	/*set the sys that we are getting and set it to */
	sys = state->s_a0;

	/*in user mode*/
	if((sys > = 1) && (sys <= 8) && (user != 0)){
		/*call the program traph*/
		pgmTrapH(); 
	}

	/*not in user mode*/
	moveState(state, &(currentproc->p_s)); /*storing the prco state*/

	/*need to prevent the loop*/

	currentproc->p_s.s_pc = currentproc->p_s.s_pc + 4;

	/*call switch to change the state depending on the value*/
	switch(sys){

		/*create a process -- sys 1*/
		case CREATEPROCESS: {
			createProcess();
		}

		/*terminate the process -- sys 2*/
		case TERMINATEPROCESS:{
			/*end the process*/
			removeProcess(currentproc);

			/*call the scheduler*/
			scheduler();
		}

		/* P -- sys 3*/
		case PASSEREN:{
			passeren();
		}

		/*V -- sys 4*/
		case VERHOGEN:{
			verhogen();
		}

		/*IO wait -- sys 5*/
		case WAITIO:{
			waitIO();
		}

		/*getTime -- sys 6*/
		case GETTIME:{

			/*variables*/
			cpu_t resulTime;

			STCK(currentTime);
			resulTime = ((currentTime) - startTOD) + currentproc->p_time;

			currentproc->p_s.s_v0 = resulTime;

			/*need to return control at the end */

			contextSwitch(currentproc);
		}

		/*Clock Wait  -- sys 7*/
		case CLOCKWAIT:{
			waitClock();
		}

		/*get support PTR -- sys 8*/
		case GETSUPPORTPTR:{

			/*idk what happens here tbh*/

			supportPtr();
		}

		default :{
			passUpOrDie(GENERALEXCEPT);
		}
		/*NO MORE CASES*/
	}
}

/******************** SYS 1 *****************************************************/
void createProcess(){

	/*sys 1*/

	/*allocate a new pcb*/
	pcb_t *created = allocPCB();

	/*need a support structureand set it to the currentproc's a2*/
	support_t *support = (support_t *)currentproc->p_s.s_a2;

	if(created == NULL){
		/*need to switch the context*/
	}
	/*if the p is not null

	/*increase the processcnt*/
	processcnt += 1;

	/*p_s from a1 */
	moveState((state_Ptr)currentproc->p_s.sa2, &(created->p_s));

	/*p_supportStruct is set to NULL*/
	created->p_supportStruct = NULL;

	/*need to check the support */

	if((support != NULL)|| (support != 0)){
		created->p_supportStruct = support; /*setting created's support structure to be support*/
	}
	if((support == NULL)|| (support == 0)){
		insertProcQ(&(readyQueue), created);
		insertChild(&(currentproc), created);
		currentproc->p_s.s_v0 = 1;
	}

	/*time*/
	created->p_time = 0; 

	/*p_semADD is set to NULL*/
	created -> p_semADD == NULL; 

	/*need to switch Context*/
	contextSwitch(currentproc);

}

/******************** SYS 3 *****************************************************/
void passeren(){
	/* sys3; Wait; performs the p operation on sema4s. Requested by calling 
	processes by placing the value 3 in a0, the physical address of the 
	sema4 to be P'ed in a1, and then executing the SYSCALL instructions */

	/*pointer to a sem4 value* and set it to the currentproc's a1 */
	int *sem = currentproc->p_s.s_a1;
	*sem -=1;

	/*need to block and invoke scheduler*/
	if(*sem < 0){

		blockCurrent(&sem; currentproc);

		scheduler();
	}
	/*now another context switch*/
	contextSwitch(currentproc);
}

/******************** SYS 4 *****************************************************/
void verhogen(){
	/*sys4; signal; service is requested by calling process by placing
	the value 4 in a0, the physical address of the sema4 in a1, and then
	ececuting the SYSCALL instructions */

	/*need a pcb pointer*/
	pcb_t *temp; 

	/*thing as passeren()*/
	int *sem = currentproc->p_s.s_a1;
	*sem +=1;

	if (sem <= 0){
		temp = removeBlocked(*sem);

		/*need to check and make sure temp is not null*/

		if (temp != NULL){

			insertProcQ(&readyQueue, temp);
		}
	}

	/*now another context switch*/
	contextSwitch(currentproc);
}

/******************** SYS 5 *****************************************/
void waitIO(){

	/*variables*/
	int deviceNum;
	int interruptLine;

	/*match the interrupt numbers with the device*/

	/*need to set interrupts to a1*/
	interruptLine = currentproc->p_s.s_a1;

	/*need to set device number to a2*/
	deviceNum = currentproc->p_s.s_a2;

	/*convert devices to sema4 */
	deviceNum += ((interruptLine - DISKINT) * DEVPERINT);

	/*terminal reading*/

	if((interruptLine = TERMINT) && (currentproc->p_s.s_a3)){
		deviceNum +=DEVPERINT;
	}

	/*taking one off the device list */
	devices[deviceNum]-=1;

	/*now if it doesn't occur*/

	/*need to block and schedule*/
	if(devices[deviceNum] < 0){
		softBlock +=1;
		blockCurrent(&(devices[deviceNum]));
	}else{
		currentproc->p_s.s_v0 = devStat[deviceNum];

		/*another switch*/
		contextSwitch(currentproc);
	}

}

/******************** SYS 7 *****************************************/
void waitClock(){

	/*take one off of the clock sem4*/
	clockSem -=1; 

	/*need to block current process*/
	if(clockSem < 0){
		/*now block it*/
		blockCurrent(&(clockSem));
	}

	/*finally context switch*/
	contextSwitch(currentproc);

}

/******************** SYS 8 *****************************************/
void supportPtr(){

	/*variables*/
	support_t *support;

	/*setting support*/
	support = currentproc->p_supportStruct;

	/*setting currentproc's v0 to be support*/
	currentproc->p_s.s_v0 = support;

	/*now switch the context*/
	contextSwitch(currentproc);
}

HIDDEN void passUpOrDie(int except){

	/*idk what is happening here*/ 

	/*check to make sure the support is not null*/
	if(currentproc->p_supportStruct != NULL){
		/*if it is not null kill it*/
		moveState((state_PTR)BIOSDATAPAGE, &(currentproc->p_supportStruct->sup_except[except]));
		LDCXT(currentproc->p_supportStruct->sup_exceptContext[except].c_stackPtr), 
		currentproc->p_supportStruct->sup_exceptContext[except].c_status,
		currentproc->p_supportStruct->sup_except[except].c_pc;
	}

	/*if it is null send it to the passup vector*/

	/*get rid of the currentproc*/
	removeProcess(currentproc);

	/*need to switch the process*/
	procSwitch();
}


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

	/*if( in user mode){call program trap}*/
}

void blockCurrent(int *blockSem){
	/*places the current process that is being called into the queue. Afterwards it calls
	to switch over the process*/

	/*variables*/

	cpu_t stopTOD; /*what the hell is this*/

	STCK(stopTOD);

	/*set the currentproc's time to the new stopTOD */
	currentproc->p_time += ((stopTOD)- startTOD);
	insertBlock(blockSem, currentproc);

	/*set the current to NULL*/
	currentproc = NULL;

	/*go to the next process*/
	procSwitch();
}

void removeProcess(pcb_t *proc){
	/*this helps with sys 2 whitch removes a process. The function
	recursively removes the process downwards and then frees the pcb
	and decreases the processcnt*/

	/* recursively removes the process down

	/*variables*/
	/*pcb_t *temp;*/
	int *sem; /*ptr to a semadd value*/

	/*first you need to make sure it doesn't have a child*/
	while(!emptyChild(proc)){
		removeProcess(removeChild(proc));
	}

	/*if you are the current proc*/

	if(proc == currentproc){
		outChild(proc);
	}

	/*if the process is on the ready queue*/
	if(proc->p_semADD == NULL){
		outProcQ(&(readyQueue), proc);
	}


	/*set the pcb free*/
	freePcb(proc);

	/*fix the process count*/
	processcnt -=1;
}