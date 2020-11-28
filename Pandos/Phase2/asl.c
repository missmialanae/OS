/*including all necessary files*/
#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"

/**************************************************************************** 
 * 
 *  This file contains all methods and instruction for ASL. 
 * 
 ****************************************************************************/

/*semaphore descriptor type */
typedef struct semd_t {
    struct semd_t *s_next;   /*next element on the ASL */
    int           *s_semAdd; /*pointer to the sempahore */
    pcb_t         *s_procQ;  /* tail pointer to a */
                         /*process queue */
} semd_t; 

/*GLOBALS*/
HIDDEN semd_t *semd_h, *semdFree_h;

/*defining local variables */
HIDDEN semd_t* findsem(int *semAdd);
HIDDEN semd_PTR allocation(int *sem);  
HIDDEN void backToFreeList(semd_t *oldSem);

void initASL(){
    /*initialize the semdFree list to contain all the elements 
    of the array called only once*/ 

    int i;

    static semd_t semdTable[MAXPROC+2]; /*plus 2 for the dummy nodes*/ 
    semdFree_h = &semdTable[0]; 

    for (i=1; i< MAXPROC; i++){
        semdTable[i-1].s_next = &semdTable[i];
    }

    /*setting boundaries and fields */
    semdTable[MAXPROC - 1].s_next = NULL; 
    semd_h = &semdTable[MAXPROC];
    semd_h->s_semAdd = 0;
    semd_h->s_procQ = mkEmptyProcQ();
    semd_h->s_next = &semdTable[MAXPROC+1];
    semd_h->s_next->s_semAdd = MAXINT; /*make int is infinity*/
    semd_h->s_next->s_procQ = mkEmptyProcQ(); 
    semd_h->s_next->s_next = NULL; 

    return;
}/*initASL*/

int insertBlocked(int *semAdd, pcb_t *p){

    /* insert the pcb pointed to by p at the tail of the process queue associated with the semaphore whose 
    physical address is semAdd address of p to semAdd. If the sempahore is not active: allocate 
    a new descriptor from the semdFree list, insert it in the ASL, initialize all of the fields. Return true 
    and false */

    /*if a new semaphore descriptor needs to be allocated and the semdFree list is empty, return true*/

     semd_PTR previous = findsem(semAdd); /*dummy pointer to insert location in ASL*/

    if (previous->s_next->s_semAdd == semAdd){
        p->p_semAdd = semAdd;

        /*insert p into Q in ASL*/
        insertProcQ(&(previous->s_next->s_procQ), p); 

        return FALSE;
    }

    /*if we don't find, allocate*/

    if (semdFree_h == NULL){
       
        return TRUE;
    }

    /*remove semd from Free list and add to ASL, then and insert pcb into new semd */

    semd_t *newSem = allocation(semAdd); 
    semd_t *next = previous->s_next;  

    previous->s_next = newSem;
    p->p_semAdd = semAdd;
    newSem->s_next = next;

    insertProcQ(&(newSem->s_procQ), p);

    return FALSE;

}/*insertBlocked*/

pcb_t* removeBlocked(int *semAdd){
    /*Search the ASL for a descriptor of this semaphore. If none is found, return NULL; otherwise, remove the head
    pcb from the process queue of the found semaphore descriptor and return a pointer to it. If the process queue 
    for this semaphore becomes empty (emptyProcQ(s procq) is TRUE), remove the semaphore descriptor from 
    the ASL and return it to the semdFree list. */

    /*search for the semAdd*/
    semd_t *temp = findsem(semAdd);
    if(temp->s_next->s_semAdd == semAdd){
        pcb_t *removedPCB = removeProcQ(&(temp->s_next->s_procQ)); /*dummy node to return at end*/
        if(emptyProcQ(temp->s_next->s_procQ) == TRUE){
            back(temp);
        }

        return removedPCB; 
    }
    return NULL; 
}/*removeBlocked*/

pcb_t* outBlocked(pcb_t *p){
    /* Remove the pcb pointed to by p from the process queue associated with p’s semaphore (p→ p semAdd) on the ASL. 
    If pcb pointed to by p does not appear in the process queue associated with p’s semaphore, which is an error condition, r
    eturn NULL; otherwise, return p. */

    semd_t *temp = findsem(p->p_semAdd);
    if(temp->s_next->s_semAdd == p->p_semAdd){
        pcb_t *outPCB = outProcQ(&(temp->s_next->s_procQ), p); /*dummy node to call at end */
        if(emptyProcQ(temp->s_next->s_procQ)){
            back(temp);
        }

        return outPCB; 
    }
    /*error condition*/
    return NULL; 
}/*outBlocked*/

pcb_t* headBlocked(int *semAdd){
    /* return to the pcb that is at the head of the process queue associated to that semAdd */

    semd_t *temp = findsem(semAdd);/*creating a pcb to hold sem add*/ 

    if(temp->s_next->s_semAdd == semAdd){
        /*process queue is empty return NULL*/
        if(emptyProcQ(temp->s_next->s_procQ)){
            return NULL;
        }
        return headProcQ(temp->s_next->s_procQ); 
    }
    /* return NULL if semAdd is not found in ASL*/

    return NULL;

}/* headblocked */

semd_t* allocation(int *sem){

    /*hopefully this will fix the issues with ASL*/

    semd_t *new = semdFree_h;
    semdFree_h = new->s_next;
    new->s_next = NULL;
    new->s_semAdd = sem;
    new->s_procQ = mkEmptyProcQ();

    return new; 
}

void back(semd_t *oldSem){

    /*fix asl issues for phase 2*/
    semd_t *temp;

    temp = oldSem->s_next;

    oldSem->s_next = temp->s_next;

    temp->s_next = semdFree_h;

    semdFree_h = temp;
}

semd_t* findsem(int *semAdd){
    /* find the semAdd that is being called need. Basically a search function to search for semAdds*/

    semd_t *temp = semd_h;

    while(temp->s_next->s_semAdd < semAdd){
        temp = temp->s_next;
    }

    return temp; 
    
}/*findsem*/

/***************END***********************/