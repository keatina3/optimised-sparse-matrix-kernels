#ifndef _DAG_H_
#define _DAG_H_

//---------------------------------------------------------------------//
// VARIOUS FUNCTIONALITY RELATING TO DIRECTED ACYCLIC GRAPH & REACHSET //
//---------------------------------------------------------------------//


// DATA STRUCTURES//

// nodes for reachset //
typedef struct node
{
	dim vertex;               
	struct node *next; 
	struct node *prev;
}
node;

// linked list representing reachset //
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
	reachset reach;
}
Graph;


// FUNCTIONS //

node* createNode(dim v);
Graph* createGraph(dim n);
void freeGraph(Graph* graph);
void appendSet(reachset* reach, dim vertex);
void DFS(mat_mar* L, Graph* graph, dim vertex);
Graph* getReach(mat_mar* L, mat_mar* b);

#endif
