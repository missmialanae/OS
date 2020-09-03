//Including the .h files

#include "../h/const.h"
#include "../h/types.h"
#include "../h/pcb.h"
#include "../h/asl.h"

//global pointer for free list
HIDDEN pcb_t * pcbfree_h;

/********************Allocation and Deallocation*****************/
initPcb(){
	static pcb_t foo[MAXPROC];
	pcbfree_h = NULL;

	for(int i = 0; i < MAXPROC; i++){
		freepcb(& foo[i]);
	}

}//initPcb

void freePcb (pcb_t *p){

	insertProcQ(*p);

}// freePcb

pcb_t *allocPcb (){
	//remove an element from the pcbfree list

	//check to see if the pcbfree list is empty 
	if (pcbfree_h == NULL){
		return NULL;
	}

	else{ //set the pcb and give it's information 

		//make sure it's not being used
		pcb_t p = NULL // that pcb is empty 

		//set the information to the pcb
			p->p_next = NULL; 
			p->p_prev = NULL;
			p->semAdd = NULL;
			//p->s_next = NULL;
			//p->ProcQ = NULL;


	}//allocPcb

	return; 
}


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

insertProcQ (pcb_t **tp, pcb_t *p){

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
		(*tp)-> p_next = head;

		//set p to be the new tp
		tail = p; 

		//set tp's p_next to the pointer 
		(*tp)->p_next = p;

		//setting the p pointer to the head dummy node
		p-> p_next = head; 

		head->p_next = head

		(*tp)->p_next = p; 

	}


}// insertProcQ

pcb_t removeProcQ (pcb_t *tp){

	//removes the head from the queue
	if(emptyProcQ(tp)){
		return NULL; // list is empty 
	} 

	if((*tp)->p_next =(*tp) ){ // If the only one in the queue
		//set the whole thing to NULL
		(*tp)->p_prev = NULL; 
		(*tp)>p_next = NULL;
		(*tp)= NULL; 

		//put back on to freelist
		freepcb(tp);
	}

	/******** What if I am not the only one in the queue******/

	//dummy nodes
	pcb_t *head = tp->p_next; // create a dummy node to access the head
	pcb_t *newHead = head->p_next; // access the node that is acually next to be head 
	pcb_t*temp; //so we don't delete the head 

	//resetting the tp
	(*tp)->p_next = newHead; 

	//resetting the old head's relationship
	head-> p_next= NULL;
	head->p_prev = NULL;

	// reset the pointer for the new head
	newHead->p_prev = (*tp);

	//set the old head to a temp node
	temp = head; 

	//freepcb to add her to the freelist 
	freePcb(temp);

	//return 
	return headProcQ(**tp);

}// removeProcQ

pcb_t outProcQ (pcb_t **tp, pcb_t *p){
	// remove pcb from the middle of the queue; we want to delete p

	//dummy nodes 
	pcb_t *temp = p;
	pcb_t *head = tp->p_next; 

	//traverse the list
	temp->p_next->p_prev = temp->p_prev;
	temp->p_prev->p_next = temp->p_next;

	//if node is not there
	if(emptyProcQ(*tp)){
		return NULL;
	}

	//if p is the head of the queue
	//if p is the head call removeProcQ and reset pointers
	//if p is not then traverse through the list until you find p
	//one you find p call removeProcQ and reset pointer

	//Return outprocQ
	return outProcQ(tp);

}

/*********************** PROCESS TREE MAINTENANCE ***********************/

int emptyChild(pcb_t *p){
	//return a true value if the pcb pointed to by p has no children
	return (p == NULL);
}// emptyChild

void insertChild(pcb_t *prnt, pcb_t *p){
	//make the pcb pointed ot by p a child of the pcb pointed to by prnt

	//dummy nodes cause we stan 
	

	//insert at the beggining for constant 

	//Check to see if the tree is empty 
	if(emptyChild(p)){
		return NULL;
	}
	if(prnt->p_child = NULL){// if 
		prnt->p_child = p; // setting the parent to the child 
	}

	/***** What if the parent has a child*****/



	//parent point to new child 

   
}// insertChild

	

pcb_t *removeChild (pcb_t *p){
	//we are given a parent 

	if(emptyChild(p)){ //if tree is empty, return null
	return NULL;
	}

	if(p->child->p_sib == NULL){ //there is only one child
		p->p_child = NULL; 
		prnt->p_child = NULL; 
	}

	//setting dummhy nodes to prevent confusion
	p->p_child = oldchild;
	pcb_t *parent = oldchild->p_prnt;
	pcb_t *newChild = oldchild->p_sib;

	//stops next sib from pointing
	newChild->p_sib = NULL;
	//set parent first child as new sibling
	parent->p_sib = newchild;
	//make p have no siblings
	oldchild->p_sib = NULL;
	oldChild->p_prnt = NULL; 

	return; 

} // removeChild

pcb_t *outChild(pcb_t *p){
	//p point to the child 
	pcb_t *parent = p->p_prnt;


	if(emptyChild(p)){ //checks if tree is empty; 
 		return NULL;
 	}
 	//you have a parent but they don't point back to you; not first child

 	//middle child 

 	//last child


 	//if my parent doesn't point back to me I am not the first child
 	if(parent->child != p){
 		p->p_prnt = NULL; // kill the relationship 
 		p-p_sibprev
 	}

 	//p has no parent
 	//p is the parent's first child 

}// outChild

/************************END*******************************/