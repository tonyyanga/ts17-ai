#include <iostream>

#include "../headers/basictypes.h"
#include "../headers/searchtree.h"

SearchTree::SearchTree(const SceneState* rootstate, int DFSdepth=8) {
	// init
	lnNode* HeapList;
	this->root = &SearchNode(rootstate, NULL, NULL);
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

bool SearchTree::BFS(int BFSdepth=3) {
	lnNode* ptr;
	int count;
	bool result=false;
	for(count=1;count<=BFSdepth;count++){
		ptr = this->GetAllNodes();
		result = result || search_layer_exec(ptr);
	}
	return result;
}

bool SearchTree::DFS(int width=1) {
	MaxHeap* heap;
	int depth=this->DFSdepth;
	void* temp;
	lnNode* ptr;
	bool result=false;
	while(depth <= this->DFSdepth) {
		heap = this->GetHeap(-1);
		temp = heap->getmax(width);
		if (width==1) {
			result = result || this->search_layer_exec((SearchNode*) temp);
			if (result)
				break;
			else
				depth = ((SearchNode*)temp)->depth + 1;
		} else {
			result = result || this->search_layer_exec((lnNode*) temp);
			if (result) {
				int cancontinue=false;
				ptr = this->GetAllNodes();
				while(ptr->next) {
					cancontinue = cancontinue || (((SearchNode*)(ptr->dataptr))->haschildren());
					ptr = ptr->next;
				}
				if (!cancontinue)
					break;
			depth = ((SearchNode*)((lnNode*)temp)->dataptr)->depth + 1;
			}
		}
	}
}

bool SearchTree::search_layer_exec(const lnNode* layerNodes) {
	lnNode* ptr = (lnNode*)layerNodes;
	lnNode* child;
	SearchNode* temp=(SearchNode*)ptr->dataptr;
	if (temp)
		int depth = temp->depth + 1;
	else
		return false;
	while(ptr->next) {
		temp = (SearchNode*)(ptr ->dataptr);
		temp->span();
		child = temp->rankchildren();
		while(child->next) {
			//add to heap
			child = child->next;
		}
		ptr = ptr->next;
	}
}

bool SearchTree::search_layer_exec(const SearchNode* node) {

}