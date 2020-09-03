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
	semFree_h;
	sem_h->s_ProcQ = mkemptyProcQ();
	sem_h->s_next == NULL;
	sem_h->semAdd == NULL; 
	sem_h = semdTable[MAXPROC];

}

int insertBlocked(int *semAdd, pcb_t *p){
	//insert the pcb pointed to by p at the tail of the process queue 
	/inserts a new element into the queue

	//creating dummy nodes
	pcb_t *temp = p; 

	if(emptyProcQ(s_ProcQ)){
		return NULL; // list is empty
	}

	/***** You are the only one in the list*****/

	//make sure the semAdd is not being used
	while(temp ->s_next->s_semAdd)

	/*****inserting at the tail****************/

}

pcb_t *removeBlocked(int *semAdd){

	pcb_t *temp;// creating a pcb to hold sem add; 
	temp->s_semAdd = *semAdd; setting the semAdd to a pcb
	while(temp ->s_next->s_semAdd){
		temp = s->s_next; 
	}

	return outProcQ(temp); 

}

pcb_t *outBlocked(pcb_t *p){

}
pcb_t headBlocked(int *semAdd){
	// return to the pcb that is at the head of the process queue 

	pcb_t *temp;// creating a pcb to hold sem add; 
	temp->s_semAdd = *semAdd; setting the semAdd to a pcb
	while(temp ->s_next->s_semAdd){
		temp = s->s_next; 
	}

	return headProcQ(temp); 
}// headblocked 