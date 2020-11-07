/*Including the .h files*/

#include "../h/const.h"
#include "../h/types.h"
#include "../h/pcb.h"
#include "../h/asl.h"

/**************************************************************************** 
 * 
 *	This file contains all methods and instructions for PCB and Trees 
 *	Processing (Child)
 * 
 ****************************************************************************/
extern void initPcbs();

/*global pointer for free list*/
HIDDEN pcb_t *pcbfree_h;

/**********************DEBUG FUNCTIONS CREATED BY US ************/
void debugA(int a, pcb_t *t, pcb_t * h, pcb_t * c){
	/*a debug function */

	int i = 0; 

}/* debugA */

void debugB(int a){
	/*quick and simple debug statement to show errors quicker*/

	int i = 0; 
}/* debugB */

/********************Allocation and Deallocation*****************/

void initPcbs(){
	/*Initialize the PCBFree list to contain all the elements 
	of MAXPROC in an queue array. Called only once */

	int i; 

	pcbfree_h = mkEmptyProcQ();
	static pcb_t foo[MAXPROC];
	
	for(i = 0; i < MAXPROC; i++){
		insertProcQ(&pcbfree_h, &foo[i]);
	}

}/*initPcb*/

void freePcb (pcb_t*p){

	insertProcQ(&pcbfree_h,p);
}/*freePcb*/

pcb_t* allocPcb (){
	/*remove an element from the pcbfree list */

	/*check to see if the pcbfree list is empty*/ 
	if (pcbfree_h == (NULL)){
		return NULL;
	} /*set the pcb and give it's information

	/make sure it's not being used*/

	pcb_t *p = removeProcQ(&pcbfree_h); /* that pcb is empty*/ 

	/*set the information to the pcb*/
	p->p_next = NULL; 
	p->p_prev = NULL;
	p->p_semAdd = NULL;
	p->p_prnt = NULL;
	p->p_child = NULL;
	p->p_sib = NULL;
	p->p_prevSib = NULL; 
	return p; 

} /*allocPcb*/


/********************QUEUE MAINTENTANCE***************************/

pcb_t *mkEmptyProcQ(){
	/* makes the whole queue empty and returns a pointer to the tail of an empty process queue*/
	return NULL;
} /*mkemptyProcq*/

int emptyProcQ (pcb_t *tp){
	/*returns a boolean value whether the queue is empty or not. Returns true if it is empty; 
	false if the queue is not */
	return (tp == (NULL));

}/*emptyProcQ*/

pcb_t *headProcQ(pcb_t *tp){
	/*returns the head pcb in the queue pointed to by tp*/

	if(emptyProcQ(tp)){

		return NULL; 

		/*list is empty*/ 
	}

	return (tp -> p_next);

}/*headProcQ*/

void insertProcQ (pcb_t**tp, pcb_t*p){
	/*inserts a new element into the queue. PCB pointed to by p is put into queue at tp. 
	Becomes the new head*/

	if(emptyProcQ(*tp)==TRUE){
		/*if the list is empty set the new p as the tp*/
		p->p_next = p;
		p->p_prev = p; 
		(*tp)=p;

		return;
	}
		/*N -> N+1*/

	p->p_next = (*tp)->p_next; 

	(*tp)->p_next = p;

	p->p_prev = (*tp);

	p->p_next->p_prev = p; 

	(*tp) = p;

}/* insertProcQ */

pcb_t *removeProcQ (pcb_t**tp){
	/*removes the head element from the process queue whose tail pointer is pointed to by tp
	Return NULL if the provess queue was empty. Otherwise return the pointer that was removed */



	if(emptyProcQ(*tp) == TRUE){

		return NULL;
	} 

	pcb_t *head = (*tp)->p_next; 
	pcb_t *newHead = head->p_next;


	if((*tp)->p_next != (*tp)){ 

		(*tp)->p_next = newHead;
		newHead->p_prev = (*tp);
		head->p_next = NULL;
		head->p_prev = NULL;

		return head; 
	}

	/******** What if I am not the only one in the queue******/	

	(*tp)->p_next = NULL;
	(*tp)->p_prev = NULL;
	(*tp) = NULL;
	return head;

}/*removeProcQ*/

