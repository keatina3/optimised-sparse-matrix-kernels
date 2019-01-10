#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"
#include "DAG.h"
#include "routines.h"

// standard serial code soln, using CCS format of lower triangular direct solve //
real* lsolve(mat_mar* L, mat_mar* b){
	dim i, j;
	real* x;
	if(!L || !b)
		return NULL;
	if(is_dense(L->head)){
		printf("Error. File not in CCS format\n");
		return NULL
	
	x = (real*)calloc(b->m,sizeof(real));
	CCSvectoArr(b,x);
	
	for(i=0;i<L->m;i++){
		x[i] /= L->dat[L->J[i]];
		for(j = (L->J[i])+1; j < L->J[i+1]; j++)
			x[L->I[j]] -= L->dat[j] * x[i];
	}
	return x;
}

// gilbert-meierls algorithm. i.e. only iterating through cols in reachset
real* lsolve_GP(mat_mar* L, mat_mar* b, Graph* graph){
	dim i, j;
	real* x;				// to store soln.
	if(!L || !b)
		return NULL;
	if(is_dense(L->head)){
		printf("Error. File not in CCS format\n");
		return NULL
	}

	x = (real*)calloc(b->m,sizeof(real));
	CCSvectoArr(b,x);
	
	node* tmp = graph->reach.tail;
	while(tmp!=NULL){			// iterating through columns in reachset via linked list
		i = tmp->vertex;
		x[i] /= L->dat[L->J[i]];
		for(j = L->J[i]+1; j < L->J[i+1]; j++)
			x[L->I[j]] -= L->dat[j] * x[i];
		tmp = tmp->prev;
	}
	return x;
}
