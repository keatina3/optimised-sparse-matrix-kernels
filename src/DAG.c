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

	return newNode;
}

/* Function to create a graph with n vertices; Creates both directed and undirected graphs*/
Graph* createGraph(int n){
	int i;
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	if(!graph)
		printf("Error allocating DAG.\n");
	
	graph->num_vertices = n;
	graph->visited = (bool*)calloc(graph->num_vertices,sizeof(bool));
	graph->reach.head = NULL;
	graph->reach.tail = graph->reach.head;
	/* Create an array of adjacency lists*/
	graph->adjListArr = (adj_list*)malloc(n * sizeof(adj_list));
	if(!graph->adjListArr)
		printf("Error allocating list array for DAG");
		//err_exit("Unable to allocate memory for adjacency list array");

	for(i = 0; i < n; i++){
		graph->adjListArr[i].head = NULL;
		graph->adjListArr[i].num_elems= 0;
	}

	return graph;
}

/*Destroys the graph*/
void freeGraph(Graph* graph){
	if(graph){
		if(graph->adjListArr){
			int v;
        	/*Free up the nodes*/
        	for (v = 0; v < graph->num_vertices; v++){
        		node* adjListPtr = graph->adjListArr[v].head;
				while (adjListPtr){
					node* tmp = adjListPtr;
					adjListPtr = adjListPtr->next;
					free(tmp);
				}
        	}
        	/*Free the adjacency list array*/
        	free(graph->visited);
			free(graph->adjListArr);
    	}
    	/*Free the graph*/
		free(graph);
	}
}


/* Adds an edge to a graph*/
void addEdge(Graph* graph, int src, int dest){
    /* Add an edge from src to dst in the adjacency list*/
	if(src==dest)
		return;
	node* newNode = createNode(dest);
	newNode->next = graph->adjListArr[src].head;
	graph->adjListArr[src].head = newNode;
	graph->adjListArr[src].num_elems++;
}

/*
void addEdge(Graph* graph, int src, dim dest){
	if(src==dest)
		return;
	
    node* newNode = createNode(dest);
    node* old = graph->adjListArr[src].tail;
    
    if(graph->adjListArr[src].head == NULL){
        graph->adjListArr[src].head = newNode;
        old = graph->adjListArr[src].head;
    }

    newNode->next = NULL;
    old->next = newNode;
	graph->adjListArr[src].tail = newNode;
}
*/

/* Function to print the adjacency list of graph*/
void displayGraph(Graph* graph){
	int i;
	for (i = 0; i < graph->num_vertices; i++){
    	node* adjListPtr = graph->adjListArr[i].head;
    	printf("\n%u: ", i);
    	while (adjListPtr){
        	printf("%d->", adjListPtr->vertex);
        	adjListPtr = adjListPtr->next;
    	}
    	printf("NULL\n");
	}
}

void DFS(Graph* graph, int vertex) {
	adj_list adjList = graph->adjListArr[vertex];
	node* temp = adjList.head;
	graph->visited[vertex] = 1;

	appendReach(graph,vertex);
	//printf("Visited %d \n", vertex);

	while(temp!=NULL){
		int edge = temp->vertex;
    	if(graph->visited[edge] == 0){
    		DFS(graph, edge);
    	}
    	temp = temp->next;
	}
}

void appendReach(Graph* graph, int vertex){
	node* newNode = createNode(vertex);
	node* old = graph->reach.tail;
    
	if(graph->reach.head == NULL){
    	graph->reach.head = newNode;
    	old = graph->reach.head;
	}

	newNode->next = NULL;
	old->next = newNode;
	graph->reach.tail = newNode;
}
