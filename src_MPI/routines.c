#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mmio.h"
#include "DAG.h"
#include "utils.h"
#include "routines.h"

// serial code for single proc //
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

// parallel code using level set method //
real* lsolve_Par(mat_mar* L, mat_mar* b, levelSet* G, int myid, int nprocs, int root, MPI_Comm comm){
	dim i, j, k;
	dim colCount, col, *colIndex;
	real* x;		// to contain result
	real localsum;		// will be used in levelsync. 
	dim numLevels;	
	MPI_Request request;	// for checking non blocking sends. unused.
	if(!L || !b)
		return NULL;

	x = (real*)calloc(b->m,sizeof(real));
	if(myid==root){
		numLevels = G->numLevels;
		CCSvectoArr(b,x);
	}
	
	// broadcasting number of levels and the vector x //
	MPI_Bcast(x,b->m,MPI_DOUBLE,root,comm);
	MPI_Bcast(&numLevels,1,MPI_UNSIGNED_LONG,root,comm);
	
	// iterating through the levels //
	for(i=0;i<numLevels;i++){
		// assinging cols to calculate to each threa //
		colIndex = assignCols(&colCount,myid,nprocs,root,G,i,comm);
		// iterating through assigned columns //
		for(j=0;j<colCount;j++){
			col = colIndex[j];
			x[col] /= L->dat[L->J[col]];	// assigning val for x
			if(myid!=root){			// if not root, send x val to root
				MPI_Isend(&x[col],1,MPI_DOUBLE,root,col,comm,&request);
			}
			// incrementing through dependencies in assigned column
			for(k = L->J[col]+1; k < L->J[col+1]; k++){
				// temp sum, to be gathered by root and added to correct x val
				localsum = - (L->dat[k] * x[col]);
				if(myid!=root){	// send to roor
					MPI_Isend(&localsum,1,MPI_DOUBLE,root,L->I[k],comm,&request);
				}
				else		// if root continue as if using serial algorithm
					x[L->I[k]] += localsum;
			}
		}
		// sync all data at end of level so data dependencies are upheld //
		levelSync(L,x,G,i,colCount,myid,root,comm);
		if(colCount>0)		// if processor was assigned data, free it
			free(colIndex);
	}
	return x;
}

