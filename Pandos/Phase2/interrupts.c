#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"
#include "/usr/local/include/umps3/umps/libumps.h"
#include "../h/exceptions.h"
#include "../h/initial.h"
#include "../h/scheduler.h"

/***********************************************************************************
*
* Thie file contains all methods and instruction for interrupts. This file contains 
* the instructions for trapH and deals with the the proper protocol once it is 
* passed to the proper response. This file contains the instructions for the 
* local timer(PLT), handling of Pseudo Clock Ticks, and IO handling as well as the 
* bitmapping and terminal input and output. 
*
/************************************************************************************/

/*externs*/

HIDDEN void plt(cpu_t time);
HIDDEN void pseudo();
HIDDEN void IOHandler(int lineNum);




void trapH(){
    /**This method to do the specific actions when an interrupt occurs and what caused it*/

    /*local variables*/
    cpu_t stop; /*relates to end of the time*/
    cpu_t quantum;
    
    /*read the stop clock*/
    STCK(stop);

    /*gets the quantum timer*/
    quantum = getTIMER();

    /*determines the cause of interrupt*/

    if (((((state_t *)BIOSDATAPAGE)->s_cause) & PLTINTERRUPT) != 0){

        /*local timer interrupt*/
        plt(stop);
    }

    else if (((((state_t *)BIOSDATAPAGE)->s_cause) & PSEUDOCLOCKINT) != 0)
    {
        /*timer interrupt*/
        pseudo();
    }

    else if (((((state_t *)BIOSDATAPAGE)->s_cause) & DISKINTERRUPT) != 0)
    {
        /*disk interrupt*/
        IOHandler(DISKINT);
    }

    else if (((((state_t *)BIOSDATAPAGE)->s_cause) & FLASHINTERRUPT) != 0)
    {
        /*flash interrupt*/
        IOHandler(FLASHINT);
    }

    else if (((((state_t *)BIOSDATAPAGE)->s_cause) & PRINTINTERRUPT) != 0)
    {
        /*print interrupt*/
        IOHandler(PRNTINT);
    }

    else if (((((state_t *)BIOSDATAPAGE)->s_cause) & TERMINALINTERRUPT) != 0)
    {
        /*terminal interrupt*/
        IOHandler(TERMINT);
    }

    /*if the currentproc is not NULL*/
    if (currentproc != NULL){

        /*get the time*/
        currentproc->p_time += (stop - startTOD);

        /*call move state and move the BIOSDATAPAGE to the currentproc's*/
        moveState((state_t *)BIOSDATAPAGE, &(currentproc->p_s));

        /*call intervalswitch for quantum clock*/
        intervalSwitch(quantum);
    }

    else
    {

       /*if no currentproc, then halt*/
        HALT();
    }

}

void plt(cpu_t timeStop){
    /**This function handles the procedures dealing with cpu and if the cpu will generates
     **any sort of clock interrupts. The function is given an end time*/

    /*if no currentproc, panic*/
    if (currentproc == NULL){
        PANIC();
    }

    /*calculate the time*/
    currentproc->p_time += (timeStop - startTOD);
    /*call movestate for BIOSDATATPAGE and move it to currentproc's s register*/
    moveState((state_t *)BIOSDATAPAGE, &(currentproc->p_s));

    /*insert it*/
    insertProcQ(&readyQueue, currentproc);

    /*set the currentproc to NULL*/
    currentproc = NULL;

    /*schedule next*/
    scheduleProcess();
}

void pseudo(){
    /**This function handles the pseudo clock ticks. It will perform a v (sys 4) operation
     **evert 100 millisecond to maintain a semaphore list called the pseudo clock*/

    /*local variable*/
    pcb_t *removedPcbs; /* pcb that is being removed*/


    LDIT(PSEUDO);/*reset psuedoc lock*/

    /*sets the removedPcbs to the removed block with the address of the device*/
    removedPcbs = removeBlocked(&(devices[DEVPERINT + DEVCNT]));

    /*remove it*/
    while (removedPcbs != NULL){
        /*need to place it on ready queue*/
        insertProcQ(&readyQueue, removedPcbs);

        /*decrease soft block count*/
        softBlock -= 1;

        /*remove it*/
        removedPcbs = removeBlocked(&(devices[DEVPERINT + DEVCNT]));
    }

     /*reset psuedo to 0*/
    devices[DEVPERINT + DEVCNT] = 0;

    /*check there is no currentproc, call scheduleProcess*/
    if (currentproc == NULL){

        /*schedule it*/
        scheduleProcess();
    }
}


