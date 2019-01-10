#ifndef _DAG_H_
#define _DAG_H_

//--------------------------------------------------------------------------------//
// VARIOUS FUNCTIONALITY RELATING TO DIRECTED ACYCLIC GRAPH, REACHSET & LEVELSETS //
//--------------------------------------------------------------------------------//

// DATA STRUCTURES //

// nodes for reachset/levelsets //
typedef struct node
{
	dim vertex;               
	struct node *next; 
	struct node *prev;
}
node;

// linked list for reachset/ each level in levelset //
typedef struct reachset
{
	node *head;
	node *tail;
	dim numElems;
}
reachset;

// struct containing array of linked list for each level in levelset & its cardinalty //
typedef struct levelSet
{
	dim numLevels;
	reachset* level_ptr;
}
levelSet;

// arrays containitng visited status & depth of each node, crit path of DAG and reachset ptr //
typedef struct Graph
{
	bool* visited;
	dim critPath;
	dim* depth;
	reachset reach;
}
Graph;


// FUNCTIONS //

node* createNode(dim v);
Graph* createGraph(dim n);
void freeGraph(Graph* graph);
void freeLevelSet(levelSet* G);
void appendSet(reachset* reach, dim vertex);
void DFS(mat_mar* L, Graph* graph, dim vertex, dim count);
void adjustDepth(mat_mar* L, Graph* graph, dim vertex, dim count);
Graph* getReach(mat_mar* L, mat_mar* b);
levelSet* assignLevelSet(Graph* graph);

#endif
