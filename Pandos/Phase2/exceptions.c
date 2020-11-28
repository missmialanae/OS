#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"
#include "/usr/local/include/umps3/umps/libumps.h"
#include "../h/initial.h"
#include "../h/scheduler.h"
#include "../h/initial.h"

/**********************************************************************************
 *
 * This file contains all methods and instructions for exception handling.
 * This file controls the all the proper directions and instructions for 
 * syscalls and  program traphs. It also contains the proper ending for pass up 
 * or die. This final also contains the instructions for moveState, blockCurrent,
 * and calcTime which are all helper functions.
 * 
 *********************************************************************************/


/*external functions*/
extern void passUpOrDie(int exceptNum);
extern void pgmTrapH();
extern void moveState(state_t *source, state_t *final);

HIDDEN int create();
HIDDEN void removeProcess(pcb_t *proc);
HIDDEN void passern();
HIDDEN void verhogen();
HIDDEN void wait();
HIDDEN void getTime();
HIDDEN void getClock();
HIDDEN support_t * getSupport();
HIDDEN void blockCurrent(int *semAddy);
HIDDEN cpu_t calcTime();


void pgmTrapH(){
    /*handles program traps and calls passup*/

    passUpOrDie(GENERALEXCEPT);
}

void tlbTrapH(){
    /* function passes the memory management issue to passUpOrDie*/


    passUpOrDie(PGFAULTEXCEPT);
}

void sysTrapH(){
    /**This method begins the process of decided how to handle a syscall when called upon.
     ** a number associated with a constant is given and the fuction will locate the proper
     ** instruction to follow. This only works for syscalls 1 - 8.**/

    int sys; /*local variable and current sys*/

    sys = ((state_t *)BIOSDATAPAGE)->s_a0;

    /*check if in kernel mode*/
    if (sys >= 1 && sys <= 8 && (((state_t *)BIOSDATAPAGE)->s_status & USER) == 1){
        /*change cause privs if not in kernal*/

        ((state_t *)BIOSDATAPAGE)->s_cause =
            (((state_t *)BIOSDATAPAGE)->s_cause & CLEAR) | (NOTPRIV << SHIFT);

        /*call program trap*/
        pgmTrapH();
    }

    /*in kernel mode, not user*/

    /*store the states in currentProc*/
    moveState(((state_t *)BIOSDATAPAGE), &(currentproc->p_s));

    currentproc->p_s.s_pc += 4; /*need to update stack pointer*/

    /*checks which sys it should go to*/

    /************************************SYS 1************************************/

    if (sys == CREATEPROCESS){
        int returnInt;
        returnInt = create();
        currentproc->p_s.s_v0 = returnInt;

        contextSwitch(currentproc);
    }
     /************************************SYS 2************************************/

    if (sys == TERMINATEPROCESS){
        removeProcess(currentproc);

        scheduleProcess(); 
    }
     /************************************SYS 3************************************/

    if (sys == PASSERN){
        passern();
    }
     /************************************SYS 4************************************/
    if (sys == VERHOGEN){
        verhogen();
        contextSwitch(currentproc);
    }

     /************************************SYS 5************************************/

    if (sys == WAITIO){
        wait();
        contextSwitch(currentproc);
    }
     /************************************SYS 6************************************/
    if (sys == GETCLOCK){
        getTime();
        contextSwitch(currentproc);
    }
     /************************************SYS 7************************************/

    if (sys == CLOCK){
        getClock();
        contextSwitch(currentproc);
    }
     /************************************SYS 8************************************/

    if (sys == SUPPORTPTR){
        support_t *data = getSupport();
        currentproc->p_s.s_v0 = data;
        contextSwitch(currentproc);
    }

    /*if none of these, then passup*/
    passUpOrDie(GENERALEXCEPT);
}

