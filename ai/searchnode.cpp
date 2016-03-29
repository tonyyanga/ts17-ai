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

lnNode* SearchNode::CheckPossibleOrders()
{
	lnNode* l=new lnNode;
	lnNode* head=l;
	Instruction* t;
	if (sizeof(this->state->map->objects)/sizeof(Object))
	{
		for (int k=InstructionType(EatAdvancedEnergy);k<=InstructionType(Skill_UpgradeSkill);k++)
		{
			t=new Instruction;
			t->type=k;
			if (k==InstructionType(Approach))
			{
				t->argvs->dataptr=new double(state->status->objects[0].radius);
			}
			if (k==InstructionType(Escape))
			{
				t->argvs->dataptr=new double(5000);
			}
			lnNode* temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
			l=l->next;
		}
		for (int k=SkillType(LONG_ATTACK);k<SkillType(HEALTH_UP);k++)
		{
			t=new Instruction;
			t->type=InstructionType(Skill_UpgradeSkill);
			t->argvs->dataptr=new int(k);
			lnNode* temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
			l=l->next;
		}
		l=NULL;
		return head;
}
	else
		;
}

double SearchNode::evaluate()
{
	//Self: hp, skill point(different skills converted to ability), density_max, time
	//Enemy: hp, skill point
	//Boss: hp
	//Regular mode: density_max, hp, skill point, time ,distance to center
	//Battle mode: Power evaluating: hp_self/hp_enemy (0--1), self skill point-enemy skill point
	//			   If weaker:	Distance, Shield, Dash status
	//			   If stronger: Distance, Shield, Dash status, attack level
	//Boss mode:   Works after a certain time or if boss's hp is low enough
	if ((sizeof(state->map->objects)/sizeof(Object)==1)&&(state->map->objects[0].team_id>=0))//Enemy in sight
	{
		;																					
	}
	else if (sizeof(state->map->objects)/sizeof(Object)==0)									//Regular mode
	{
		int rate1,rate2,rate3;
		int extra_ability=0;
		for (int i=SkillType(LONG_ATTACK);i<=SkillType(HEALTH_UP);i++)
			for (int k=0;k<state->status->objects[0].skill_level[i];k++)
				extra_ability+=kBasicSkillPrice[k];
		return state->status->objects[0].health+rate1*extra_ability-rate3*(state->map->time); //+rate2*density and distance to center
	}
	else if((sizeof(state->map->objects)/sizeof(Object)==1)&&(state->map->objects[0].team_id==-2))//Boss in sight
	{
		int rate1,rate2,rate3;
		int extra_ability=0;
		for (int i=SkillType(LONG_ATTACK);i<=SkillType(HEALTH_UP);i++)
			for (int k=0;k<state->status->objects[0].skill_level[i];k++)
				extra_ability+=kBasicSkillPrice[k];
		return state->status->objects[0].health+rate1*extra_ability-rate3*(state->map->time); //+rate2*density, distance to boss, boss's hp
	}
	else																					  //Both enemy and boss in sight
	{
		;
	}
}