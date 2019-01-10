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
	newNode->next = NULL;	// vertex representing col number
	newNode->prev = NULL;

	return newNode;
}

// intialise graph to represent directed acyclic graph for the dependencies //
Graph* createGraph(dim n){
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	if(!graph)
		printf("Error allocating DAG.\n");
	
	graph->depth = (dim*)calloc(n,sizeof(dim));	// array representing depth of each node in DAG
	graph->visited = (bool*)calloc(n,sizeof(bool));	// boolean array representing if DFS has touched node or not
	graph->reach.head = NULL;
	graph->reach.tail = graph->reach.head;		// start at tail, since DFS fn appends in reverse order
	graph->critPath = 0;				// val for critical path of DAG
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
		free(graph->depth);
        free(graph->visited);
    }
	free(graph);
}

// free all allocated memory from the levelset //
void freeLevelSet(levelSet* G){
	dim i;
	for(i=0;i<G->numLevels;i++){
		node* ptr = G->level_ptr[i].tail;
		while(ptr!=NULL){
			node* tmp = ptr;
			ptr = ptr->prev;
			free(tmp);
		}
	}
	free(G->level_ptr);
	free(G);
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
	if(count > graph->critPath)
		graph->critPath = count;
	graph->depth[vertex] = count;
	dim i;
	for(i = (L->J[vertex]+1); i < L->J[vertex+1]; i++){
		if(graph->visited[L->I[i]] == 0){
			DFS(L, graph, L->I[i], count);
		}
		if(graph->depth[L->I[i]] < count+1)	// if the depth of the child node < depth(parent)+1
			adjustDepth(L, graph, L->I[i], count);	//recursively adjust depth, child may have initially taken shorted path
	}
	appendSet(&graph->reach, vertex);
}

// recursive function to get the depth of each node in the DAG //
void adjustDepth(mat_mar* L, Graph* graph, dim vertex, dim count){
	graph->depth[vertex] = count+1;
	count++;
	if(count > graph->critPath)
		graph->critPath = count;
	dim i;
	// recursively checks if the next node in the path has a higher depth //
	for(i = (L->J[vertex]+1); i < L->J[vertex+1]; i++){
		if(graph->depth[L->I[i]] < count+1)
			adjustDepth(L, graph, L->I[i], count);
	}
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

// takes the already acquired topologically ordered reachset and breaks it up into levelsets //
levelSet* assignLevelSet(Graph* graph){
	// intialising vals
	levelSet* G = (levelSet*)malloc(sizeof(levelSet));
	G->numLevels = graph->critPath;
	G->level_ptr = (reachset*)malloc(G->numLevels*sizeof(reachset));
	node* tmp = graph->reach.tail;
	dim i;
	for(i=0;i<G->numLevels;i++){
		G->level_ptr[i].head = NULL;
		G->level_ptr[i].tail = G->level_ptr[i].head;
		G->level_ptr[i].numElems = 0;
	}
	// increment through reachset //
	// all nodes with equal depth put in same level //
	while(tmp!=NULL){
		dim set = graph->depth[tmp->vertex];
		appendSet(&G->level_ptr[set-1], tmp->vertex);
		tmp = tmp->prev;
	}

	return G;
}
