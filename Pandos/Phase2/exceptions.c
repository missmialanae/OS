#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"
#include "/usr/local/include/umps3/umps/libumps.h"
#include "../h/initial.h"
#include "../h/scheduler.h"
#include "../h/initial.h"

/*
 * This file contains all methods and instructions for exception handling.
 * This file controls the population in the pass up vector witht the address 
 * of the nucleus exception handler and the stack page. contains syscall info.
 * 
 */


/*external functions*/
extern void passUpOrDie(int exceptNum);
extern void pgmTrapH();
extern void moveState(state_t *source, state_t *copy);

HIDDEN int sys1();
HIDDEN void sys2(pcb_t *runningProc);
HIDDEN void sys3();
HIDDEN void sys4();
HIDDEN void sys5();
HIDDEN void sys6();
HIDDEN void sys7();
HIDDEN support_t * sys8();
HIDDEN void blockCurrent
(int *address);
HIDDEN cpu_t calcTime();

/*end of file specific methods*/

/************************************BEGIN METHOD DECLARATION************************************/

/**
 * This System Call (syscall) method occurs when the SYSCALL assembly 
 * intruction is executed. This places the appropriate values in 
 * the general purpose registers (a0-a3) and these values will
 * call the appropriate SYSCALL (1-8)
 * */
void sysTrapH()
{

    int sysNum; /*local variable and current sys*/

    sysNum = ((state_t *)BIOSDATAPAGE)->s_a0;

    /*check if in kernel mode*/
    if (sysNum >= 1 && sysNum <= 8 && (((state_t *)BIOSDATAPAGE)->s_status & USER) == 1)
    {
        /*change cause privs if not in kernal*/

        ((state_t *)BIOSDATAPAGE)->s_cause =
            (((state_t *)BIOSDATAPAGE)->s_cause & CLEAR) | (NOTPRIV << SHIFT);

        /*call program trap*/
        pgmTrapH();
    }

    /*in kernel mode, not user*/

    /*store the states in currentProc*/
    moveState(((state_t *)BIOSDATAPAGE), &(currentproc->p_s));

    currentproc->p_s.s_pc += 4; /*update stack pointer*/

    /*checks which sys it should go to*/
    if (sysNum == CREATEPROCESS)
    {
        int returnInt;
        returnInt = sys1();
        currentproc->p_s.s_v0 = returnInt;

        contextSwitch(currentproc);
    }

    if (sysNum == TERMINATEPROCESS)
    {
        sys2(currentproc);

        scheduleProcess(); 
    }

    if (sysNum == PASSERN)
    {
        sys3();
    }

    if (sysNum == VERHOGEN)
    {
        sys4();
        contextSwitch(currentproc);
    }

    if (sysNum == WAITIO)
    {
        sys5();
        contextSwitch(currentproc);
    }

    if (sysNum == GETCLOCK)
    {
        sys6();
        contextSwitch(currentproc);
    }

    if (sysNum == CLOCK)
    {
        sys7();
        contextSwitch(currentproc);
    }

    if (sysNum == SUPPORTPTR)
    {
        support_t *info = sys8();
        currentproc->p_s.s_v0 = info;
        contextSwitch(currentproc);
    }

    /*if none of these, then passup*/
    passUpOrDie(GENERALEXCEPT);
}


/*creates new process that is child of currentproc, inserts into tree*/
int sys1()
{

    /*local variables*/
    pcb_t *newPcb;
    state_t *allData;
    support_t *supportData;


    newPcb = allocPcb();

    /*if new PCB is null, fail*/
    if (newPcb == NULL)
    {
        return FAILED; /*put -1 in v0 when we can't make a process*/
    }

    processcnt += 1;
    allData = (state_t *)currentproc->p_s.s_a1;

    moveState(allData, &(newPcb->p_s)); /*copying states from parent to child*/
    supportData = (support_t *)currentproc->p_s.s_a2;

    /*if the support data is no null or not o then put newPCB on the support data*/
    if (!((supportData == NULL) || (supportData == 0)))
    {
        newPcb->p_supportStruct = supportData;
    }

    insertProcQ(&readyQ, newPcb);
    insertChild(currentproc, newPcb);
    return OKAY; /*put 0 in v0 when we make a process*/
}



