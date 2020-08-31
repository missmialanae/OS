//Including the .h files

#include "../h/const.h"
#include "../h/types.h"
#include "../h/pcb.h"

//global pointer for free list
HIDDEN pcb_t * pcbfree-h;

/********************Allocation and Deallocation*****************/
initPcb(){
	static pcb_t foo[MAXPROC];
	pcbfree_h = NULL;

	for(int i = 0; i < MAXPROC; i++){
		freepcb(& foo[i]);
	}

}//initPcb

void freePcb (pcb_t *p){
	// insett the element pointed to by p onto the pcbFree list

}// freePcb

pcb_t *allocPcb (){
	//remove an element from the pcbfree list

	//check to see if the pcbfree list is empty 
	if (pcbfree_h == NULL){
		return NULL;
	}else{
		//remove the element from the list portion 

		//
	}

}//allocPcb


/********************QUEUE MAINTENTANCE***************************/

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

	//set head to temp node
	//delete the temp node 
	//free pcb
	//reset pointers

	pcb_t *head = tp->p_next;
	pcb_t temp = head;
	head = p_next;

	//add her to the freelist
	freepcb(temp);

}// removeProcQ

pcb_t outProcQ (pcb_t **tp, pcb_t *p){
	// remove pcb from the middle of the queue

	//doubly linked and circular queue 

	//check queue
	if(emptyProcQ(tp)){
		return NULL;
	}

}

/*********************** PROCESS TREE MAINTENANCE ***********************/
int emptyChild(pcb_t *prnt){
	//return a true value if the pcb pointed to by p has no children
	return (p_child == NULL);

}// emptyChild

void insertChild(pcb_t *prnt, pcb_t *p){
	//makes the pcb pointed to by p a child of the pcb pointed to by prnt 
	// insert first child for constant 

	//dummy nodes are created
	pcb_t *old = prnt -> child;

	//check tree
	if(emptyChild(prnt)){
		return NULL; //list is empty 
	}// list is not empty 

	//set parent pointer to point to new child 
	prnt ->child = p;

	//make the new child point to new sibling 
	p_sib = old; 

}// insertChild

pcb_t *removeChild (pcb_t *p){
	//makes the first child of the pcb pointed to by p no longer a child of p

	//dummy nodes
	//gonna set the old child's sibling to a pointer 
	pcb_t *new = p_sib;

	//check the tree
	if(emptyChild(prnt)){
		return NULL; //list is empty 
	}// list is not empty 

	//set the parent to new sibling 
	new = prnt_child;

	//add her to the freelist
	freepcb(p);

} // removeChild

pcb_t *outChild(pcb_t *p){
	//makes the pcb pointed to by p no longer the child of it parent 

	//check to see if the pcb has a parent 
	//if empty return NULL

	//if not empty 
}// outChild

/************************END*******************************/