#ifndef SEARCHTREE
#define SEARCHTREE

#include "basic.h" // get basic structs
#include "basictypes.h" // use inheritance
#include "instructions.h" // use basic data

typedef struct SceneState{
	// below, ptr?
	Map map;
	Status status;
};

class SearchNode:public MaxHeapNode {
public:
	int depth; // tree depth
	bool spanned;
	const SearchNode* father; // pointer to father node
	const Instruction* order;
	double number; // work with MaxHeap
private:
	lnNode* children; // linked list to child nodes
	const SceneState* state;
	int gameover_state;

public:
	// constructor
	SearchNode(const SceneState* state, const SearchNode* father, const Instruction* order);
	~SearchNode();

	double evaluate(); // return evaluated value of this state AND SET gameover_state
	SearchNode* getbestchild(); // return the best child
	lnNode* rankchildren(); // return sorted child linked list, the first should be the best
	lnNode* reverserankchildren(); // return sorted child linked list, the first should be the worst
	int span(); // span this node, return number of children
	int gameover(); //return 0 as not over yet, 1 as winning, 2 as losing
	bool haschildren();

private:
	SceneState* Estimate(const Instruction* order); // calculate the next state
	lnNode* CheckPossibleOrders(); // return a linked list of possible orders
	void AddChild(const SceneState* state, const Instruction* Order);
};

class SearchTree{
private:
	SearchNode* root;
	lnNode* HeapsAtDepth; // linked list to store heaps for nodes at certain depth
	lnNode* EndedNodes; // heap for ended nodes pointers
	//bool Ended
	int DFSdepth;

public:
	//constructor
	SearchTree(const SceneState* rootstate, int maxDepth); // root is at depth = 0
	~SearchTree();
	
	//return True when there are end nodes
	bool BFS(int BFSdepth);
	bool DFS(int width);
	
	SearchNode* GetBestNode(int depth);
	lnNode* GetAllNodes(int depth);
private:
	MaxHeap* GetHeap(int depth);

	bool search_layer_exec(const lnNode* layerNodes);
	bool search_layer_exec(const SearchNode* node);
	MaxHeap* extendheap();
};


#endif