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
	       
       if(emptyChild(p_Child)){ //check if tree is empty
       	return NULL;
       }
        if(pcb_PTR *p = temp){
        	temp -> p_leftChild = new p_leftChild; //put new child in a new node with temp 
        	p_leftChild = p_next; //insert child
        	freepcb(temp); //free temp
        }

        if(pcb_PTR * p_Child = temp){
        	temp -> p_rightChild = new p_rightChild; //puts new right child in new node with temp
        	p_rightChild = p_next; //insert child
        	freepcb(temp); //free temp
        }

}// insertChild

pcb_t *removeChild (pcb_t *p){
		if(emptyChild (p_Child )){ //if tree is empty, return null
		return NULL;
	}

	if(pcb_PTR *p = p_next){ 
	pcb_PTR -> temp = p_leftChild; //set temp to left child 
	p_leftChild = NULL; //delete the left child 
	freepcb(temp); //free the temp

	else if(pcb_PTR * p = p_next){
		pcb_PTR -> temp = p_rightChild; // set temp to right child 
		p_rightChild = NULL; //delete the right child 
		freepcb(temp); //free the temp 
	}
	

	}

} // removeChild

pcb_t *outChild(pcb_t *p){
	if(emptyChild(p)){ //checks if tree is empty
 		return NULL;

 		//other code to remove a middle child in tree
 	}
 	
	}// outChild

/************************END*******************************/