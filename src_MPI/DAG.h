#ifndef _DAG_H_
#define _DAG_H_

typedef struct node
{
	dim vertex;               
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
	dim* depth;
	dim reachCard;
	reachset reach;
}
Graph;

node* createNode(dim v);
Graph* createGraph(dim n);
void freeGraph(Graph* graph);
void appendReach(Graph* graph, dim vertex);
void DFS(mat_mar* L, Graph* graph, dim vertex, dim count);
void adjustDepth(mat_mar* L, Graph* graph, dim vertex, dim count);
Graph* getReach(mat_mar* L, mat_mar* b);

#endif
