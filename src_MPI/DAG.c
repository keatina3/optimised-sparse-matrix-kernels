#include <stdio.h>
#include <stdlib.h>
#include "mm.h"
#include "DAG.h"

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
        free(graph->visited);
    }
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
	for(i = (L->J[vertex]+1); i < L->J[vertex+1]; i++){
		if(graph->visited[L->I[i]] == 0){
			DFS(L, graph, L->I[i]);
		}
	}
	graph->reachCard++;
	appendReach(graph, vertex);
}

Graph* getReach(mat_mar* L, mat_mar* b){
	Graph* graph = createGraph(L->n);
	dim i;
	graph->reachCard = 0;
	for(i=0;i<b->nz;i++)
		if(graph->visited[i] == 0)
			DFS(L, graph, b->I[i]);
	return graph;
}

void getLevels(mat_mar* L, Graph* graph, real* levelSets, real** levelSets_ptr){
	levelSets = (real*)calloc(2*graph->reachCard,sizeof(real));
	levelSets_ptr = (real**)malloc(2*sizeof(real*));
	levelSets_ptr[0] = levelSets;
	levelSets_ptr[1] = &levelSets[graph->reachCard];
	graph->depth = (int*)calloc(L->m, sizeof(int));

	int parent,child,depmax,i,count;
	node* tmp1 = graph->reach.tail;
	while(tmp1!=NULL){
		count = 0;
		depmax = 0;
		node* tmp2 = graph->reach.tail;
		while(tmp2 != tmp1){
			child = tmp1->vertex;
			parent = tmp2->vertex;
			if( (child > parent) && (child <= L->I[L->J[parent+1]-1]) ){
				for(i = L->J[parent]; i < L->J[parent+1]; i++){
					if(child == L->I[i] && graph->depth[i] > depmax){
						depmax = graph->depth[i];
					}
				}
			}
			tmp2 = tmp2->prev;
		}
		graph->depth[tmp1->vertex] = depmax + 1;
		levelSets_ptr[0][count] = tmp1->vertex;
		levelSets_ptr[1][count] = depmax;
		tmp1 = tmp1->prev;
		count++;
	}
}
