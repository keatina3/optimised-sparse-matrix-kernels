#include <stdio.h>
#include <stdlib.h>
#include "mm.h"
#include "DAG.h"

/* Function to create an adjacency list node*/
node* createNode(int v){
	node* newNode = (node*)malloc(sizeof(node));
	if(!newNode)
		printf("Error creating node\n");
		
	newNode->vertex = v;
	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}

Graph* createGraph(int n){
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
		while (reachPtr){
			node* tmp = reachPtr;
			reachPtr = reachPtr->next;
			free(tmp);
        }
        	/*Free the adjacency list array*/
        free(graph->visited);
    }
    	/*Free the graph*/
	free(graph);
}


void appendReach(Graph* graph, int vertex){
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

void DFS(mat_mar* L, Graph* graph, int vertex) {
	graph->visited[vertex] = 1;
	dim i;
	for(i = (L->J[vertex]+1); i < L->J[vertex+1]; i++)
		if(graph->visited[L->I[i]] == 0)
			DFS(L, graph, L->I[i]);
	appendReach(graph, vertex);
}
