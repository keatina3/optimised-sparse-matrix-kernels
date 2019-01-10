#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"
#include "DAG.h"

// create a new node in reachset //
node* createNode(dim v){
	node* newNode = (node*)malloc(sizeof(node));
	if(!newNode)
		printf("Error creating node\n");
		
	newNode->vertex = v;	// vertex representing col number
	newNode->next = NULL;	// 2 direction linked list
	newNode->prev = NULL;

	return newNode;
}

// intialise graph to represent directed acyclic graph for the dependencies //
Graph* createGraph(dim n){
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	if(!graph)
		printf("Error allocating DAG.\n");
	
	graph->visited = (bool*)calloc(n,sizeof(bool));	// boolean array representing if DFS has touched node or not	
	graph->reach.head = NULL;
	graph->reach.tail = graph->reach.head;			//start at tail, since DFS fn appends in reverse order
	return graph;
}

// free all allocated memory from the DAG //
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

// appending node/element to the reachset //
void appendSet(reachset* reach, dim vertex){
	node* newNode = createNode(vertex);
	node* old = reach->tail;
    
	if(reach->head == NULL){	// if Nullset
    	reach->head = newNode;
    	old = reach->head;
	}

	// reassinging values and adding new node to tail //
	newNode->next = NULL;
	newNode->prev = old;
	old->next = newNode;
	reach->tail = newNode;
	reach->head->prev = NULL;
	reach->numElems++;			// numElems to be used later when cardinality of reachset needed
}

// depth first search functionality //
void DFS(mat_mar* L, Graph* graph, dim vertex) {
	graph->visited[vertex] = 1;		// set node to visited
	dim i;
	for(i = (L->J[vertex]+1); i < L->J[vertex+1]; i++){			// looping through all dependencies of 'vertex'. Using matrix as a DAG.
		if(graph->visited[L->I[i]] == 0){
			DFS(L, graph, L->I[i]);						// recursive. Do if not visited.
		}
	}
	appendSet(&graph->reach, vertex);			// once fn has recusively worked its way throug all child nodes, add node to reachset.
}

// gets reachset in topological order //
Graph* getReach(mat_mar* L, mat_mar* b){		// apply DFS for all relevant entry nodes to get reachset.
	Graph* graph = createGraph(L->n);
	dim i;
	graph->reach.numElems = 0;
	for(i=0;i<b->nz;i++)
		if(graph->visited[i] == 0)
			DFS(L, graph, b->I[i]);
	return graph;
}