pcb_t *outProcQ (pcb_t**tp, pcb_t*p){
	/*remove pcb pointed to by p from the process queue whose tp is pointed to by tp.
	Update the Process Queue. If not there return NULL. Otherwise return p*/

	int i; 

	/*if the tree is actually empty */
	if(emptyProcQ(*tp)){
		return NULL;
	}

	pcb_t *temp = (*tp)->p_next;

	if(temp == p){

		return removeProcQ(tp); 
	}

	for(i=0; i <MAXPROC; i++){
		
		if(temp != p){

		temp = temp->p_next;

		continue; 
		}

		if(temp == p){

			pcb_t *next = temp->p_next;
			pcb_t *back = temp->p_prev;

			next->p_prev = back;
			back->p_next = next;

			temp->p_next = NULL;
			temp->p_prev = NULL;
			return p; 
		}
	}

	return (NULL);

}/*outProcQ*/

/*********************** PROCESS TREE MAINTENANCE ***********************/

int emptyChild(pcb_t*p){
	/*return a true value if the pcb pointed to by p has no children*/
	return (p->p_child == NULL);

}/*emptyChild*/

void insertChild(pcb_t*prnt, pcb_t*p){
	/*make the pcb pointed ot by p a child of the pcb pointed to by prnt*/

	/*insert at the beggining for constant */

	/*Check to see if the tree is empty */
	if(emptyChild(prnt)){

		prnt->p_child = p;
		p->p_prnt = prnt;
		p->p_sib = NULL;
		p->p_prevSib = NULL; 

		return; 
	}

	/***** What if the parent has a child*****/

	/*have p point to the sibling*/
	p->p_sib = prnt->p_child; 

	prnt->p_child->p_prevSib = p; 

	/*parent point to new child */
	prnt->p_child = p; 

	/*have new child point to new parent */
	p->p_prnt = prnt;

	return; 

}/*insetChild*/
	

pcb_t *removeChild (pcb_t*p){
	/*we are given a parent from top */

	if(emptyChild(p)){ /*if tree is empty, return null*/
		return NULL;
	}

	if(p->p_child->p_sib == NULL){ /*there is only one child*/

		pcb_t *child = p->p_child; 

		child->p_prnt = NULL; 
		p->p_child = NULL;

		return child; 
	}

	/*setting dummhy nodes to prevent confusion*/
	pcb_t *oldChild = p->p_child;
	pcb_t *newChild = oldChild->p_sib;

	/*stops next sib from pointing*/
	newChild->p_prevSib = NULL;
	/*set parent first child as new sibling*/
	p->p_child = newChild;
	/*make p have no siblings*/
	oldChild->p_sib = NULL;
	oldChild->p_prnt = NULL; 

	return oldChild; 

} /*removeChild*/

pcb_t *outChild(pcb_t*p){
	/*make the pcb pointed to by p no longer the child of its parent*/
	if(p == NULL){ /*checks if p points to anything */
 		return NULL;
 	}

 	if(p->p_prnt == NULL){
 		return NULL;
 	}

	pcb_t *parent = p->p_prnt; /*setting a dummy node for parent*/
 	/********* You are the child ********/
 	if(p->p_prnt->p_child == p){
 		return(removeChild(parent));
 	}

 	/*** Not the first child ****/

 	/*end child */

 	if(p->p_sib == NULL){

 		p->p_prnt = NULL;
 		p->p_prevSib->p_sib = NULL;
 		p->p_prevSib = NULL;
 		return p;
 	}

 	/*middle child */

 	/*remove pointer from p to parent */
 	p->p_prnt = NULL; 

 	/*make child point to new sibling */
 	p->p_prevSib->p_sib = p->p_sib;
 	p->p_sib->p_prevSib = p->p_prevSib; 

 	/*remove the sibling from p*/
 	p->p_sib = NULL; 
 	p->p_prevSib = NULL;

 	return p;

}/* outChild*/

/************************END*******************************/