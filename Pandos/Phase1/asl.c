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

	//creating dummy nodes
	pcb_t *temp = p; 
	semd_t *tempS;


	if(emptyProcQ(s_ProcQ)){
		return NULL; // list is empty
	}

	/***** You are the only one in the list*****/
	if(semfree_h->s_next = NULL){ // if free list points to nothing 
		tempS->s_semAdd = findsem(semAdd)
		sem_h->s_next = tempS;
		temps->s_next = NULL; 

	}
	//make sure the semAdd is not being used
	//while(temp->s_next->s_semAdd)

	/*****inserting but it is not the only one****************/
	if(semfree_h->s_next != NULL){ // if it is pointing to something
		semd_t *next = semfree_h->s_next; //making a dummy node to store the old next
		semfree_h->s_next = tempS; // tempS is now the new next
		temps->s_next = next; 
		insertProcQ(p);

	}

	return headProcQ(tempS);
}

pcb_t *removeBlocked(int *semAdd){

	temp->s_semAdd = *semAdd; //setting the semAdd to a pcb
	while(temp ->s_next->s_semAdd){
		temp = s->s_next; 
	}
	
	findsem(semAdd);
	return outProcQ(temp); 

}

pcb_t *outBlocked(pcb_t *p){


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

	if(temp->s_semAdd != semAdd ){// temp doesn't have the semAdd
		temp = temp->s_next; //run through the list 
	}

	//if you find the semAdd

	temp->s_semAdd = semAdd// if you find semAdd
	return(temp); //return the temp 

	return NULL; //semAdd is not there 
}

/***************END***********************/