#ifndef SEARCHTREE
#define SEARCHTREE

#include "basic.h" // get Position and Speed structs
#include "instructions.h" // use basic data

class SearchNode {
public:
	int depth;
	SearchNode* father;
private:
	lnNode* children;

public:
	double evaluate();

};


#endif