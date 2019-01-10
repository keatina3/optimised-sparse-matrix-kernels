#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <getopt.h>
#include <time.h>
#include "mmio.h"
#include "DAG.h"
#include "routines.h"
#include "utils.h"

#define err 1E-10	// tolerance level of check for accuracy

// fn to be used for reordering output for printing to file //
int compare (const void * a, const void * b) {return ( *(int*)a - *(int*)b );}

int main(int argc, char* argv[]){
	int myid, nprocs, root = 0;	// proc ID, number procs, root proc
	mat_mar A,L,b;			// matrix structures
	real *x,*y;			// array for results of  serial & parallel code for comp
	real SSE;			// sum square error
	levelSet* G;			// declaring levelset
	Graph* DG;			// DAG
	dim i;			
	clock_t start, end;
	double time_taken;
	char* fileA;			// matrix file
	char* fileb;			// RHS
	int option = 0;
	
	// taking fileA address & fileb address from user input params in bash //
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
	
	// intialising MPI state //
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	
	
	// READING IN DATA & GENERATING DEPENDENCIES //

	if(myid==root){
		A = init_mat(fileA);
		L = getLvals(&A);		// lower triangular version of A if necessary
		b = init_mat(fileb);
		DG = getReach(&L, &b);		// getting reachset
		G = assignLevelSet(DG);		// assinging levelset
	}
	
	// DATA DISPERSION //
	
	// broadcasting matrix data to all threads //
	BcastMatrix(&L, myid, root, MPI_COMM_WORLD);
	BcastMatrix(&b, myid, root, MPI_COMM_WORLD);

	
	// ROUTINES //
	
	start = clock();
	x = lsolve_Par(&L, &b, G, myid, nprocs, root, MPI_COMM_WORLD);
	end = clock();
	time_taken = ((double)(end-start))/CLOCKS_PER_SEC;
	if(myid==root) printf("Time taken: %lf, Nprocs = %d\n",time_taken,nprocs);

	// VERIFICATION //
	
	// same as in serial code, using SSE and abs(diff)> tolerance as verification checks //
	SSE = 0;
	if(myid==root){
		y = lsolve(&L, &b);
		for(i=0;i<b.m;i++){
			SSE += (x[i]-y[i])*(x[i]-y[i]); 
			if(fabs(x[i]-y[i]) > err)
				printf("x[%lu] = %0.16lf, y[%lu] = %0.16lf\n",i,x[i],i,y[i]);
		}
		printf("SSE = %0.16lf\n",SSE);
		

	// OUTPUTTING RESULTS TO FILE //
		
		dim* nzInd = (dim*)malloc(DG->reach.numElems*sizeof(dim));
		node* tmp = DG->reach.tail;
		i=0;
		while(tmp!=NULL){
				nzInd[i] = tmp->vertex;
				tmp = tmp->prev;
				i++;
		}
		qsort(nzInd, DG->reach.numElems, sizeof(dim), compare);
		mat_mar xMM = ArrtoCCS(x,nzInd,b.m,DG->reach.numElems);
		free(nzInd);
		writeMM(&xMM,"x.mtx");

	// FREE DATA //
		
		free(x);
		free(y);
		free_mat(&A);
		free_mat(&xMM);
		freeGraph(DG);
		freeLevelSet(G);
	}
	free_mat(&L);
	free_mat(&b);
	
	// ending MPI state //
	MPI_Finalize();
	return 0;
}
