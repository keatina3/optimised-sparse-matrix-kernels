#ifndef _DAG_H_
#define _DAG_H_

/* Adjacency list node*/
typedef struct node
{
	int vertex;                /*Index to adjacency list array*/
	struct node *next; /*Pointer to the next node*/
}
node;

/* Adjacency list */
typedef struct adj_list
{
	int num_elems;			/*number of members in the list (for future use)*/
	node *head;					/*head of the adjacency linked list*/
}
adj_list;

/* Graph structure. A graph is an array of adjacency lists.
   Size of array will be number of vertices in graph*/
typedef struct Graph
{
	int num_vertices;         /*Number of vertices*/
	bool* visited;
	adj_list* adjListArr;     /*Adjacency lists' array*/
}
Graph;

node* createNode(dim v);
Graph* createGraph(dim n);
void freeGraph(Graph* graph);
void addEdge(Graph* graph, dim src, dim dest);
void displayGraph(Graph* graph);
void DFS(Graph* graph, dim vertex);

#endif
