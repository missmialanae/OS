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

HIDDEN pcb_t *pcbFree_h;
HIDDEN semd_t *semd_h, *semdFree_h;


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
	
}

int insertBlocked(int *semAdd, pcb_t *p){

	/* insert the pcb pointed ot by p at the tail of th eprocess queue associated with the semaphore whose 
	physical address is semAdd address of p to semAdd. If the sempahore is not active return NULL, allocate 
	a new descriptor from the semdFree list, insert it in the ASL, initialize all of the fields. Return true 
	and false */

	semd_t *temp = findsem(semAdd); /* finding the semaddress*/
	if(temp->s_next->s_semAdd == semAdd){
		insertProcQ(&(temp->s_procQ),p);
		return FALSE; 
	}

	if(semdFree_h == NULL){
		return TRUE; /*list is empty*/
	}
	semd_t *newSem = semdFree_h;
	semdFree_h = newSem->s_next;
	newSem->s_next = NULL;

	temp = findsem(newSem->s_semAdd);
	temp->s_next = newSem->s_next;
	temp->s_next = newSem;
	insertProcQ(newSem->s_procQ,p);
	return /*TRUE OR FALSE*/; 
}

pcb_t *removeBlocked(int *semAdd){
	semd_t *temp = findsem(semAdd);
	if(temp->s_next->s_semAdd == semAdd){
		removeProcQ(temp->s_next->s_procQ);


		if(emptyProcQ(temp->s_next->s_procQ)){
			semd_t *remove = temp->s_next;
			temp->s_next = remove->s_next;
			remove->s_next = NULL;
			remove->s_next = semdFree_h;
			semdFree_h->s_next = remove; 
		}

		return; 
	}
	return NULL; 
}

pcb_t *outBlocked(pcb_t *p){
semd_t *temp = findsem(p->p_semAdd);
	if(temp->s_next->s_semAdd == p->p_semAdd){
		outProcQ(temp->s_next->s_procQ, p);


		if(emptyProcQ(temp->s_next->s_procQ)){
			semd_t *remove = temp->s_next;
			temp->s_next = remove->s_next;
			remove->s_next = NULL;
			remove->s_next = semdFree_h;
			semdFree_h->s_next = remove; 
		}

		return; 
	}
	return NULL; 

}

pcb_t *headBlocked(int *semAdd){
	/* return to the pcb that is at the head of the process queue */

	semd_t *temp = findsem(semAdd);/*creating a pcb to hold sem add*/ 

	if(temp->s_next->s_semAdd != semAdd){
		return NULL;
	}
	if(emptyProcQ(temp->s_next->s_procQ)){
		return NULL;
	}
	return headProcQ(temp->s_next->s_procQ); 

}/* headblocked */

int findsem(int *semAdd){

	semd_t *temp = semd_h;

	while(temp->s_next->s_semAdd < semAdd){
		temp = temp->s_next;
	}

	return temp; 
}
/***************END***********************/