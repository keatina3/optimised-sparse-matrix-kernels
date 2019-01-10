#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mm.h"
#include "DAG.h"
#include "utils.h"
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


real* lsolve_Par(mat_mar* L, mat_mar* b, levelSet* G, int myid, int nprocs, int root, MPI_Comm comm){
	dim i, j, k;
	dim colCount, col, *colIndex;
	real* x;
	real localsum;
	dim numLevels;
	MPI_Request request;
	if(!L || !b)
		return NULL;

	x = (real*)calloc(b->m,sizeof(real));
	if(myid==root){
		numLevels = G->numLevels;
		CCSvectoArr(b,x);
	}
	MPI_Bcast(x,b->m,MPI_DOUBLE,root,comm);
	MPI_Bcast(&numLevels,1,MPI_UNSIGNED_LONG,root,comm);
	
	for(i=0;i<numLevels;i++){
		colIndex = assignCols(&colCount,myid,nprocs,root,G,i,comm);
		for(j=0;j<colCount;j++){
			col = colIndex[j];
			x[col] /= L->dat[L->J[col]];
			if(myid!=root){
				MPI_Isend(&x[col],1,MPI_DOUBLE,root,col,comm,&request);
			}
			for(k = L->J[col]+1; k < L->J[col+1]; k++){
				localsum = - (L->dat[k] * x[col]);
				if(myid!=root){
					MPI_Isend(&localsum,1,MPI_DOUBLE,root,L->I[k],comm,&request);
				}
				else
					x[L->I[k]] += localsum;
			}
		}
		levelSync(L,x,G,i,colCount,myid,root,comm);
		if(colCount>0)
			free(colIndex);
	}
	return x;
}

