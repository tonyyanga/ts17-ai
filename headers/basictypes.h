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

class MaxHeap{
	// sort items with their pointers, by ptr->number
public:
	void* getmax(int n=1); // get the best several items
	lnNode* returnall();
	void additem(const void* item);
	void cleanup();
private:
	binaryNode* root();
	void heapsort();
};

#endif