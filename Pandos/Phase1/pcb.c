/*Including the .h files*/

#include "../h/const.h"
#include "../h/types.h"
#include "../h/pcb.h"
#include "../h/asl.h"

/*global pointer for free list*/
HIDDEN pcb_t *pcbfree_h;

/********************Allocation and Deallocation*****************/
void initPcb(){
	int i; 

	static pcb_t foo[MAXPROC];
	pcbfree_h = NULL;

	for(i = 0; i < MAXPROC; i++){
		freePcb(& foo[i]);
	}

}/*initPcb*/

void freePcb (pcb_t *p){

	insertProcQ(&pcbfree_h,p);

}/*freePcb*/

pcb_t *allocPcb (){
	/*remove an element from the pcbfree list */

	/*check to see if the pcbfree list is empty*/ 
	if (pcbfree_h == (NULL)){
		return NULL;
} /*set the pcb and give it's information

	/make sure it's not being used*/
	pcb_t *p = NULL; /* that pcb is empty*/ 

	/*set the information to the pcb*/
	p->p_next = NULL; 
	p->p_prev = NULL;
	p->p_semAdd = NULL;
	p->p_next = NULL;
	/*p->s_ProcQ = mkEmptyProcQ();*/

	p = removeProcQ(&pcbfree_h);

	return p; 
} /*allocPcb*/


/********************QUEUE MAINTENTANCE***************************/

pcb_t *mkEmptyProcQ(){
	/* makes the whole queue empty and returns a pointer to the tail of an empty process queue*/
	return NULL;
} /*mkemptyProcq*/

int emptyProcQ (pcb_t *tp){
	/*returns a boolean value whether the queeue is empty or not */
	return (tp == (NULL));

}/*emptyProcQ*/

pcb_t *headProcQ(pcb_t *tp){

	/*returns the head pcb in the queue*/
	if(emptyProcQ(tp)){

		return NULL; 

		/*list is empty*/ 
	}

	return (tp -> p_next);

}/*headProcQ*/

void insertProcQ (pcb_t **tp, pcb_t *p){
	/*inserts a new element into the queue

	creating dummy nodes*/
	pcb_t *head = tp->p_next;

	/*creating a dummy node*/

	if(emptyProcQ(*tp)){
		return NULL; 

		/* list is empty*/
		
	}else{ /*list is not empty */

		/*set head to be the tp's p_next to head */
		(*tp)-> p_next = head;

		/*set tp's p_next to the pointer */
		(*tp)->p_next = p;

		/*setting the p pointer to the head dummy node*/
		p-> p_next = head; 

		head->p_next = head;

		(*tp)->p_next = p; 

	}

}/* insertProcQ */

pcb_t *removeProcQ (pcb_t **tp){
	/*removes the head from the queue*/

	if(emptyProcQ(tp)){
		return tp; 
		/*will not let me return NULL here*/

		/* list is empty */
	} 

	if(tp->p_next == tp ){ 
		/*If the only one in the queue */

		/*set the whole thing to NULL*/
		tp->p_prev = NULL; 
		tp->p_next = NULL;
		tp = (NULL); 

		/*put back on to freelist*/
		freePcb(tp);
		return tp; 
	}

	/******** What if I am not the only one in the queue******/

	/*dummy nodes*/
	pcb_t *head = tp->p_next; /*create a dummy node to access the head*/
	pcb_t *newHead = head->p_next; /*access the node that is acually next to be head */
	pcb_t *temp; /*so we don't delete the head */

	/*resetting the tp*/
	tp->p_next = newHead; 

	/*reset the pointer for the new head*/
	tp = newHead->p_prev;

	tp->p_next->p_prev = NULL; 

	/*set the old head to a temp node*/
	temp = head; 

	/*freepcb to add her to the freelist */
	freePcb(temp);

	return headProcQ(tp);
	/*returns the head pcb in the queue*/

	if(emptyProcQ(tp)){
		return NULL; /*list is empty */
	}

	return (tp->p_next);

}/*removeProcQ*/

