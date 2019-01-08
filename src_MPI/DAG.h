#ifndef _DAG_H_
#define _DAG_H_

typedef struct node
{
	int vertex;               
	struct node *next; 
	struct node *prev;
}
node;

typedef struct reachset
{
	node *head;
	node *tail;
}
reachset;

typedef struct Graph
{
	bool* visited;
	int* depth;
	dim reachCard;
	reachset reach;
}
Graph;

node* createNode(int v);
Graph* createGraph(int n);
void freeGraph(Graph* graph);
void appendReach(Graph* graph, int vertex);
void DFS(mat_mar* L, Graph* graph, int vertex);
Graph* getReach(mat_mar* L, mat_mar* b);

#endif
