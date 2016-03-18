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
public:
	void* getmax();
	void additem(const void* item);
	void cleanup();
private:
	binaryNode* root();
	void heapsort();
};

#endif