#ifndef _DAG_H_
#define _DAG_H_

/* Adjacency list node*/
typedef struct node
{
	int vertex;                /*Index to adjacency list array*/
	struct node *next; /*Pointer to the next node*/
	struct node *prev;
}
node;

/* Adjacency list */
typedef struct reachset
{
	node *head;					/*head of the adjacency linked list*/
	node *tail;
}
reachset;

/* Graph structure. A graph is an array of adjacency lists.
   Size of array will be number of vertices in graph*/
typedef struct Graph
{
	bool* visited;
	reachset reach;
}
Graph;

node* createNode(int v);
Graph* createGraph(int n);
void freeGraph(Graph* graph);
void appendReach(Graph* graph, int vertex);
void DFS(mat_mar* L, Graph* graph, int vertex);

#endif
