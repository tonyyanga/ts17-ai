#include <stdlib.h>
#include "../headers/basictypes.h"
#include "../headers/searchtree.h"

MaxHeap::MaxHeap() {
	this->root=NULL;
}

MaxHeap::~MaxHeap() {

}

void MaxHeap::additem(class SearchNode* item) {
	lnNode* temp=this->root;
	if (!temp) {
		root=(lnNode*)malloc(sizeof(lnNode));
		root->next=NULL;
		root->dataptr=item;
		return;
	}
	while(temp->next) {
		if (((MaxHeapNode*)(temp->dataptr))->number < item->number) {
			lnNode* newnode=(lnNode*)malloc(sizeof(lnNode));
			newnode->dataptr=temp->dataptr;
			newnode->next=temp->next;
			temp->next=newnode;
			temp->dataptr=item;
			return;
		}
		else
			temp=temp->next;
	}
	if (temp!=this->root||((MaxHeapNode*)(temp->dataptr))->number >= item->number) {
		lnNode* newnode=(lnNode*)malloc(sizeof(lnNode));
		newnode->dataptr=item;
		newnode->next=NULL;
		temp->next=newnode;
	} else {
		lnNode* newnode=(lnNode*)malloc(sizeof(lnNode));
		newnode->dataptr=item;
		newnode->next=this->root;
		this->root=newnode;
	}
}

lnNode* MaxHeap::returnall() {
	return this->root;//TODO: deep copy
}

MaxHeapNode* MaxHeap::getmax(int n) {
	MaxHeapNode* newroot=(MaxHeapNode*)malloc(n*sizeof(MaxHeapNode));
	lnNode* track=this->root;
	MaxHeapNode* temp=newroot;
	int count;
	for (count=0;count<=n-1;count++) {
		if (!track)
			break;
		temp=newroot+n;
		temp=(MaxHeapNode*)track->dataptr;
		track=track->next;
	}
	return newroot;
	// who shall clean up new root?
}