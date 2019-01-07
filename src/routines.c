#include <stdio.h>
#include <stdlib.h>
#include "mm.h"
#include "DAG.h"
#include "routines.h"

real* lsolve(mat_mar* L, mat_mar* b){
	dim i, j;
	real* x;
	if(!L || !b)
		return NULL;
	
	x = (real*)calloc(b->m,sizeof(real));
	CCSvectoArr(b,x);
	
	for(i=0;i<L->m;i++){
		x[i] /= L->dat[L->J[i]];
		for(j = (L->J[i])+1; j < L->J[i+1]; j++)
			x[L->I[j]] -= L->dat[j] * x[i];
	}
	return x;
}

real* lsolve_GP(mat_mar* L, mat_mar* b, Graph* graph){
	dim i, j;
	real* x;
	if(!L || !b)
		return NULL;
	if(is_dense(L->head)){
		printf("Gilbert Meierls algorithm for dense RHS of no benefit. Using standard lsolve\n");
		return lsolve(L,b);
	}

	x = (real*)calloc(b->m,sizeof(real));
	CCSvectoArr(b,x);
	
	//for(i=0;i<b->nz;i++)
	//	DFS(L,graph,b->I[i]);
	
	node* tmp = graph->reach.tail;
	while(tmp!=NULL){
		i = tmp->vertex;
		x[i] /= L->dat[L->J[i]];
		for(j = L->J[i]+1; j < L->J[i+1]; j++)
			x[L->I[j]] -= L->dat[j] * x[i];
		tmp = tmp->prev;
	}
	return x;
}