pcb_t *outProcQ (pcb_t **tp, pcb_t *p){
	/*remove pcb from the middle of the queue; we want to delete p

	dummy nodes */
	pcb_t *temp = p; 
	pcb_t *head = tp->p_next; 

	/*if the tree is actually empty */
	if(emptyProcQ(tp)){
		return NULL;
	}

	/****** What if i am the only node *****/

	if(head == temp){ /*if I am the head node and what you are looking for */
		removeProcQ(temp); /*call removeProcQ*/
	}

	/******* located in the middle *********/

	/*traverse the list*/
	if(temp->p_next != p){/*if temp is not p*/

		/*transverse through the list*/
		temp->p_next->p_prev = temp->p_prev;
		temp->p_prev->p_next = temp->p_next;

		/*find p*/
		if(temp->p_next == p){
			temp->p_next = temp->p_prev;
			temp->p_next = NULL; 
			temp->p_prev = NULL;
			freePcb(temp);
			return temp;
		}
	}

	if(tp == temp){
		pcb_t *temp = tp; /*temp to the tp */
		pcb_t *tail = temp->p_prev; /*dummhy nod that set new tail */

		tail->p_next = head; /*pointing to head */
		head->p_prev = tail; /*head point to new tail */

		/*getting rid of the pointers*/
		temp->p_prev = NULL;
		temp->p_next = NULL;

		/*resetting the tp*/
		tail = tp; 
		
		/*put temp back onto free list */
		freePcb(temp);

		return temp;
	}

	/*Return */
	return NULL; 
}

/*********************** PROCESS TREE MAINTENANCE ***********************/

int emptyChild(pcb_t *p){
	/*return a true value if the pcb pointed to by p has no children*/
	return (p == NULL);
}/*emptyChild*/

void insertChild(pcb_t *prnt, pcb_t *p){
	/*make the pcb pointed ot by p a child of the pcb pointed to by prnt*/

	/*insert at the beggining for constant */

	/*Check to see if the tree is empty */
	if(emptyChild(p)){
		return NULL;
	}

	/******* What if my parent has no children ****/
	if(prnt->p_child == NULL){/* if my parent has no children*/
		prnt->p_child = p; /* setting the parent to the child */
	}

	/***** What if the parent has a child*****/

	/*have p point to the sibling*/
	p->p_sib = prnt->p_child; 

	/*parent point to new child */
	prnt->p_prnt = p; 

	/*have new child point to new parent */
	p->p_prnt = prnt;

}
	

pcb_t *removeChild (pcb_t *p){
	/*we are given a parent from top */

	if(emptyChild(p)){ /*if tree is empty, return null*/
	return NULL;
	}

	if(p->p_child->p_sib == NULL){ /*there is only one child*/
		p->p_child = NULL; 
		p->p_prnt->p_child = NULL; 
	}

	/*setting dummhy nodes to prevent confusion*/
	pcb_t *oldChild = p->p_child;
	pcb_t *parent = oldChild->p_prnt;
	pcb_t *newChild = oldChild->p_sib;

	/*stops next sib from pointing*/
	newChild->p_sib = NULL;
	/*set parent first child as new sibling*/
	parent->p_sib = newChild;
	/*make p have no siblings*/
	oldChild->p_sib = NULL;
	oldChild->p_prnt = NULL; 

	return newChild; 

} /*removeChild*/

pcb_t *outChild(pcb_t *p){
	/*make the pcb pointed to by p no longer the child of its parent*/

	pcb_t *parent = p->p_prnt; /*setting a dummy node for parent*/
	pcb_t *child = parent ->p_child; /*a place holder for the child*/

	if(emptyChild(p)){ /*checks if tree is empty */
 		return NULL;
 	}

 	/********* You are the child ********/
 	if(p->p_prnt == p){
 		removeChild(p);
 	}

 	/*** Not the first child ****/

 	/*middle child */

 	/*remove pointer from p to parent */
 	p->p_prnt = NULL; 

 	/*make child point to new sibling */
 	child->p_sib = p->p_sib;

 	/*remove the sibling from p*/
 	p->p_sib = NULL; 

 	return outChild(p);

}/* outChild*/

/************************END*******************************/