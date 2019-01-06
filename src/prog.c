#include <stdio.h>
#include <stdlib.h>
#include "mm.h"
#include "DAG.h"
#include "routines.h"

int main(int argc, char* argv[]){
	char* fileA = "../torso1/torso1.mtx";
	char* fileb = "../b_for_torso1.mtx";
	//char* fileA = "../TSOPF_RS_b678_c2/TSOPF_RS_b678_c2.mtx";
	//char* fileb = "../b_for_TSOPF_RS_b678_c2_b.mtx";
	mat_mar A;
	mat_mar b;
	mat_mar L;
	real* x;
	dim i,j;

	A = init_mat(fileA);
	b = init_mat(fileb);
	L = getLvals(&A);
	//printf("%lu, %lf\n",A.I[10],A.dat[10]);
	//printf("%lu\n",A.J[A.m-1]);
	
	x = (real*)calloc(b.m,sizeof(real));
	CCSvectoArr(&b,x);

	lsolve(&L, x);
	
	Graph* DG = createGraph(L.n);
	for(i=0;i<L.n;i++)
		for(j= L.J[i]; j < L.J[i+1]; j++)
			addEdge(DG, i, L.I[j]);
	
	/*
	Graph* DG = createGraph(10);
    addEdge(DG, 0, 6);
    addEdge(DG, 0, 9);
    addEdge(DG, 1, 2);
    addEdge(DG, 1, 5);
    addEdge(DG, 2, 8);
    addEdge(DG, 2, 5);
    addEdge(DG, 3, 4);
    addEdge(DG, 3, 5);
    addEdge(DG, 3, 8);
    addEdge(DG, 4, 5);
    addEdge(DG, 4, 8);
    addEdge(DG, 5, 7);
    addEdge(DG, 6, 7);
    addEdge(DG, 7, 8);
    addEdge(DG, 7, 9);
    addEdge(DG, 8, 9);

    printf("\nDIRECTED GRAPH\n");
    displayGraph(DG);
	
	DFS(DG, 0);
	DFS(DG, 5);
	for(i=0;i<10;i++){
		printf("visited: %d, vertex: %ld\n",DG->visited[i],i+1);
	}
	*/

	freeGraph(DG);
	free_mat(&A);
	free_mat(&b);
	free_mat(&L);
	free(x);

	return 0;
}