int create(){
    /**This is sys 1. The method will creates new process which is the child of currentproc, 
     **inserts into tree. It will return the value that is going to be put into the 
     **currentproc's v0 which is indicated by a FAILED or OKAY constant */

    /*local variables*/
    pcb_t *created; /*the new pcb that is being created*/
    state_t *stateData; /*holds the state of the parent*/
    support_t *supportData; /*support to hold the a2 of the currentproc/parent*/


    created = allocPcb();

    /*if new PCB is null*/
    if (created == NULL){
        return FAILED; /*put -1 in v0 cause we can't make a process*/
    }

    /*increase the processcnt*/
    processcnt += 1;

    /*set the stateData with a1*/
    stateData = (state_t *)currentproc->p_s.s_a1;

    moveState(stateData, &(created->p_s)); /*call moveState from parent to child*/
    supportData = (support_t *)currentproc->p_s.s_a2; /*set the supportData*/

    /*if the support data is no null or not 0 */
    if (!((supportData == NULL) || (supportData == 0))){

        /*then put created on the support data*/
        created->p_supportStruct = supportData;
    }

    /*insert*/
    insertProcQ(&readyQueue, created);
    insertChild(currentproc, created);
    return OKAY; /*put 0 in v0 to make a process*/
}



/*when children need to be removed/wiped*/
void removeProcess(pcb_t *proc){
    /**This is sys 2. This function will recursively remove all children of a given pcb.
     **The function is given a function a pcb called proc and it will free proc and 
     ** decrease the proccess count*/

    /*local variables*/
    pcb_t *remove; /* pcb of the one being removed*/
    int *semNum; /*sem addy*/
    

    /*if the proc is not empty*/
    while (!(emptyChild(proc))){
         /*remove children till there is none left*/
        removeProcess(removeChild(proc));
    }

    /*if current is running, remove it*/
    if (proc == currentproc){
        outChild(proc); 
    }

    /*if on ready queue*/
    else if (proc->p_semAdd == NULL){
        outProcQ(&readyQueue, proc);
    }

    /* if blocked*/
    else
    {
        remove = outBlocked(proc);

        /*if the remove is not NULL*/
        if (remove != NULL){
            semNum = remove->p_semAdd; /*set the semNum*/

            /*if the semNum is valid*/
            if (semNum >= &devices[0] && semNum <= &devices[DEVCNT + DEVPERINT]){

                /*remove blocked process*/
                softBlock -= 1;
            }

            else
            {
                /*if proc wasnt blocked*/
                *semNum += 1;
            }
        }
    }

    /*once not on the ready queue and unblocked */

    freePcb(proc); /*set her free*/
    processcnt -= 1; /*decrease the processcnt*/

}


void passern(){
    /**This is sys 3. It performs the p operation on sem address which is then needed 
     **to blocked.The sem address is also decresed*/

    /*local variables*/
    int *sem; /*sem addy*/
    
    /*set sem addy to a1*/
    sem = (int *)currentproc->p_s.s_a1;

    /*update CPU*/
    *sem -= 1;

    /*if semess is less than 0 then do P operation*/
    if (*sem < 0){
        blockCurrent(sem);
    }

    else
    {

        contextSwitch(currentproc);
    }
}



void verhogen(){
    /**This is sys 4. It performs v operation on a sem address which is them removed and 
     **If it needed to v then it will be inserted into the readyQ*/

    /*local variables*/
    int *sem; /*sem addy*/
    pcb_t *removedPcb; /*points to the pcb that is going to be removed from readyQueue*/
    
    /*set the sem to be the a1 of the curren proc*/
    sem = (int *)currentproc->p_s.s_a1;

    /*increase the sem amount*/
    *sem += 1;

    /* if sem is less than or equal to 0 do the V operation*/
    if (*sem <= 0){
        removedPcb = removeBlocked(sem);

        /*make sure to insert*/
        if (removedPcb != NULL){
            insertProcQ(&readyQueue, removedPcb);
        }
    }
}

void wait(){
    /**This is sys 5. This method set the currentproc to be the proc that is running. It 
     **will also save the current state in v0*/

    /*local variables*/
    int lineNum; /*line number*/
    int deviceNum; /*device number*/
    

    /* setting the lineNum and deviceNum*/
    lineNum = currentproc->p_s.s_a1;
    deviceNum = currentproc->p_s.s_a2;

    /*find device*/
    deviceNum += ((lineNum - DISKINT) * DEVPERINT); 

    /*if interrupt on line 7 then correct deviceNum*/
    if ((deviceNum == TERMINT) && (currentproc->p_s.s_a3 == TRUE)){
        deviceNum += DEVPERINT;
    }

    /*decrease the deviceNum of devices*/
    devices[deviceNum] -= 1;

    /*if the device[deviceNum] is less than 0*/
    if (devices[deviceNum] < 0){

        /*increase the softBlock count*/
        softBlock += 1;

        /*Block it*/
        blockCurrent(&devices[deviceNum]);
    }

    /*interrupt happened and ACK, load savedState */
    currentproc->p_s.s_v0 = saveState[deviceNum];
}



