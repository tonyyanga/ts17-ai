#include <iostream>

#include "../headers/basictypes.h"
#include "../headers/searchtree.h"

SearchTree::SearchTree(const SceneState* rootstate, int BFSdepth=3, int DFSdepth=8) {
	// init
	lnNode* HeapList;
	this->root = &SearchNode(rootstate, NULL, NULL);
	this->BFSdepth = BFSdepth;
	this->DFSdepth = DFSdepth;

	HeapList=(lnNode*)malloc(sizeof(lnNode));
	HeapList->next=NULL;
	HeapList->dataptr = root;
	this->HeapsAtDepth = HeapList;
	HeapList=(lnNode*)malloc(sizeof(lnNode));
	HeapList->next=NULL;
	HeapList->dataptr = NULL;
	this->EndedNodes = HeapList;
}

MaxHeap* SearchTree::GetHeap(int depth) {
	int count=0;
	lnNode* ptr=this->HeapsAtDepth;
	while(ptr->next) {
		count++;
		ptr = ptr->next;
		if (count==depth)
			break;
	}
	if (count!=depth && depth != -1)
		return NULL;
	else
		return (MaxHeap*)ptr->dataptr;
}

SearchNode* SearchTree::GetBestNode(int depth=-1) {
	MaxHeap* heap = this->GetHeap(depth);
	return (SearchNode*)(heap->getmax());
}

lnNode* SearchTree::GetAllNodes(int depth=-1) {
	MaxHeap* heap = this->GetHeap(depth);
	return (lnNode*)(heap->returnall());
}