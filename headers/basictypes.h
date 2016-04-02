#ifndef BASICTYPES
#define BASICTYPES

typedef struct lnNode{
	// a basic node of a linked list
	// make .next = NULL to make the end
	void* dataptr;
	lnNode* next;
};

typedef struct binaryNode{
	// a basic node of a linked list
	// make .next = NULL to make the end
	void* dataptr;
	lnNode* left;
	lnNode* right;
};

class MaxHeapNode{
public:
	double number;
};

class MaxHeap{
	// sort items with their pointers, by ptr->number

	// for temporary implementation, use insert sort
public:
	//constructor
	MaxHeap();
	~MaxHeap();

	// get the best several items, item pointer when n=1, linkedlistNode when n>1
	MaxHeapNode** getmax(int n=1);
	lnNode* returnall();
	void additem(class SearchNode* item);

private:
	lnNode* root;
};

#endif