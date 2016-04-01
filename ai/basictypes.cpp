#include <stdlib.h>
#include "../headers/basictypes.h"

MaxHeap::MaxHeap() {
	this->root=NULL;
}

MaxHeap::~MaxHeap() {

}

void MaxHeap::additem(MaxHeapNode* item) {
	lnNode* temp=root;
	while(temp) {
		if (((MaxHeapNode*)(temp->dataptr))->number < item->number)
			break;
		else
			temp=temp->next;
	}
	if (temp) {
		lnNode* newnode=(lnNode*)malloc(sizeof(lnNode));
		newnode->dataptr=temp->dataptr;
		newnode->next=temp->next;
		temp->next=newnode;
		temp->dataptr=item;
	} else {
		temp=(lnNode*)malloc(sizeof(lnNode));
		temp->next=NULL;
		temp->dataptr=item;
	}
}

lnNode* MaxHeap::returnall() {
	return this->root;
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