#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <math.h>
#include "mmio.h"
#include "DAG.h"
#include "routines.h"

#define err 1E-15		// tolerance level of check for accuracy

// fn to be used for reordering output for printing to file //
int compare (const void * a, const void * b){return ( *(int*)a - *(int*)b);}

int main(int argc, char* argv[]){
	mat_mar A, b, L;			// matrix structures
	real *x,*y;					// arrays for results of TRSV
	dim i;
	real SSE = 0;				// Sum squared error, used for accuracy check
	clock_t start, end;
	double time_taken;
	char* fileA;				// matrix
	char* fileb;				// RHS
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

	// READING IN DATA //

	A = init_mat(fileA);
	b = init_mat(fileb);
	L = getLvals(&A);
	
	// ROUTINES //
	
	/* serial code */
	start = clock();
	x = lsolve(&L, &b);
	end = clock();
	time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Time taken: %lf\n", time_taken);
	
	/* Gilbert-Meierls */
	Graph* DG = getReach(&L, &b);
	start = clock();
	y = lsolve_GP(&L, &b, DG);
	end = clock();
	time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Time taken: %lf\n", time_taken);
	

	// VERIFICATION //
	
	// using SSE and also printing any vals with diff > err to screen to verify answer against serial code soln //
	for(i=0;i<L.m;i++){
		SSE += (y[i]-x[i])*(y[i]-x[i]); 
		if(fabs(y[i]-x[i]) > err)
			printf("x[%lu] = %0.16lf, y[%lu] = %0.16lf\n",i,x[i],i,x[i]);	
	}
	// printing SSE to terminal //
	printf("SSE = %0.16lf\n",SSE);
	
	
	// OUTPUTTING RESULTS //

	dim* nzInd = (dim*)malloc(DG->reach.numElems*sizeof(dim)); // will hold indices of sorted rows for printing to file
	node* tmp = DG-> reach.tail;			// starting at end of reachset
	i=0;
	while(tmp!=NULL){						// incrementing through reachset, using vals in reachset as row indices
		nzInd[i] = tmp->vertex;
		tmp = tmp->prev;
		i++;
	}
	qsort(nzInd, DG->reach.numElems, sizeof(dim), compare);	// sorting in order
	mat_mar xMM = ArrtoCCS(y,nzInd,b.m,DG->reach.numElems);	// converting array into CCS format
	free(nzInd);
	writeMM(&xMM,"x.mtx");		// writing to .mtx file

	// FREE DATA //

	freeGraph(DG);
	free_mat(&A);
	free_mat(&b);
	free_mat(&L);
	free_mat(&xMM);
	free(x);
	free(y);

	return 0;
}
