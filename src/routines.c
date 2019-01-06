#include <stdio.h>
#include <stdlib.h>
#include "mm.h"
#include "DAG.h"
#include "routines.h"
#include "DAG.h"

real* lsolve(mat_mar* L, mat_mar* b){
	dim i, j;
	real* x;
	if(!L->J || !L->I || !b)
		return NULL;
	
	if(is_sparse(b->head)){
		x = (real*)calloc(b->m,sizeof(real));
		CCSvectoArr(b,x);
	} else
		x = b->dat;

	for(i=0;i<L->m;i++){
		x[i] /= L->dat[L->J[i]];
		for(j = L->J[i]+1; j < L->J[i+1]; j++)
			x[L->I[j]] -= L->dat[j] * x[i];
	}
	return x;
}

real* lsolve_GP(mat_mar* L, mat_mar* b, Graph* graph){
	dim i, j;
	real* x;
	if(!L->J || !L->I || !b)
		return NULL;
	
	if(is_sparse(b->head)){
		x = (real*)calloc(b->m,sizeof(real));
		CCSvectoArr(b,x);
	} else
		x = b->dat;
	
	for(i=0;i<b->nz;i++)
		DFS(graph,b->I[i]);

	node* tmp = graph->reach.head;
	while(tmp!=NULL){
		i = tmp->vertex;
		x[i] /= L->dat[L->J[i]];
		for(j = L->J[i]+1; j < L->J[i+1]; j++)
			x[L->I[j]] -= L->dat[j] * x[i];
		tmp = tmp->next;
	}
	return x;
}