void IOHandler(int lineNum){
    /**This Function will handle any devices that are going to interrupt using 
     **a bitmap that will point to correct device and the correct response to that
     **device. This also contains the special intructions related to the terminal. The
     **function is given a lineNum which is based off a constant associated with trapH */

    /*Local Variables*/

    int found; /*gives the status of a device*/
    pcb_t *temp; /*pcb*/
    int deviceNum; /*gives device number*/
    unsigned int bitMap; /*bit map*/
    unsigned int devStatus; /*device status*/
    volatile devregarea_t *devReg; /* terminal device register*/
    volatile devregarea_t *deviceRegister; /*device register*/

    /*set found to FALSE until changed*/
    found = FALSE; 

    /*establish addressing*/
    deviceRegister = (devregarea_t *)RAMBASEADDR;
    bitMap = deviceRegister->interrupt_dev[(lineNum - DISKINT)];

    /*if the bitMap has nothing in it, panic*/
    if ((&(bitMap) == NULL) & !(found)){
        PANIC();
    }

    /*see which device is on, bitmapping*/
    if (((bitMap & DEVICE0) != 0) & !(found) ){
        /*set the device number to the proper device*/
        deviceNum = 0;

        /*set this to True when you find it*/
        found = TRUE; 
    }

    if (((bitMap & DEVICE1) != 0) & !(found)){
        deviceNum = 1;
        found = TRUE;
    }

    if (((bitMap & DEVICE0) != 0) & !(found)){
        deviceNum = 2;
        found = TRUE;
    }

    if (((bitMap & DEVICE0) != 0) & !(found)){
        deviceNum = 3;
        found = TRUE;
    }

    if (((bitMap & DEVICE0) != 0) & !(found)){
        deviceNum = 4;
        found = TRUE;
    }

    if (((bitMap & DEVICE0) != 0) & !(found)){
        deviceNum = 5;
        found = TRUE;
    }

    if (((bitMap & DEVICE0) != 0) & !(found)){
        deviceNum = 6;
        found = TRUE;
    }

    if (((bitMap & DEVICE0) != 0) & !(found)){
        deviceNum = 7;
        found = TRUE;
    }

    /*determine sem for device*/
    deviceNum += ((lineNum - DISKINT) * DEVPERINT);

    /*case for terminal*/
    if (lineNum == TERMINT){
        /*check to see if terminal is transmit or receive*/ 

        /*set devReg*/

        devReg = (devregarea_t *)RAMBASEADDR;

        /*get status from registers*/
        devStatus = devReg->devreg[(deviceNum)].t_transm_status;

        if ((devStatus & BITS) != TRUE){

            /*ack the recieve*/
            devReg->devreg[(deviceNum)].t_transm_command = ACK;
        }

        else
        {
            devStatus = devReg->devreg[(deviceNum)].t_recv_status; /*copy the status*/
            devReg->devreg[(deviceNum)].t_recv_command = ACK; /*ACK the interrupt*/

            /*increment the device sema4 by DEVPERINT*/
            deviceNum += DEVPERINT;
        }
    }

    else{

        devStatus = (deviceRegister->devreg[deviceNum]).d_status; /*copy the status*/
        (deviceRegister->devreg[deviceNum]).d_command = ACK;      /*ACK the interrupt*/
    }

    /*increase the device amount*/
    devices[deviceNum] += 1;

    /*done waiting for IO, unblock pcb */
    if (devices[deviceNum] <= 0){
        temp = removeBlocked(&(devices[deviceNum]));

        if (temp != NULL)
        {
            /*if there is a process, then unblock and set the status*/
            temp->p_s.s_v0 = devStatus;

            /*insert*/
            insertProcQ(&readyQueue, temp);

            /*decrease soft block count*/
            softBlock -= 1;
        }
    }

    else{
        /*nothing to unblock*/

        /*save the state because there's no where else*/
        saveState[deviceNum] = devStatus;
    }

    /*if there is no currentProc*/
    if (currentproc == NULL){

        /*schedule*/
        scheduleProcess();
    }
}
