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
	pcb_T 		  *s_ProcQ;  /* tail pointer to a */
				  			 /*process queue */
} semd_t; 

intASL(){

	static semd_t semdTable[MAXPROC+2]; 
	semFree_h;
	sem_h->s_ProcQ = mkemptyProcQ();
	sem_h->s_next == NULL;
	sem_h->semAdd == NULL; 
	sem_h = semdTable[MAXPROC];

}

int insertBlocked(int *semAdd, pcb_t *p){

}

pcb_t *removeBlocked(int *semAdd){

}

pcb_t *outBlocked(pcb_t *p){

}
pcb_t headBlocked(int *semAdd){
	// return to the pcb that is at the head of the process queue 

}