#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"
#include "/usr/local/include/umps3/umps/libumps.h"
#include "../h/exceptions.h"
#include "../h/initial.h"
#include "../h/scheduler.h"

/**************************************************
* Thie file contains all methods and instruction for interrupts. This file controls
* context switches and deals with the mapping of all the devices through. 
* deals with non timer interrupts, plt interrups, 
* and the system wide intercal timer 
* and psuedoclock
*
*
/**************************************************/

/*externs*/

HIDDEN void localTimer(cpu_t time);
HIDDEN void pseudoInterrupts();
HIDDEN void deviceIOHandler(int deviceType);



/*method to do the specific actions when an interrupt occurs and what caused it*/
void trapH()
{

    /*local variables*/
    cpu_t stopTime;
    cpu_t quantumTimer;
    

    STCK(stopTime);
    quantumTimer = getTIMER();

    /*determining cause of interrupt*/

    if (((((state_t *)BIOSDATAPAGE)->s_cause) & PLTINTERRUPT) != 0)
    {
        /*local timer interrupt*/
        localTimer(stopTime);
    }

    else if (((((state_t *)BIOSDATAPAGE)->s_cause) & PSEUDOCLOCKINT) != 0)
    {
        /*timer interrupt*/
        pseudoInterrupts();
    }

    else if (((((state_t *)BIOSDATAPAGE)->s_cause) & DISKINTERRUPT) != 0)
    {
        /*disk interrupt*/
        deviceIOHandler(DISKINT);
    }

    else if (((((state_t *)BIOSDATAPAGE)->s_cause) & FLASHINTERRUPT) != 0)
    {
        /*flash interrupt*/
        deviceIOHandler(FLASHINT);
    }

    else if (((((state_t *)BIOSDATAPAGE)->s_cause) & PRINTINTERRUPT) != 0)
    {
        /*print interrupt*/
        deviceIOHandler(PRNTINT);
    }

    else if (((((state_t *)BIOSDATAPAGE)->s_cause) & TERMINALINTERRUPT) != 0)
    {
        /*terminal interrupt*/
        deviceIOHandler(TERMINT);
    }

    if (currentproc != NULL)
    {

        currentproc->p_time += (stopTime - startTime);
        moveState((state_t *)BIOSDATAPAGE, &(currentproc->p_s));
        intervalSwitch(quantumTimer);
    }

    else
    {

       /*if no currentproc, then halt*/
        HALT();
    }

}/**end of trapH**/


/*handles the procedures to handle cpu when it generates a clock interrupt*/
void localTimer(cpu_t timeStop)
{
    /*if no currentproc, panic*/
    if (currentproc == NULL)
    {
        PANIC();
    }


    currentproc->p_time += (timeStop - startTime);
    moveState((state_t *)BIOSDATAPAGE, &(currentproc->p_s));

    insertProcQ(&readyQ, currentproc);
    currentproc = NULL;

    /*put back on ready queue and schedule next*/
    scheduleProcess();
}/**end of local timer**/


/*this function hancles pseudo clock tick. performs v operations every 100 milliseconds
on nucelus maintained sema4 list called psuedoclock*/
void pseudoInterrupts()
{
    /*local variable*/
    pcb_t *removedPcbs;


    LDIT(PSEUDO);/*reset psuedoclock*/

    removedPcbs = removeBlocked(&(deviceSema4[DEVPERINT + DEVCNT]));

    /*remove it*/
    while (removedPcbs != NULL)
    {
        /*need to place it on ready queue*/
        insertProcQ(&readyQ, removedPcbs);

        /*decrease soft block count*/
        softBlock -= 1;
        removedPcbs = removeBlocked(&(deviceSema4[DEVPERINT + DEVCNT]));
    }

     /*reset psuedo to 0*/
    deviceSema4[DEVPERINT + DEVCNT] = 0;

    /*check there is no currentproc, call scheduleProcess*/
    if (currentproc == NULL)
    {
        scheduleProcess();
    }

    
}