/*function gives total proc time*/
void getTime(){
     /**This is sys 6. This methods runs the calculate the total time by calling a 
      **calcTime functionand set to v0.*/

    currentproc->p_s.s_v0 = calcTime();
}




void getClock(){
    /**This is sys 7. This method will perform the p operation on psuedo clock similar to 
     **sys 3. */

    /*decrement device sem*/
    devices[DEVCNT + DEVPERINT] -= 1;

    /*if devices is less than 0*/
    if (devices[DEVCNT + DEVPERINT] < 0){

        /*increase softblock count and block the proc on clock*/
        softBlock += 1;

        blockCurrent(&devices[DEVCNT+DEVPERINT]);

    }
}

support_t* getSupport(){
    /**This is sys 8. This method returns value of supportStruct from currentproc.
     **if there isn't one it will return null*/

    /*return pointer if not null*/
    if (currentproc->p_supportStruct != NULL){

        return (currentproc->p_supportStruct);
    }

    /*if nothing there, return null*/
    return (NULL);

}

/*function that takes all sys calls and passup if currentproc support isnt null
*or call removeProcess to terminate*/

void passUpOrDie(int exceptNum){
    /**This function takes all syscalls and if the support of the currentproc is not NULL
     **then it will pass up. If it is NULL then it will call removeProcess and end it and
     ** call scheduleProcess*/

    /*start local variables*/
    unsigned int stack; 
    unsigned int status;
    unsigned int pc;
    

    /*if current process has support struct, passup*/
    if (currentproc->p_supportStruct != NULL){
        /*call movestate and move the BIOSDATAPAGE to the support structure*/
        moveState((state_t *)BIOSDATAPAGE, &(currentproc->p_supportStruct->sup_exceptState[exceptNum]));

        /*set the stack pointer*/
        stack = currentproc->p_supportStruct->sup_exceptContext[exceptNum].c_stackPtr;
        /*set the status*/
        status = currentproc->p_supportStruct->sup_exceptContext[exceptNum].c_status;

        /*set the pc*/
        pc = currentproc->p_supportStruct->sup_exceptContext[exceptNum].c_pc;

        /*call LDCXT*/
        LDCXT(stack, status, pc);
    }
    
    /*or die*/
    removeProcess(currentproc);

    /*schedule*/
    scheduleProcess();
    
}

void moveState(state_t *source, state_t *final){
    /**This function is basically just copies the cause, entryHI, status, anc pc of the 
     **source and move it to the final desination*/

    int i; /*local variable*/

    /*cycle through all the states and copy them*/
    for (i = 0; i < STATEREGNUM; i += 1)
    {
        final->s_reg[i] = source->s_reg[i];
    }

    /*copy the last 4*/

    final->s_cause = source->s_cause;
    final->s_entryHI = source->s_entryHI;
    final->s_status = source->s_status;
    final->s_pc = source->s_pc;
}

void blockCurrent(int *semAddy){

    /**This function will block the current proc. The fucntion takes in a sem address and 
     **will stop the time, block it and then call scheduleProcess.*/

    /*time calculations*/
    currentproc->p_time = calcTime();

    /*block process*/
    insertBlocked(semAddy, currentproc);
    currentproc = NULL;

    scheduleProcess();
}

cpu_t calcTime(){
    /**This is a helper function for sys 6 and the calling block current. It will end the 
     **time and calculate the time and return the final total time. */

    /*local variables*/
    cpu_t endtime; /*set the end time*/
    cpu_t finalTime; /*final time*/

    /*read the time*/
    STCK(endtime); 

    /*gets the final time*/
    finalTime = currentproc->p_time + (endtime-startTOD);

    /*return the final time*/
    return(finalTime);
}