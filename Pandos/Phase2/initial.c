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
* start the timers, and instaniate single process ready queue and call the sceduler to end control from the 
* main().
*
************************************************************************************************************/


extern void uTLB_RefillHandler();   
extern void test();                 
HIDDEN void GenExceptionHandler(); 


/**global variables**/

/*saves the state*/
unsigned int saveState[DEVCNT + DEVPERINT]; 

/*process count*/
int processcnt;     

/*soft block count*/                        
int softBlock; 

/*readyQueue*/                          
pcb_t *readyQueue; 

/*current process */                           
pcb_t *currentproc;  

/*device sema4 and setting it with the proper count*/                       
int devices[DEVCNT + DEVPERINT + 1];  

/*start the timer*/
cpu_t startTOD;

/*timer for slice*/                         
cpu_t timeSlice;                            

 

int main(){
    /**This function is the main and allows the inializion of all variables, starts clocks, sets up 
     **the passup vector, sets up the RAM, begins the first process. This will only run once. 
     **Once scheduler done, main has no control. */

    /*local variables*/
    passupvector_t *passup; /*passup vector*/
    int i; /*device for loop*/
    pcb_t *current;
    memaddr topOfRam; /*used for RAMTOP*/
    
    /*Start the PCBs and ASL*/
    initPcbs(); 
    initASL(); 

    /*************************Initalize Variables*****************************************************/
    processcnt = 0;   
    softBlock = 0; 
    readyQueue = mkEmptyProcQ();
    currentproc = NULL; 


    /*************************PASSUP VECTOR*****************************************************/

    /*inialize the passup to be the passupvector*/
    passup = (passupvector_t *)PASSUPVECTOR; 

    /*setting refill handler and exception handler*/
    passup->tlb_refll_handler = (memaddr)uTLB_RefillHandler; 
    passup->tlb_refll_stackPtr = KERNAL;
    passup->exception_handler = (memaddr)GenExceptionHandler; 
    passup->exception_stackPtr = KERNAL;

   /*setting clock sema4 to 0*/
    devices[DEVCNT + DEVPERINT] = 0;
    
    /*Initializing the devices list*/
    for (i = 0; i < (DEVCNT + DEVPERINT); i++){
        devices[i] = 0;
    }

    /*set interval time to 100 milliseconds*/
    LDIT(PSEUDO); 

    /*set RAM Top*/
    RAMTOP(topOfRam);

    /*create first process*/
    current = allocPcb();

    /*If nothing allocated*/
    if (current == NULL){

        /*panic*/
        PANIC();
    }

    /*if something allocated, put it on top of the RAM*/
    else{

         /*links it to test file*/
        current->p_s.s_pc = (memaddr)test;
        current->p_s.s_t9 = (memaddr)test;

        /*setting the status of all bits to off*/
        current->p_s.s_status = (ALLOFF | IEPREVON | IMASKON | TIMEREBITON); 

        /*set top of RAM*/
        current->p_s.s_sp = topOfRam;

        /*increase the process count*/
        processcnt +=1;

        /*insert into readyQueue*/
        insertProcQ(&readyQueue, current); 
        
        /*schedule next process and signal last control of main*/
        scheduleProcess(); 
    }

    /*Mikey said to do this*/
    return (0); 

} /*end of main*/



void GenExceptionHandler(){
    /**This function looks into the cause register and leads the program to the appropriate exception 
     **handling whether it be a program trap, a sycall, or an IO interrupt. The cause register is 
     **stored in the BIOSDATAPAGE. */

    /*local variables*/
    state_t *currentState;
    int reason;
    
    /*check state in BIOSDATAPAGE*/
    currentState = (state_t *)BIOSDATAPAGE; 

    reason = (int)((currentState->s_cause & CAUSE) >> SHIFT); /*get cause number*/
    
    if (reason == SYSCALLS){ 

        /*if cause is 8, go to syscall*/
        sysTrapH();
    }

    else if (reason == INTERRUPTS)
    {
        /*if cause is 0, go to interrupts*/
        trapH();
    }

    else if (reason <= CAUSEMAX && reason > CAUSEMIN)
    {
        /*if cause between 0 and 3, go to tlbTrapH*/
        tlbTrapH();
    }

    else{
         /*if anything else*/
         pgmTrapH();

    }

}