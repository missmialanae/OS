#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"
#include "/usr/local/include/umps3/umps/libumps.h"
#include "../h/exceptions.h"
#include "../h/interrupts.h"
#include "../h/scheduler.h"

/**********************************************************************************************************
*
* This file contains all methods and instructions for initial. This file is basically the main().
* Here is where we declare all variables, the population of the pass up vector, the initialaztion of phase 1,
* begin the timers, and instaniate single process ready queue and call the sceduler to end control from the 
* main().
*
************************************************************************************************************/


/************ DECLARE OF GLOBAL VARIABLES AND METHODS *********************/

extern void uTLB_RefillHandler();   
extern void test();                 
HIDDEN void GenExceptionHandler(); 


/**global variables**/

unsigned int saveState[DEVCNT + DEVPERINT]; 
int processcnt;                             
int softBlock;                           
pcb_t *readyQ;                              
pcb_t *currentproc;                         
int deviceSema4[DEVCNT + DEVPERINT + 1];       
cpu_t startTime;                            
cpu_t timeSlice;                            


 /*deviceSema4[DEVCNT+DEVPERINT] will be used as our clock sema4*/
 

int main()
{
    /*local variables*/
    passupvector_t *passUpVec; /*passup vector*/
    int counter; /*device for loop*/
    pcb_t *newPcb;
    memaddr topRamAdd; /*used for RAMTOP*/
    

    initPcbs(); /*initalize all pcbs and set up free list*/
    initASL(); /*create sema4 free list and ASL*/

    processcnt = 0;   /*Initializing process count*/
    softBlock = 0; /*Initializing soft block count*/
    readyQ = mkEmptyProcQ();/*ready queue*/
    currentproc = NULL; /*setting current process*/


    /***PASSUP VECTOR*****/

    passUpVec = (passupvector_t *)PASSUPVECTOR; /*pointer that points to the PASSUPVECTOR*/

    passUpVec->tlb_refll_handler = (memaddr)uTLB_RefillHandler; /*setting refill handler*/
    passUpVec->tlb_refll_stackPtr = KERNAL;
    passUpVec->exception_handler = (memaddr)GenExceptionHandler; /*setting exception handler*/
    passUpVec->exception_stackPtr = KERNAL;

   /*setting clock sema4 to 0*/
    deviceSema4[DEVCNT + DEVPERINT] = 0;
    
    /*Initializing the deviceSema4 list and setting everything in it to 0*/
    for (counter = 0; counter < (DEVCNT + DEVPERINT); counter+=1)
    {
        deviceSema4[counter] = 0;
    }

    LDIT(PSEUDO); /*set interval time to 100 milliseconds*/

    /*using C macro to set up the location of topRamAdd*/
    RAMTOP(topRamAdd);

    newPcb = allocPcb();/*create a new process*/

    /*If nothing allocated, panic*/
    if (newPcb == NULL)
    {
        PANIC();
    }

    /*if something allocated, put it on top of the RAM*/
    else
    {
        newPcb->p_s.s_pc = (memaddr)test; /*link to test file*/
        newPcb->p_s.s_t9 = (memaddr)test;

        newPcb->p_s.s_status = (ALLOFF | IEPREVON | IMASKON | TIMEREBITON); /*get status*/
        newPcb->p_s.s_sp = topRamAdd;/*set top of RAM*/

        processcnt +=1;/*new process ready to go!*/

        insertProcQ(&readyQ, newPcb); /*insert into readyQ*/
        
        scheduleProcess(); /*schedule next process*/

    }

    return (0); /*we're done with main*/

} /*end of main*/


/*looks at the cause register (stored by BIOS) and points to which syscall it is*/
void GenExceptionHandler()
{
    /*local variables*/
    state_t *programState;
    int causeNum;
    

    programState = (state_t *)BIOSDATAPAGE; /*check state in BIOSDATAPAGE*/

    causeNum = (int)((programState->s_cause & CAUSE) >> SHIFT); /*get cause number*/
    
    if (causeNum == GOTOSYSCALL)
    { 
        /*if cause is 8, go to syscall*/
        sysTrapH();
    }

    else if (causeNum == GOTOINTERRUPTS)
    {
        /*if cause is 0, go to interrupts*/
        trapH();
    }

    else if (causeNum <= PGFAULTCAUSEMAX && causeNum > PGFAULTCAUSEMIN)
    {
        /*if cause between 0 and 3, go to tlbTrapH*/
        tlbTrapH();
    }

    else{
         /*if anything else*/
         pgmTrapH();

    }

}/*end GenExceptionHandler*/