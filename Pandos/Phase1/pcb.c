/*Including the .h files*/

#include "../h/const.h"
#include "../h/types.h"
#include "../h/pcb.h"
#include "../h/asl.h"

/*global pointer for free list*/
HIDDEN pcb_t *pcbfree_h;

/********************Allocation and Deallocation*****************/
void debugA(int a){

	/*a simple debug function to help fix our mistakes*/

	int i = 0; 

}/* debugA */

void initPcbs(){
	int i; 

	pcbfree_h = mkEmptyProcQ();
	static pcb_t foo[MAXPROC];
	
	for(i = 0; i < MAXPROC; i++){
		insertProcQ(&pcbfree_h, &foo[i]);
	}

}/*initPcb*/

void freePcb (pcb_t*p){

	insertProcQ(&pcbfree_h,p);
	/*debugA(1);*/

}/*freePcb*/

pcb_t* allocPcb (){
	/*remove an element from the pcbfree list */

	/*check to see if the pcbfree list is empty*/ 
	if (pcbfree_h == (NULL)){
		/*debugA(1);*/
		return NULL;
} /*set the pcb and give it's information

	/make sure it's not being used*/
	/*debugA(debugg);*/

	pcb_t *p = removeProcQ(&pcbfree_h); /* that pcb is empty*/ 

	/*set the information to the pcb*/
	p->p_next = NULL; 
	p->p_prev = NULL;
	p->p_semAdd = NULL;
	p->p_prnt = NULL;
	p->p_child = NULL;
	p->p_sib = NULL;
	p->p_prevSib = NULL; 

	/*debugg++;*/ 
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

void insertProcQ (pcb_t**tp, pcb_t*p){
	/*inserts a new element into the queue*/

	if(emptyProcQ(*tp)==TRUE){
		/*if the list is empty set the new p as the tp*/
		p->p_next = p;
		p->p_prev = p; 
		(*tp)=p; 
		/*debugA(1);*/
	}
		/*N -> N+1*/

	/*debugA(debugg);*/

	p->p_next = (*tp)->p_next; 

	(*tp)->p_next = p;

	p->p_prev = (*tp);

	(*tp)->p_next->p_prev = p; 

	(*tp) = p;

	/*debugg++;*/
}/* insertProcQ */

pcb_t *removeProcQ (pcb_t**tp){
	/*removes the head element from the process queue whose tail pointer is pointed to by tp
	Return NULL if the provess queue was empty. Otherwise return the pointer that was removed */

	if(emptyProcQ(*tp) == TRUE){
		return NULL;
	} 

	pcb_t *temp = (*tp)->p_next; 

	
	if(temp == (*tp)){ 
		/*If the only one in the queue */
		/*set the whole thing to NULL*/
		(*tp)->p_prev = NULL; 
		(*tp)->p_next = NULL;
		(*tp) = (NULL); 
		return temp; 
	}

	/******** What if I am not the only one in the queue******/	
	(*tp)->p_next = temp->p_next;
	temp->p_next->p_prev = (*tp);
	temp->p_next = NULL;
	temp->p_prev = NULL;
	return temp; 

}/*removeProcQ*/

pcb_t *outProcQ (pcb_t**tp, pcb_t*p){
	/*remove pcb pointed to by p from the process queue whose tail pointer is pointed to by tp.
	Update the Process Queue. If not there return NULL. Otherwise return p*/

	/*if the tree is actually empty */
	if(emptyProcQ(*tp)){
		return NULL;
	}

	debugA(1);
	pcb_t *temp = (*tp)->p_next; 

	/****** What if i am the only node *****/

	if(temp == p){ /*if I am the head node and what you are looking for */
		removeProcQ(tp); /*call removeProcQ*/
	}


	/******* located in the middle *********/
	/*traverse the list*/
	int i;

	for(i = 0; i<MAXPROC; i++){
		if(temp != p){/*if temp is not p*/
		temp = temp->p_next;
		continue;
		}

		/*find p*/
		if(temp  == p){
			pcb_t *next = temp->p_next;
			pcb_t *back = temp->p_prev;
			next->p_prev = back; 
			back->p_next = next;

			p->p_next = NULL; 
			p->p_prev = NULL;
			return p;
		}

	}
	/*Return */
	return NULL; 
}

/*********************** PROCESS TREE MAINTENANCE ***********************/

int emptyChild(pcb_t*p){
	/*return a true value if the pcb pointed to by p has no children*/
	return (p == NULL);
}/*emptyChild*/

void insertChild(pcb_t*prnt, pcb_t*p){
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
	

pcb_t *removeChild (pcb_t*p){
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

pcb_t *outChild(pcb_t*p){
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