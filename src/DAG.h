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
typedef struct adj_list
{
	int num_elems;			/*number of members in the list (for future use)*/
	node *head;					/*head of the adjacency linked list*/
	node *tail;
}
adj_list;

/* Graph structure. A graph is an array of adjacency lists.
   Size of array will be number of vertices in graph*/
typedef struct Graph
{
	int num_vertices;         /*Number of vertices*/
	bool* visited;
	adj_list reach;
	adj_list* adjListArr;     /*Adjacency lists' array*/
}
Graph;

node* createNode(int v);
Graph* createGraph(int n);
void freeGraph(Graph* graph);
void addEdge(Graph* graph, int src, int dest);
void displayGraph(Graph* graph);
void DFS(Graph* graph, int vertex);
void appendReach(Graph* graph, int vertex);

#endif
