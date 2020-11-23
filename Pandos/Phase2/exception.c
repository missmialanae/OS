#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"
#include "../h/init.h"
#include "../h/scheduler.h"
#include "../h/interrupts.h"

#include "/usr/local/include/umps3/umps/libumps.h"

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
extern void blockCurrent(int *blockSem);
extern void removeProcess(pcb_t *proc);
HIDDEN void passUpOrDie(int except);
extern int createProcess();
extern void passeren();
extern void verhogen();
extern void waitIO();
extern void waitClock();
extern support_t* supportPtr();


void pgmTrapH(){
	/*When the program triggers a program trap then it is sent to passupordie*/
	
	passUpOrDie(GENERALEXCEPT);
}

void tlbTrapH(){
	/*when the program triggers a TLB mgmt trap then it is sent to passupordie*/
	
	passUpOrDie(PGFAULTEXCEPT);
}

void sysTrapH(){

	int sys; /* current sys */
	state_PTR state = (state_PTR)BIOSDATAPAGE;
	cpu_t currentTime;

	/*set the sys that we are getting and set it to */
	sys = state->s_a0;

	/*in user mode, NOT KERNAL*/
	if((sys >= 1) && (sys <= 8) && ((state-> s_status & USER) == 1)){

         /*change cause privs*/
		(state->s_cause) = (state->s_cause & CLEAR) | (NOPRIV << SHIFTS);

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
			int created;
			created = createProcess();
			currentproc->p_s.s_v0 = created; 
			contextSwitch(currentproc);
		}

		/*terminate the process -- sys 2*/
		case TERMINATEPROCESS:{
			/*end the process*/
			removeProcess(currentproc);

			/*call the scheduler*/
			debuggerA(4);
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
			support_t *supp = supportPtr();
			currentproc->p_s.s_v0 = supp;
			contextSwitch(currentproc);
		}

		default :{
			passUpOrDie(GENERALEXCEPT);
		}
		/*NO MORE CASES*/
	}
}

/******************** SYS 1 *****************************************************/
int createProcess(){

	/*sys 1*/

	/*allocate a new pcb*/
	pcb_t *created;
	state_t *data;
	support_t *support;


	created = allocPcb();

	if(created == NULL){
		/*change v0 to -1 */
		currentproc->p_s.s_v0 = -1;

		return (1);
		
	}
	/*if the p is not null*/

	/*increase the processcnt*/
	processcnt += 1;

	data = (state_t *) currentproc->p_s.s_a1;
	moveState(data, &(created->p_s));
	support = (support_t *)currentproc->p_s.s_a2;

	/*need to check the support */

	if(!((support == NULL)|| (support == 0))){
		created->p_supportStruct = support; /*setting created's support structure to be support*/
	}
	
	insertProcQ(&(readyQueue), created);
	insertChild((currentproc), created);
	currentproc->p_s.s_v0 = 0; /*put 0 into v0*/

	return (0);

}

/******************** SYS 3 *****************************************************/
void passeren(){
	/* sys3; Wait; performs the p operation on sema4s. Requested by calling 
	processes by placing the value 3 in a0, the physical address of the 
	sema4 to be P'ed in a1, and then executing the SYSCALL instructions */

	/*pointer to a sem4 value* and set it to the currentproc's a1 */
	int *sem;
	sem =(int *)currentproc->p_s.s_a1;
	*sem -=1;

	/*need to block and invoke scheduler*/
	if(*sem < 0){

		blockCurrent(&(sem));
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
	int *sem;

	/*thing as passeren()*/
	sem = (int *) currentproc->p_s.s_a1;
	*sem +=1;

	if (*sem <= 0){
		temp = removeBlocked(sem);

		/*need to check and make sure temp is not null*/

		if (temp != NULL){

			insertProcQ(&(readyQueue), temp);
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
	interruptLine = (int) currentproc->p_s.s_a1;

	/*need to set device number to a2*/
	deviceNum = (int) currentproc->p_s.s_a2;

	/*convert devices to sema4 */
	deviceNum += ((interruptLine - DISKINT) * DEVPERINT);/*find which device your in*/

	/*terminal reading*/

	if((interruptLine = TERMINT) && (currentproc->p_s.s_a3 == TRUE)){
		deviceNum += DEVPERINT;
	}

	/*taking one off the device list */
	devices[deviceNum] -= 1;

	/*now if it doesn't occur*/

	/*need to block and schedule*/
	if(devices[deviceNum] < 0){
		softBlock +=1;
		blockCurrent(&(devices[deviceNum]));
		
	}else{
		currentproc->p_s.s_v0 = saveStat[deviceNum];

		contextSwitch(currentproc);
	}

}

/******************** SYS 7 *****************************************/
void waitClock(){

	/*processes ClockWait syscall*/

	devices[DEVPERINT + DEVICECNT] -= 1;
	/*take one off of the clock sem4*/

	/*need to block current process*/
	if(devices[DEVPERINT + DEVICECNT] < 0){
		/*now block it*/

		softBlock += 1; 
		blockCurrent(&(devices[DEVPERINT + DEVICECNT]));
	}

	/*finally context switch*/
	contextSwitch(currentproc);

}

/******************** SYS 8 *****************************************/
support_t* supportPtr(){

	/*variables*/
	support_t *support;

	/*setting support*/
	support = currentproc->p_supportStruct;

	if(support != NULL){
		return (support);
	}

	return 0; 
}

HIDDEN void passUpOrDie(int except){

	/*idk what is happening here*/ 

	/*check to make sure the support is not null*/
	if(currentproc->p_supportStruct != NULL){
		/*if it is not null kill it*/
		moveState((state_t *)BIOSDATAPAGE, &(currentproc->p_supportStruct->sup_exceptState[except]));

		LDCXT(currentproc->p_supportStruct->sup_exceptContext[except].c_stackPtr, 
		currentproc->p_supportStruct->sup_exceptContext[except].c_status,
		currentproc->p_supportStruct->sup_exceptContext[except].c_pc);
	}

	/*if it is null send it to the passup vector*/

	/*get rid of the currentproc*/
	removeProcess(currentproc);

	scheduler();
}

void blockCurrent(int *blockSem){
	/*places the current process that is being called into the queue. Afterwards it calls
	to switch over the process*/

	/*variables*/
	cpu_t stopTOD; 

	/*read clock*/
	STCK(stopTOD);

	/*set the currentproc's time to the new stopTOD */
	currentproc->p_time += ((stopTOD)- startTOD);
	insertBlocked(blockSem, currentproc);

	/*set the current to NULL*/
	currentproc = NULL;

	/*go to the next process*/
	debuggerA(3);
	scheduler();
}

void removeProcess(pcb_t *proc){
	/*this helps with sys 2 whitch removes a process. The function
	recursively removes the process downwards and then frees the pcb
	and decreases the processcnt*/

	/* recursively removes the process down

	/*variables*/
	pcb_t *temp;
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
	if(proc->p_semAdd == NULL){
		outProcQ(&(readyQueue), proc);
	}

	temp = outBlocked(proc);

	if(temp != NULL){
		sem = temp->p_semAdd;

		if(sem >= &devices[0] && sem <= &devices[DEVICECNT + DEVPERINT]){
			softBlock -=1;
		}

		sem += 1;
	}

	/*set the pcb free*/
	freePcb(proc);

	/*fix the process count*/
	processcnt -=1;
}