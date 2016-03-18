#ifndef SEARCHTREE
#define SEARCHTREE

#include "basic.h" // get basic structs
#include "instructions.h" // use basic data

typedef struct SceneState{
	// below, ptr?
	Map map;
	Status status;
};

class SearchNode{
public:
	int depth; // tree depth
	bool spanned;
	const SearchNode* father; // pointer to father node
	const Instruction* order;
	double number; // work with MaxHeap
private:
	lnNode* children; // linked list to child nodes
	const SceneState state;
	int gameover_state;

public:
	// constructor
	SearchNode(const SceneState* state, const SearchNode* father, const Instruction* order);

	double evaluate(); // return evaluated value of this state
	SearchNode* getbestchild(); // return the best child
	lnNode* rankchildren(); // return sorted child linked list, the first should be the best
	lnNode* reverserankchildren(); // return sorted child linked list, the first should be the worst
	int span(); // span this node, return number of children
	int gameover(); //return 0 as not over yet, 1 as winning, 2 as losing
};

class SearchTree{
private:
	SearchNode* root;
	lnNode* HeapsAtDepth; // linked list to store heaps for nodes at certain depth
	lnNode* EndedNodes; // heap for ended nodes
	//bool Ended
	int BFSdepth;
	int DFSdepth;

public:
	//constructor
	SearchTree(const SceneState* rootstate, int BFSdepth=3, int DFSdepth=8); // root is at depth = 0
	bool BFS();
	bool DFS();
	SearchNode* GetBestNode(int depth=-1);
	lnNode* GetAllNodes(int depth=-1);
private:
	SceneState* Estimate(const SceneState* origin, const Instruction* order);
	MaxHeap* GetHeap(int depth)
};




#endif