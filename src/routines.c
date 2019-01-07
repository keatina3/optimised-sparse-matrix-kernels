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
	
	x = (real*)calloc(b->m,sizeof(real));
	CCSvectoArr(b,x);
	
	for(i=0;i<b->nz;i++)
		DFS(graph,b->I[i]);
	int count = 0;
	node* tmp = graph->reach.tail;
	while(tmp!=NULL){
		count++;
		i = tmp->vertex;
		//printf("R[i] = %lu\n",i);
		x[i] /= L->dat[L->J[i]];
		for(j = L->J[i]+1; j < L->J[i+1]; j++)
			x[L->I[j]] -= L->dat[j] * x[i];
		tmp = tmp->prev;
	}
	printf("Count = %d\n",count);
	return x;
}
