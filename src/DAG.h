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
	dim numElems;
}
reachset;

typedef struct Graph
{
	bool* visited;
	//dim critPath;
	//dim* depth;
	reachset reach;
}
Graph;

node* createNode(dim v);
Graph* createGraph(dim n);
void freeGraph(Graph* graph);
void appendSet(reachset* reach, dim vertex);
void DFS(mat_mar* L, Graph* graph, dim vertex, dim count);
Graph* getReach(mat_mar* L, mat_mar* b);

#endif
