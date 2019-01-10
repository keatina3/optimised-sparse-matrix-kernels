#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <math.h>
#include "mmio.h"
#include "DAG.h"
#include "routines.h"

#define err 1E-15

int compare (const void * a, const void * b){return ( *(int*)a - *(int*)b);}
int main(int argc, char* argv[]){
	mat_mar A, b, L;
	real *x,*y;
	dim i;
	real SSE = 0;
	clock_t start, end;
	double time_taken;
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

	for(i=0;i<L.m;i++){
		SSE += (y[i]-x[i])*(y[i]-x[i]); 
		if(fabs(y[i]-x[i]) > err)
			printf("x[%lu] = %0.16lf, y[%lu] = %0.16lf\n",i,x[i],i,x[i]);	
	}
	printf("SSE = %0.16lf\n",SSE);
	
	// OUTPUTTING RESULTS //

	dim* nzInd = (dim*)malloc(DG->reach.numElems*sizeof(dim));
	node* tmp = DG-> reach.tail;
	i=0;
	while(tmp!=NULL){
		nzInd[i] = tmp->vertex;
		tmp = tmp->prev;
		i++;
	}
	qsort(nzInd, DG->reach.numElems, sizeof(dim), compare);
	mat_mar xMM = ArrtoCCS(y,nzInd,b.m,DG->reach.numElems);
	free(nzInd);
	writeMM(&xMM,"x.mtx");

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