/*when children need to be removed/wiped*/
void sys2(pcb_t *runningProc)
{
    /*local variables*/
    pcb_t *blockedChild;
    int *semNum;
    

    /*remove children till there is none left*/
    while (!(emptyChild(runningProc)))
    {
        sys2(removeChild(runningProc));
    }

    /*if current is running, remove it*/
    if (runningProc == currentproc)
    {
        outChild(runningProc); 
    }

    /*if on ready queue*/
    else if (runningProc->p_semAdd == NULL)
    {
        outProcQ(&readyQ, runningProc);
    }

    /* if blocked*/
    else
    {
        blockedChild = outBlocked(runningProc);

        if (blockedChild != NULL)
        {
            semNum = blockedChild->p_semAdd;

            /*if the semNum is valid*/
            if (semNum >= &deviceSema4[0] && semNum <= &deviceSema4[DEVCNT + DEVPERINT])
            {
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

    /*once not on the ready queue and unblocked, free*/
    freePcb(runningProc);
    processcnt -= 1;

}



/*performing p operation on sema4*/
void sys3()
{

    /*local variables*/
    int *semAddr;
    

    semAddr = (int *)currentproc->p_s.s_a1;

    /*update CPU*/
    *semAddr -= 1;

    /*if semAddress is less than 0 then do P operation*/
    if (*semAddr < 0)
    {
        blockCurrent(semAddr);
    }
    else
    {

        contextSwitch(currentproc);
    }
}


/*perform v operation on sema4*/
void sys4()
{

    /*local variables*/
    int *semAddr;
    pcb_t *removedPcb;
    

    semAddr = (int *)currentproc->p_s.s_a1;
    *semAddr += 1;

    /* if semaddress is less than or equal to 0 do the V operation*/
    if (*semAddr <= 0)
    {
        removedPcb = removeBlocked(semAddr);

        /*make sure to insert*/
        if (removedPcb != NULL)
        {
            insertProcQ(&readyQ, removedPcb);
        }
    }
}



/*transitions currentproc to running, the save state is in v0*/
void sys5()
{

    /*local variables*/
    int lineNum;
    int deviceNum;
    

    /*lineNum and deviceNum*/
    lineNum = currentproc->p_s.s_a1;
    deviceNum = currentproc->p_s.s_a2;

    /*find device*/
    deviceNum += ((lineNum - DISKINT) * DEVPERINT); 

    /*if interrupt on line 7 then correct deviceNum*/
    if ((deviceNum == TERMINT) && (currentproc->p_s.s_a3 == TRUE))
    {
        deviceNum += DEVPERINT;
    }

    deviceSema4[deviceNum] -= 1;

    /*block process and move on*/
    if (deviceSema4[deviceNum] < 0)
    {
        softBlock += 1;

        blockCurrent(&deviceSema4[deviceNum]);
    }

    /*interrupt happened and ACK, load savedState */
    currentproc->p_s.s_v0 = saveState[deviceNum];
}



/*function gives total proc time*/
void sys6()
{
     /*calculate the total time and set to v0*/

    currentproc->p_s.s_v0 = calcTime();
}



/*p operation on psuedo clock*/
void sys7()
{
    /*decrement sem address*/
    deviceSema4[DEVCNT + DEVPERINT] -= 1;

    /*increase softblock count and block the proc on clock*/
    if (deviceSema4[DEVCNT + DEVPERINT] < 0)
    {
        softBlock += 1;

        blockCurrent(&deviceSema4[DEVCNT+DEVPERINT]);

    }

}



/*returns value of supportStruct (p), from currentproc*/
support_t* sys8()
{

    /*return pointer if not null*/
    if (currentproc->p_supportStruct != NULL)
    {
        return (currentproc->p_supportStruct);
    }

    /*if nothing there, return null*/
    return (NULL);

}



/*handles program traps and calls passup*/
void pgmTrapH(){

    passUpOrDie(GENERALEXCEPT);
}

/* function passes the memory management issue to passUpOrDie*/

void tlbTrapH(){

    passUpOrDie(PGFAULTEXCEPT);
}



/*function that takes all sys calls and passup if currentproc support isnt null
*or call sys2 to terminate*/

void passUpOrDie(int exceptNum)
{
    /*start local variables*/
    unsigned int stackPtr;
    unsigned int status;
    unsigned int pc;
    

    /*if current process has support struct, passup*/
    if (currentproc->p_supportStruct != NULL)
    {
        moveState((state_t *)BIOSDATAPAGE, &(currentproc->p_supportStruct->sup_exceptState[exceptNum]));

        stackPtr = currentproc->p_supportStruct->sup_exceptContext[exceptNum].c_stackPtr;
        status = currentproc->p_supportStruct->sup_exceptContext[exceptNum].c_status;
        pc = currentproc->p_supportStruct->sup_exceptContext[exceptNum].c_pc;

        LDCXT(stackPtr, status, pc);
    }
    
    /*or die*/
    sys2(currentproc);

    scheduleProcess();
    
}


/*moveState method, copies*/
void moveState(state_t *source, state_t *move)
{

    int i; /*local variable*/

    /*cycle through all the states and copy them*/
    for (i = 0; i < STATEREGNUM; i += 1)
    {
        move->s_reg[i] = source->s_reg[i];
    }

    /*copy the last 4*/

    move->s_cause = source->s_cause;
    move->s_entryHI = source->s_entryHI;
    move->s_status = source->s_status;
    move->s_pc = source->s_pc;
}


/*function to stop the time keeping and block proc, then schedule*/
void blockCurrent(int *address)
{
    /*time calculations*/
    currentproc->p_time = calcTime();

    /*block process*/
    insertBlocked(address, currentproc);
    currentproc = NULL;

    scheduleProcess();
}



/*calculate total time used by a proc, return the time*/
cpu_t calcTime(){
    cpu_t endtime;
    cpu_t totalTime;

    STCK(endtime);

    totalTime = currentproc->p_time + (endtime-startTime);

    return(totalTime);

    
}