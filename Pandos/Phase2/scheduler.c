#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"
#include "/usr/local/include/umps3/umps/libumps.h"
#include "../h/initial.h"
#include "../h/exceptions.h"
#include "../h/interrupts.h"

/********************************************************************************
 * 
 * This file contains all methods and instruction for scheduler. This file 
 * controls the ready queue so that it can guarantee the execution of 
 * instructions. This is a round-robin scheduling algorithm. When it is called it
 * removes a PCB from the head of the queue and stores the pointer to a PCB in 
 * the current process in the ready queue. Loads a milliseconds on the PLT.
 * Performs a load processor state on the processor state stored in PCB of the 
 * Current Process.
 * 
 *********************************************************************************/



void contextSwitch(pcb_t *next){   
    /**This function performs a load state so that the next process that needs to be in 
     **control is in control. This is basically a context switch*/

    /*make sure current proc is set*/
    currentproc = next;

    /*call loadstate*/
    LDST(&(next->p_s));

}

void scheduleProcess(){
    /**This function takes the next process that is on the ready queue and makes it the 
     **the current process. Given the state of the process count and softblock will control
     **whether it PANIC, HALT, OR WAIT.*/


    /*local variables*/
    pcb_t *nextProc;
    unsigned int procState;
    

    /*if ready queue isnt empty*/
    if (readyQueue != NULL){
        nextProc = removeProcQ(&readyQueue);/*remove it from the queue*/

        /*start the clock*/
        STCK(startTOD);     
        setTIMER(QUANTUM); 
        
        contextSwitch(nextProc);
    }

    /*if no processes, halt*/
    if (processcnt == 0){
        HALT(); 
    }

    
    if (processcnt > 0 && softBlock > 0){
        currentproc = NULL;
        setTIMER(MAXINT); 
    
        /*fix status*/
        procState = (ALLOFF | IECURRENTON | IMASKON | TIMEREBITON);

        setSTATUS(procState);

        WAIT();
    }

    /*anything else just panis*/
    if (processcnt > 0 && softBlock == 0){
        PANIC();
    }

    return;
    
}

void intervalSwitch(cpu_t setTime){
    /*This function is a helper funtion that set the interval timer for a function at
     **the beginning of a new process and for different interrupts that can occur*/

    /*reads the start clock*/
    STCK(startTOD); 

    /*set the timer*/
    setTIMER(setTime);

    /*call a loadstate*/
    contextSwitch(currentproc);
}