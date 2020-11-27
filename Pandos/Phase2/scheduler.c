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
 */



/*switches to the process that needed to be in control*/
void contextSwitch(pcb_t *process)
{   
    /*make sure current proc is set*/
    currentproc = process;

    /*call loadstate*/
    LDST(&(process->p_s));

}


/*takes the next process on the ready queue, and makes it the current process.*/
void scheduleProcess()
{

    /*local variables*/
    pcb_t *newProc;
    unsigned int waitState;
    

    /*if ready queue isnt empty*/
    if (readyQ != NULL)
    {
        newProc = removeProcQ(&readyQ);/*remove it from the queue*/

        /*start the clock*/
        STCK(startTime);     
        setTIMER(STANQUANTUM); 
        
        contextSwitch(newProc);
    }

    /*if no processes, halt*/
    if (processcnt == 0)
    {
        HALT(); 
    }

    
    if (processcnt > 0 && softBlock > 0)
    {
        currentproc = NULL;
        setTIMER(MAXINT); 
    
        /*fix status*/
        waitState = (ALLOFF | IECURRENTON | IMASKON | TIMEREBITON);

        setSTATUS(waitState);

        WAIT();
    }

    /*anything else just panis*/
    if (processcnt > 0 && softBlock == 0)
    {
        PANIC();
    }

    return;
    
}/*****END OF SCEDUALEPROCESS*/



/*this prepares the interval timer for given pcb, call this when you need to load new process,
and for clock and io interrupts*/
void intervalSwitch(cpu_t specificTime)
{
    STCK(startTime); /*call the STCK to read the clock*/
    setTIMER(specificTime); /*set the timer to time given*/

    contextSwitch(currentproc);
}

/***********************END*****************************/