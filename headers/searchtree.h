#ifndef SEARCHTREE
#define SEARCHTREE

#include "basic.h" // get basic structs
#include "instructions.h" // use basic data

typedef SceneState{
	// below, ptr?
	Map map;
	Status status;
};

class SearchNode{
public:
	int depth; // tree depth
	bool spanned;
	SearchNode* father; // pointer to father node
	Instruction* order;
	double number; // work with MaxHeap
private:
	lnNode* children; // linked list to child nodes
	const SceneState state;
	int gameover_state;

public:
	// constructor
	SearchNode(const SceneState* state);

	double evaluate(); // return evaluated value of this state
	SearchNode* getbestchild(); // return the best child
	lnNode* rankchildren(); // return sorted child linked list, the first should be the best
	lnNode* reverserankchildren(); // return sorted child linked list, the first should be the worst
	int span(); // span this node, return number of children
	int gameover(); //return 0 as not over yet, 1 as winning, 2 as losing
};

class SearchTree{
private:
	lnNode* HeapsAtDepth // linked list to store heaps for nodes at certain depth
	MaxHeap* EndedNodes // heap for ended nodes
	//bool Ended

public:
	//constructor
	SearchTree(const SceneState* rootstate, int BFSdepth=3, int DFSdepth=8);

	SearchNode* GetBestNode(int depth=-1);
	lnNode* GetAllNodes(int depth=-1);
private:
	SceneState* Estimate(const SceneState* origin, const Instruction* order);




#endif