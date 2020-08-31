//Including the .h files

#include "../h/const.h"
#include "../h/types.h"
#include "../h/pcb.h"

//global pointer for free list
HIDDEN pcb_t * pcbfree-h;
/********************Allocation and Deallocation*************/
initPcb(){
	static pcb_t foo[MAXPROC];
	pcbfree-h = NULL;

	for(int i = 0; i < MAXPROC; i++){
		freepcb(& foo[i]);
	}

}//initPcb

void freePcb (pcb_t *p){

}

pcb_t *allocPcb (){

}


/********************QUEUE MANAGEMENT***************************/

pcb_t mkemptyProcQ(){
	// makes the whole queue empty and returns a pointer to the tail of an empty process queue
	return NULL;
} //mkemptyProcQ

int emptyProcQ (pcb_t **tp){
	//returns a boolean value whether the queeue is empty or not 
	return (tp == NULL);
}// emptyProcQ

pcb_t *headProcQ(pcb_t *tp){

	//returns the head pcb in the queue
	if(emptyProcQ(tp)){
		return NULL; // list is empty 
	}
	return (tp -> p_next);
}// headProcQ

void insertProcQ (pcb_t **tp, pcb_t *p){

	//inserts a new element into the queue

	//creating dummy nodes
	pcb_t *head; 
	pcb_t *tail; 

	//set head to be the tp's p_next to head 

	tp-> p_next = head; //creating a dummy node
	tp = tail; //another dummy node
	if(emptyProcQ(tp)){
		return NULL; // list is empty
		
	}else{ // list is not empty 

		//set head to be the tp's p_next to head 
		tp-> p_next = head;

		//set p to be the new tp
		tail = p; 

		//set tp's p_next to the pointer 
		tp->p_next = p;

		//setting the p pointer to the head dummy node
		p-> p_next = head; 

	}


}// insertProcQ

pcb_t removeProcQ (pcb_t *tp){

	//removes the head from the queue
	if(emptyProcQ(tp)){
		return NULL; // list is empty 
	} 

	pcb_t *head = tp->p_next;
	pcb_t temp = head;
	head = p_next;
	freepcb(temp);

	//set head to temp node
	//delete the temp node 
	//free pcb
	//reset pointers

}// removeProcQ

pcb_t outProcQ (pcb_t **tp, pcb_t *p){
	// remove pcb from the middle of the queue
	//doubly linked and circular queue 

	//check queue
	if(emptyProcQ(tp)){
		return NULL;
	}


}






