#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mm.h"
#include "DAG.h"
#include "routines.h"

int main(int argc, char* argv[]){
	//char* fileA = "../torso1/torso1.mtx";
	//char* fileb = "../torso1/b_for_torso1.mtx";
	//char* fileA = "../TSOPF_RS_b678_c2/TSOPF_RS_b678_c2.mtx";
	//char* fileb = "../TSOPF_RS_b678_c2/b_for_TSOPF_RS_b678_c2_b.mtx";
	//char* fileA = "../af_0_k101/af_0_k101.mtx";
	//char* fileb = "../af_0_k101/b_sparse_af_0_k101.mtx";
	//char* fileb = "../af_0_k101/b_dense_af_0_k101.mtx";
	char* fileA = "../test_L.mtx";
	char*fileb = "../test_b.mtx";
	
	mat_mar A;
	mat_mar b;
	mat_mar L;
	real *x,*y;
	dim i,j;

	A = init_mat(fileA);
	b = init_mat(fileb);
	L = getLvals(&A);
	//printf("%lu, %lf\n",A.I[10],A.dat[10]);
	//printf("%lu\n",A.J[A.m-1]);

	Graph* DG = createGraph(L.n);
	for(i=0;i<L.n;i++)
		for(j= L.J[i]+1; j < L.J[i+1]; j++)
			addEdge(DG, i, L.I[j]);
	
	x = lsolve(&L, &b);
	printf("x[0] = %lf\n",x[L.m-1]);
	//for(i=0;i<L.m;i++)
	//	printf("x[i] = %lf\n",x[i]);

	y = lsolve_GP(&L, &b, DG);		
	printf("y[0] = %lf\n",y[L.m-1]);	
	//for(i=0;i<L.m;i++)
	//	printf("y[i] = %lf\n",y[i]);
	
	real SSE = 0;
	for(i=0;i<L.m;i++)
		SSE += (y[i]-x[i])*(y[i]-x[i]); 
	printf("SSE = %lf\n",SSE);
	
	/*
	freeGraph(DG);
	DG = createGraph(14);
    addEdge(DG, 0, 2);
    addEdge(DG, 2, 4);
    addEdge(DG, 2, 7);
    addEdge(DG, 4, 7);
    addEdge(DG, 7, 9);
    addEdge(DG, 1, 3);
    addEdge(DG, 1, 8);
    addEdge(DG, 1, 6);
    addEdge(DG, 3, 8);
    addEdge(DG, 5, 8);
    addEdge(DG, 5, 9);
    addEdge(DG, 6, 9);
    addEdge(DG, 6, 10);
    addEdge(DG, 9, 10);
    addEdge(DG, 9, 12);
    addEdge(DG, 9, 13);
    addEdge(DG, 10, 11);
    addEdge(DG, 10, 12);
    addEdge(DG, 11, 12);
    addEdge(DG, 8, 11);
    addEdge(DG, 8, 12);

    printf("\nDIRECTED GRAPH\n");
    displayGraph(DG);
	
	DFS(DG, 3);
	DFS(DG, 5);
	for(i=0;i<14;i++){
		printf("visited: %d, vertex: %ld\n",DG->visited[i],i+1);
	}
	node* tmp = DG->reach.head;
	while(tmp!=NULL){
		printf("%u\n",1+tmp->vertex);
		tmp = tmp->next;
	}
	
	*/	
    //displayGraph(DG);
	dim count = 0;
	node* tmp = DG->reach.head;
	while(tmp!=NULL){
		//printf("%u\n",1+tmp->vertex);
		count++;
		tmp = tmp->next;
	}
	printf("Count = %lu\n",count);
		
	freeGraph(DG);
	free_mat(&A);
	free_mat(&b);
	free_mat(&L);
	free(x);

	return 0;
}
