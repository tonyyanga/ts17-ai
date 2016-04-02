#include <iostream>

#include "../headers/common.h"
#include "../headers/basictypes.h"
#include "../headers/searchtree.h"

SearchTree::SearchTree(const SceneState* rootstate, int DFSdepth) {
	// init
	lnNode* HeapList;
	MaxHeap* newheap = new MaxHeap;
	this->root = new SearchNode(rootstate, NULL, NULL);
	this->DFSdepth = DFSdepth;

	HeapList=(lnNode*)malloc(sizeof(lnNode));
	HeapList->next=NULL;
	newheap->additem(root);
	HeapList->dataptr=newheap;
	this->HeapsAtDepth = HeapList;
	HeapList=(lnNode*)malloc(sizeof(lnNode));
	HeapList->next=NULL;
	HeapList->dataptr = new MaxHeap;
	this->EndedNodes = HeapList;
	std::cout<<"search tree finished"<<std::endl;
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

SearchNode* SearchTree::GetBestNode(int depth) {
	MaxHeap* heap = this->GetHeap(depth);
	return (SearchNode*)(heap->getmax());
}

lnNode* SearchTree::GetAllNodes(int depth) {
	MaxHeap* heap = this->GetHeap(depth);
	return (lnNode*)(heap->returnall());
}

bool SearchTree::BFS(int BFSdepth) {
	lnNode* ptr;
	int count;
	int result=0;
	for(count=1;count<=BFSdepth;count++){
		ptr = this->GetAllNodes(count-1);
		result+=search_layer_exec(ptr);
	}
	return result;
}

bool SearchTree::DFS(int width) {
	MaxHeap* heap;
	int depth=this->DFSdepth;
	void* temp;
	lnNode* ptr;
	int result=0;
	int count;
	while(depth <= this->DFSdepth) {
		heap = this->GetHeap(-1);
		temp = heap->getmax(width);
		if (width==1) {
			result += this->search_layer_exec(*(SearchNode**)temp);
			if (result)
				break;
			else
				depth = ((SearchNode*)temp)->depth + 1;
		} else {
			for(count=0;count<=width-1;count++)
				result += this->search_layer_exec((*(SearchNode**)temp+count));
			if (result) {
				int cancontinue=false;
				ptr = this->GetAllNodes();
				while(ptr) {
					cancontinue = cancontinue || (((SearchNode*)(ptr->dataptr))->haschildren());
					ptr = ptr->next;
				}
				if (!cancontinue)
					break;
			depth = ((SearchNode*)((lnNode*)temp)->dataptr)->depth + 1;
			}
		}
	}
	return result;
}

bool SearchTree::search_layer_exec(const lnNode* layerNodes) {
	lnNode* ptr = (lnNode*)layerNodes;
	lnNode* child;
	MaxHeap* heap=NULL;
	int depth;
	bool result=false;
	SearchNode* temp=(SearchNode*)ptr->dataptr;
	if (temp)
		depth = temp->depth + 1;
	else
		return false;
	while(ptr) {
		temp = (SearchNode*)(ptr->dataptr);
		temp->span();
		child = temp->rankchildren();
		while(child) {
			if (heap==NULL)
				heap = this->GetHeap(depth);
			if (heap==NULL)
			{
				lnNode* heapNode=this->HeapsAtDepth;
				lnNode* newNode=(lnNode*)malloc(sizeof(lnNode));
				while(heapNode->next)
					heapNode = heapNode->next;
				heapNode->next = newNode;
				newNode->next=NULL;
				newNode->dataptr = new MaxHeap;
				heap=(MaxHeap*)newNode->dataptr;
			}
			heap->additem((SearchNode*)child->dataptr);
			if (!((SearchNode*)(child->dataptr))->haschildren()) {
				lnNode* linkedlist = (lnNode*)malloc(sizeof(lnNode));
				linkedlist->next = this->EndedNodes;
				linkedlist->dataptr = child->dataptr;
				result = true;
				this->EndedNodes = linkedlist;
			}
			child = child->next;
		}
		ptr = ptr->next;
	}
	return result;
}

bool SearchTree::search_layer_exec(const SearchNode* node) {
	int depth;
	MaxHeap* heap=NULL;
	SearchNode* temp=(SearchNode*)node;
	lnNode* child;
	bool result=false;
	if (temp)
		depth = temp->depth + 1;
	else
		return false;
	temp->span();
	child = temp->rankchildren();
	while(child) {
		if (heap==NULL)
			heap = this->GetHeap(depth);
		if (heap==NULL)
			{
				lnNode* heapNode=this->HeapsAtDepth;
				lnNode* newNode=(lnNode*)malloc(sizeof(lnNode));
				while(heapNode->next)
					heapNode = heapNode->next;
				heapNode->next = newNode;
				newNode->next=NULL;
				newNode->dataptr = new MaxHeap;
				heap=(MaxHeap*)newNode->dataptr;
			}
		heap->additem((SearchNode*)child->dataptr);
		if (!((SearchNode*)(child->dataptr))->haschildren()) {
			lnNode* linkedlist = (lnNode*)malloc(sizeof(lnNode));
			linkedlist->next = this->EndedNodes;
			linkedlist->dataptr = child->dataptr;
			result = true;
			this->EndedNodes = linkedlist;
		}
		child = child->next;
	}
	return result;
}

MaxHeap* SearchTree::extendheap() {
	lnNode* heapNode=this->HeapsAtDepth;
	lnNode* newNode=(lnNode*)malloc(sizeof(lnNode));
	while(heapNode->next)
		heapNode = heapNode->next;
	heapNode->next = newNode;
	newNode->next=NULL;
	newNode->dataptr = new MaxHeap;
	return (MaxHeap*)newNode->dataptr;
}

SearchTree::~SearchTree() {

}