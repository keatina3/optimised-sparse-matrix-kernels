#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"
#include "DAG.h"

node* createNode(dim v){
	node* newNode = (node*)malloc(sizeof(node));
	if(!newNode)
		printf("Error creating node\n");
		
	newNode->vertex = v;
	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}

Graph* createGraph(dim n){
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	if(!graph)
		printf("Error allocating DAG.\n");
	
	graph->visited = (bool*)calloc(n,sizeof(bool));
	graph->reach.head = NULL;
	graph->reach.tail = graph->reach.head;
	return graph;
}

void freeGraph(Graph* graph){
	if(graph){
		node* reachPtr = graph->reach.head;
		while (reachPtr!=NULL){
			node* tmp = reachPtr;
			reachPtr = reachPtr->next;
			free(tmp);
        }
        free(graph->visited);
    }
	free(graph);
}

void appendSet(reachset* reach, dim vertex){
	node* newNode = createNode(vertex);
	node* old = reach->tail;
    
	if(reach->head == NULL){
    	reach->head = newNode;
    	old = reach->head;
	}

	newNode->next = NULL;
	newNode->prev = old;
	old->next = newNode;
	reach->tail = newNode;
	reach->head->prev = NULL;
	reach->numElems++;
}

void DFS(mat_mar* L, Graph* graph, dim vertex, dim count) {
	graph->visited[vertex] = 1;
	count++;
	dim i;
	for(i = (L->J[vertex]+1); i < L->J[vertex+1]; i++){
		if(graph->visited[L->I[i]] == 0){
			DFS(L, graph, L->I[i], count);
		}
	}
	appendSet(&graph->reach, vertex);
}

Graph* getReach(mat_mar* L, mat_mar* b){
	Graph* graph = createGraph(L->n);
	dim i;
	graph->reach.numElems = 0;
	for(i=0;i<b->nz;i++)
		if(graph->visited[i] == 0)
			DFS(L, graph, b->I[i],0);
	return graph;
}
