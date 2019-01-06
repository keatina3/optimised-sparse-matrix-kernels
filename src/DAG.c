#include <stdio.h>
#include <stdlib.h>
#include "mm.h"
#include "DAG.h"

/* Function to create an adjacency list node*/
node* createNode(dim v){
	node* newNode = (node*)malloc(sizeof(node));
	if(!newNode)
	    printf("Error creating node\n");
		//err_exit("Unable to allocate memory for new node");

	newNode->vertex = v;
	newNode->next = NULL;

	return newNode;
}

/* Function to create a graph with n vertices; Creates both directed and undirected graphs*/
Graph* createGraph(dim n){
	dim i;
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	if(!graph)
		printf("Error allocating DAG.\n");
		//err_exit("Unable to allocate memory for graph");

	graph->num_vertices = n;
	graph->visited = (bool*)calloc(graph->num_vertices,sizeof(bool));
    
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
			dim v;
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
void addEdge(Graph* graph, dim src, dim dest){
    /* Add an edge from src to dst in the adjacency list*/
	if(src==dest)
		return;
	node* newNode = createNode(dest);
	newNode->next = graph->adjListArr[src].head;
	graph->adjListArr[src].head = newNode;
	graph->adjListArr[src].num_elems++;

}

/* Function to print the adjacency list of graph*/
void displayGraph(Graph* graph){
    dim i;
    for (i = 0; i < graph->num_vertices; i++)
    {
        node* adjListPtr = graph->adjListArr[i].head;
        printf("\n%lu: ", i);
        while (adjListPtr)
        {
            printf("%d->", adjListPtr->vertex);
            adjListPtr = adjListPtr->next;
        }
        printf("NULL\n");
    }
}

void DFS(Graph* graph,dim vertex) {
	adj_list adjList = graph->adjListArr[vertex];
    node* temp = adjList.head;
    graph->visited[vertex] = 1;
    //printf("Visited %d \n", vertex);

    while(temp!=NULL) {
        dim connectedVertex = temp->vertex;
        if(graph->visited[connectedVertex] == 0) {
            DFS(graph, connectedVertex);
        }
        temp = temp->next;
    }
}
