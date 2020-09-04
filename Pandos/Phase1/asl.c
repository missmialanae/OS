#include "../h/const.h"
#include "../h/types.h"
#include "../h/asl.h"
#include "../h/pcb.h"

HIDDEN pcb_t *pcbFree_h;
HIDDEN semd_t *semd_h, *semFree_h;

/*semaphore descriptor type */
typedef struct semd_t{
	struct semd_t *s_next;   /*next element on the ASL */
	int 		  *s_semAdd; /*pointer to the sempahore */
	pcb_t		  *s_ProcQ;  /* tail pointer to a */
				  			 /*process queue */
} semd_t; 

intASL(){
	//initialize the semdFree list to contain all the elements of the array 

	static semd_t semdTable[MAXPROC+2]; //plus 2 for the dummy nodes 
	semFree_h = & semdTable [0]; 
	sem_h->s_ProcQ = mkemptyProcQ();
	sem_h->s_next == NULL;
	sem_h->semAdd == NULL; 
	sem_h = semdTable[MAXPROC];

	for(int i = 0; i < MAXPROC; i++){
		
	}

}

int insertBlocked(int *semAdd, pcb_t *p){
	//insert the pcb pointed to by p at the tail of the process queue 
	//inserts a new element into the queue

	semd_t *temp = findsem(semAdd); // finding the semaddress
	if(temp->s_next->s_semAdd == semAdd){
		insertProcQ(p);
		return p; 
	}

	if(semdfree_h == NULL){
		return NULL; // list is empty
	}
	semd_t *newSem = semdfree_h;
	semdfree_h = newSem->s_next;
	newSem->s_next = NULL;

	temp = find(newSem->s_semAdd);
	temp->s_next = newSem->s_next;
	temp->s_next = newSem;
	InsertProcQ(newSem->s_ProcQ,p);
	return newSem; 
}

pcb_t *removeBlocked(int *semAdd){
	semd_t *temp = findsem(semAdd);
	if(temp->s_next->semAdd == semAdd){
		removeProcQ(temp->s_next->s_ProcQ);


		if(emptyProcQ(temp->s_next->s_ProcQ)){
			semd_t *remove = temp->s_next;
			temp->s_next = remove->s_next;
			remove->s_next = NULL;
			remove->s_next = semdfree_h;
			semdfree_h->s_next = remove; 
		}

		return; 
	}
	return NULL; 
}

pcb_t *outBlocked(pcb_t *p){
semd_t *temp = findsem(p->p_semAdd);
	if(temp->s_next->semAdd == p->p_semAdd){
		outProcQ(temp->s_next->s_ProcQ, p);


		if(emptyProcQ(temp->s_next->s_ProcQ)){
			semd_t *remove = temp->s_next;
			temp->s_next = remove->s_next;
			remove->s_next = NULL;
			remove->s_next = semdfree_h;
			semdfree_h->s_next = remove; 
		}

		return; 
	}
	return NULL; 

}

pcb_t headBlocked(int *semAdd){
	// return to the pcb that is at the head of the process queue 

	pcb_t *temp;// creating a pcb to hold sem add; 
	temp->s_semAdd = *semAdd; //setting the semAdd to a pcb
	while(temp ->s_next->s_semAdd){
		temp = sem->s_next; 
	}

	return headProcQ(temp); 

}// headblocked 

int findsem(int *semAdd){

	//dummy nodes 
	pcb_t *temp = semd_h; 

	if(temp->s_next->semAdd < semAdd ){// temp doesn't have the semAdd
		temp = temp->s_next; //run through the list 
	}

	//if you find the semAdd

	temp->s_semAdd = semAdd// if you find semAdd
	return(temp); //return the temp 

	return NULL; //semAdd is not there 
}

/***************END***********************/