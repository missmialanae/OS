#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"

/*semaphore descriptor type */
typedef struct semd_t {
	struct semd_t *s_next;   /*next element on the ASL */
	int 		  *s_semAdd; /*pointer to the sempahore */
	pcb_t		  *s_procQ;  /* tail pointer to a */
				   		 /*process queue */
} semd_t; 


/*semd_h points to active list*/
/*semdFree_h points to free list*/

HIDDEN semd_t *semd_h, *semdFree_h;

HIDDEN semd_t* findsem(int *semAdd);

void initASL(){
	/*initialize the semdFree list to contain all the elements of the array*/ 

	int i;

	static semd_t semdTable[MAXPROC+2]; /*plus 2 for the dummy nodes*/ 
	semdFree_h = &semdTable[0]; 

	for (i=1; i< MAXPROC; i++){
		semdTable[i-1].s_next = &semdTable[i];
	}

	semdTable[MAXPROC - 1].s_next = NULL; 
	semd_h = &semdTable[MAXPROC];
	semd_h->s_semAdd = 0;
	semd_h->s_procQ = mkEmptyProcQ();
	semd_h->s_next = &semdTable[MAXPROC+1];
	semd_h->s_next->s_semAdd = MAXINT; 
	semd_h->s_next->s_procQ = mkEmptyProcQ(); 
	semd_h->s_next->s_next = NULL; 

	return;
	
}

int insertBlocked(int *semAdd, pcb_t *p){

	/* insert the pcb pointed to by p at the tail of the process queue associated with the semaphore whose 
	physical address is semAdd address of p to semAdd. If the sempahore is not active: allocate 
	a new descriptor from the semdFree list, insert it in the ASL, initialize all of the fields. Return true 
	and false */

	/*if a new semaphore descriptor needs to be allocated and the semdFree list is empty, return true*/

	semd_t *temp = findsem(semAdd); /* finding the semaddress*/

	
	if(temp->s_next->s_semAdd == semAdd){/* found the semAdd in the list */
		p->p_semAdd = semAdd; 
		insertProcQ(&(temp->s_next->s_procQ),p);
		return FALSE; 
	}

	/*if a new semaphore descriptor needs to be allocated and the semdFree list is empty, meaning that we don't have any semaphores
	return true*/
	debugB(3);
	if(semdFree_h == NULL){
		debugB(4);
		return TRUE;

	}

	/*if the list is not empty and therefore I can use them*/

	debugB(5);

	/*so we remove from the top of the stack therefore we have to set the next asl to be the new pointer ot the head*/
	semd_t *newSem = semdFree_h;
	semdFree_h = newSem->s_next;

	/*this allows us to remove it from free list and allocate the stuff for it */
	newSem->s_procQ = mkEmptyProcQ();
	newSem->s_semAdd = semAdd; 
	newSem->s_next = NULL;

	/*how do we make sure it is on the active list*/
	temp = findsem(newSem->s_semAdd);
	newSem->s_next = temp->s_next;
	temp->s_next = newSem;
	p->p_semAdd = semAdd;
	insertProcQ(&(newSem->s_procQ),p);

	
	return FALSE; 
	

}

pcb_t* removeBlocked(int *semAdd){
	/*Search the ASL for a descriptor of this semaphore. If none is found, return NULL; otherwise, remove the head
	pcb from the process queue of the found semaphore descriptor and return a pointer to it. If the process queue 
	for this semaphore becomes empty (emptyProcQ(s procq) is TRUE), remove the semaphore descriptor from 
	the ASL and return it to the semdFree list. */

	/*search for the semAdd*/
	semd_t *temp = findsem(semAdd);
	if(temp->s_next->s_semAdd == semAdd){
		pcb_t *removedPCB = removeProcQ(&(temp->s_next->s_procQ));
		if(emptyProcQ(temp->s_next->s_procQ) == TRUE){

			semd_t *remove = temp->s_next;
			temp->s_next = remove->s_next; 
			remove->s_next = NULL;
			remove->s_next = semdFree_h;
			semdFree_h = remove;
		}

		return removedPCB; 
	}
	return NULL; 
}

pcb_t* outBlocked(pcb_t *p){
	/* Remove the pcb pointed to by p from the process queue associated with p’s semaphore (p→ p semAdd) on the ASL. 
	If pcb pointed to by p does not appear in the process queue associated with p’s semaphore, which is an error condition, r
	eturn NULL; otherwise, re- turn p. */

	semd_t *temp = findsem(p->p_semAdd);
	if(temp->s_next->s_semAdd == p->p_semAdd){
		pcb_t *outPCB = outProcQ(& (temp->s_next->s_procQ), p);

		if(emptyProcQ(temp->s_next->s_procQ)){
			semd_t *remove = temp->s_next;
			temp->s_next = remove->s_next;
			remove->s_next = NULL;
			remove->s_next = semdFree_h;
			semdFree_h->s_next = remove; 
		}
		return outPCB; 
	}

	return NULL; 
}

pcb_t* headBlocked(int *semAdd){
	/* return to the pcb that is at the head of the process queue */

	semd_t *temp = findsem(semAdd);/*creating a pcb to hold sem add*/ 

	if(temp->s_next->s_semAdd == semAdd){
		if(emptyProcQ(temp->s_next->s_procQ)){
			return NULL;
		}
		return headProcQ(temp->s_next->s_procQ); 
	}

	return NULL;

}/* headblocked */

semd_t* findsem(int *semAdd){
	/* find the semAdd that is being called need. Basically a search function*/

	semd_t *temp = semd_h;

	while(temp->s_next->s_semAdd < semAdd){
		temp = temp->s_next;
	}

	return temp; 
}

/***************END***********************/