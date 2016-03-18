#include <iostream>

#include "../headers/basictypes.h"
#include "../headers/searchtree.h"

SearchNode::SearchNode(const SceneState* state, const SearchNode* father, const Instruction* order) {
	this->state=state;
	this->father=father;
	this->order=order;
	this->spanned=false;
	this->number=this->evaluate();
	this->depth=father->depth+1;
}

bool SearchNode::haschildren() {
	return (this->children != NULL);
}

int SearchNode::span() {
	int count=0;
	lnNode* temp = this->CheckPossibleOrders();
	while(temp) {
		this->AddChild(this->Estimate((Instruction*)(temp->dataptr)), (Instruction*)(temp->dataptr));
		temp = temp->next;
		count++;
	}
	this->spanned=true;
	return count;
}

void SearchNode::AddChild(const SceneState* state, const Instruction* order) {
	SearchNode* child=&SearchNode(state, this, order);
	lnNode* temp = (lnNode*)malloc(sizeof(lnNode));
	temp->next=this->children;
	temp->dataptr=child;
	this->children=temp;
}

SceneState* SearchNode::Estimate(const Instruction* order) {

}