/**this function will handle any devices that are going to interrupt using devices to map to correct device**/
void deviceIOHandler(int lineNum)
{

    /*Local Variables*/
    int deviceNumber;
    unsigned int devStatus;
    unsigned int bitMap;
    volatile devregarea_t *deviceRegister;
    pcb_t *pseudoSys4;
    volatile devregarea_t *devRegisters;
    int deviceFound;
    

    deviceFound = FALSE; /*tells us if we found the deviceNum*/

    /*establish addressing*/
    deviceRegister = (devregarea_t *)RAMBASEADDR;
    bitMap = deviceRegister->interrupt_dev[(lineNum - DISKINT)];

    /*if the bitMap has nothing in it, panic*/
    if ((&(bitMap) == NULL) & !(deviceFound))
    {
        PANIC();
    }

    /*see which device is on, bitmapping*/
    if (((bitMap & DEVICE0) != 0) & !(deviceFound) )
    {
        deviceNumber = 0;
        deviceFound = TRUE; /*we found the device!*/
    }

    if (((bitMap & DEVICE1) != 0) & !(deviceFound))
    {
        deviceNumber = 1;
        deviceFound = TRUE;
    }

    if (((bitMap & DEVICE0) != 0) & !(deviceFound))
    {
        deviceNumber = 2;
        deviceFound = TRUE;
    }

    if (((bitMap & DEVICE0) != 0) & !(deviceFound))
    {
        deviceNumber = 3;
        deviceFound = TRUE;
    }

    if (((bitMap & DEVICE0) != 0) & !(deviceFound))
    {
        deviceNumber = 4;
        deviceFound = TRUE;
    }

    if (((bitMap & DEVICE0) != 0) & !(deviceFound))
    {
        deviceNumber = 5;
        deviceFound = TRUE;
    }

    if (((bitMap & DEVICE0) != 0) & !(deviceFound))
    {
        deviceNumber = 6;
        deviceFound = TRUE;
    }

    if (((bitMap & DEVICE0) != 0) & !(deviceFound))
    {
        deviceNumber = 7;
        deviceFound = TRUE;
    }

    /*determine sem for device*/
    deviceNumber += ((lineNum - DISKINT) * DEVPERINT);

    /*case for terminal*/
    if (lineNum == TERMINT)
    {
        /*check to see if terminal is transmit or receive*/ 

        /*set devRegisters*/

        devRegisters = (devregarea_t *)RAMBASEADDR;

        /*get status from registers*/
        devStatus = devRegisters->devreg[(deviceNumber)].t_transm_status;

        if ((devStatus & BITS) != TRUE){

            /*ack the recieve*/
            devRegisters->devreg[(deviceNumber)].t_transm_command = ACK;
        }

        else
        {
            devStatus = devRegisters->devreg[(deviceNumber)].t_recv_status;
            devRegisters->devreg[(deviceNumber)].t_recv_command = ACK;

            /*increment the device sema4 by DEVPERINT*/
            deviceNumber += DEVPERINT;
        }
    }

    else
    {

        devStatus = (deviceRegister->devreg[deviceNumber]).d_status; /*copy status*/
        (deviceRegister->devreg[deviceNumber]).d_command = ACK;      /*ACK interrupt*/
    }

    deviceSema4[deviceNumber] += 1;

    /*done waiting for IO, unblock pcb */
    if (deviceSema4[deviceNumber] <= 0)
    {
        pseudoSys4 = removeBlocked(&(deviceSema4[deviceNumber]));

        if (pseudoSys4 != NULL)
        {
            /*if there is a process, then unblock and set the status*/
            pseudoSys4->p_s.s_v0 = devStatus;
            insertProcQ(&readyQ, pseudoSys4);

            softBlock -= 1;
        }
    }

    else
    {
        /*nothing to unblock*/
        /*save the state because there's no where else*/
        saveState[deviceNumber] = devStatus;
    }

    /*if there is no currentProc*/
    if (currentproc == NULL)
    {
        scheduleProcess();
    }
}
