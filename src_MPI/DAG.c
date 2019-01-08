#include <stdio.h>
#include <stdlib.h>
#include "mm.h"
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
	
	graph->depth = (dim*)calloc(n,sizeof(dim));
	graph->visited = (bool*)calloc(n,sizeof(bool));
	graph->reach.head = NULL;
	graph->reach.tail = graph->reach.head;
	return graph;
}

void freeGraph(Graph* graph){
	if(graph){
		node* reachPtr = graph->reach.head;
		while (reachPtr){
			node* tmp = reachPtr;
			reachPtr = reachPtr->next;
			free(tmp);
        }
		free(graph->depth);
        free(graph->visited);
    }
	free(graph);
}


void appendReach(Graph* graph, dim vertex){
	node* newNode = createNode(vertex);
	node* old = graph->reach.tail;
    
	if(graph->reach.head == NULL){
    	graph->reach.head = newNode;
    	old = graph->reach.head;
	}

	newNode->next = NULL;
	newNode->prev = old;
	old->next = newNode;
	graph->reach.tail = newNode;
	graph->reach.head->prev = NULL;
}

void DFS(mat_mar* L, Graph* graph, dim vertex, dim count) {
	graph->visited[vertex] = 1;
	count++;
	graph->depth[vertex] = count;
	dim i;
	for(i = (L->J[vertex]+1); i < L->J[vertex+1]; i++){
		if(graph->visited[L->I[i]] == 0){
			DFS(L, graph, L->I[i], count);
		}
		if(graph->depth[L->I[i]] < count+1)
			adjustDepth(L, graph, L->I[i], count);
	}
	graph->reachCard++;
	appendReach(graph, vertex);
}

void adjustDepth(mat_mar* L, Graph* graph, dim vertex, dim count){
	graph->depth[vertex] = count+1;
	count++;
	dim i;
	for(i = (L->J[vertex]+1); i < L->J[vertex+1]; i++){
		if(graph->depth[L->I[i]] < count+1)
			adjustDepth(L, graph, L->I[i], count);
	}
}

Graph* getReach(mat_mar* L, mat_mar* b){
	Graph* graph = createGraph(L->n);
	dim i;
	graph->reachCard = 0;
	for(i=0;i<b->nz;i++)
		if(graph->visited[i] == 0)
			DFS(L, graph, b->I[i],0);
	return graph;
}
