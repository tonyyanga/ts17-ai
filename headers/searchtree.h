#ifndef SEARCHTREE
#define SEARCHTREE

#include "basic.h" // get basic structs
#include "instructions.h" // use basic data

class SearchNode{
public:
	int depth; // tree depth
	bool spanned;
	SearchNode* father; // pointer to father node
private:
	lnNode* children; // linked list to child nodes
	Map map;
	Status status;
public:
	double evaluate(); // return evaluated value of this state
	SearchNode* getbestchild(); // return the best child
	lnNode* rankchildren(); // return sorted child linked list, the first should be the best
	lnNode* reverserankchildren(); // return sorted child linked list, the first should be the worst
	int span(); // span this node
};


#endif