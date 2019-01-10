#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <getopt.h>
#include <time.h>
#include "mm.h"
#include "DAG.h"
#include "routines.h"
#include "utils.h"

#define err 1E-15

int compare (const void * a, const void * b) {return ( *(int*)a - *(int*)b );}

int main(int argc, char* argv[]){
	int myid, nprocs, root = 0;
	mat_mar A,L,b;
	real *x,*y;
	real SSE;
	levelSet* G;
	Graph* DG;
	dim i;
	char* fileA;
	char* fileb;
	int option = 0;

	while ((option = getopt(argc, argv,"L:b:")) != -1) {
    	switch (option) {
        	case 'L' : fileA = optarg;
    		    break;
        	case 'b' : fileb = optarg;
            	break;
			default:
      			printf("Option incorrect\n");
      			return 1;
		}
	}
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	
	if(myid==root){
		A = init_mat(fileA);
		L = getLvals(&A);
		b = init_mat(fileb);
		DG = getReach(&L, &b);
		G = assignLevelSet(DG);
	}
	
	// INSERT DATA DISPERSION HERE //
	
	BcastMatrix(&L, myid, root, MPI_COMM_WORLD);
	BcastMatrix(&b, myid, root, MPI_COMM_WORLD);

	// INSERT ROUTINES HERE //
	
	x = lsolve_Par(&L, &b, G, myid, nprocs, root, MPI_COMM_WORLD);
	
	SSE = 0;
	if(myid==root){
		/*
		y = lsolve(&L, &b);
		for(i=0;i<b.m;i++){
			SSE += (x[i]-y[i])*(x[i]-y[i]); 
			if(fabs(x[i]-y[i]) > err)
				printf("x[i] = %0.16lf, y[i] = %0.16lf,col = %ld\n",x[i],y[i],i);
		}
		printf("SSE = %0.16lf\n",SSE);
		*/
		dim* nzInd = (dim*)malloc(DG->reach.numElems*sizeof(dim));
		node* tmp = DG->reach.tail;
		i=0;
		while(tmp!=NULL){
				nzInd[i] = tmp->vertex;
				tmp = tmp->prev;
				i++;
		}
		qsort (nzInd, DG->reach.numElems, sizeof(dim), compare);
		mat_mar xMM = ArrtoCCS(x,nzInd,b.m,DG->reach.numElems);
		free(nzInd);
		writeMM(&xMM,"x.mtx");

		free_mat(&A);
		free_mat(&xMM);
		freeGraph(DG);
		freeLevelSet(G);
	}
	free_mat(&L);
	free_mat(&b);

	MPI_Finalize();
	return 0;
}
