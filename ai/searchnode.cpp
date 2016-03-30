#include <iostream>
#include <math.h>
#include "../headers/basictypes.h"
#include "../headers/searchtree.h"
#include "../headers/teamstyle17.h"
#include "../headers/common.h"

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
	{
		Position* positions=new Position[3];
		SceneState s=*state;
		analyzer temp_analyzer(&s);
		for (int i=0;i<3;i++)
		{
			positions[i].x=(temp_analyzer.best_way())[i].speed.x*100+state->status->objects[0].pos.x;
			positions[i].y=(temp_analyzer.best_way())[i].speed.y*100+state->status->objects[0].pos.y;
			positions[i].z=(temp_analyzer.best_way())[i].speed.z*100+state->status->objects[0].pos.z;
			t=new Instruction;
			t->type=InstructionType(MovePosition);
			t->argvs->dataptr=&positions[i];
			lnNode* temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
			l=l->next;
		}
		t=new Instruction;
		t->type=InstructionType(EatAdvancedEnergy);
		l->dataptr=t;
		l->next=NULL;
		return head;
	}
		

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
	//rate1: all skill points
	//rate2: total density of foods
	//rate3: time
	//rate4: distance to different positions in different cases
	SceneState s=*state;
	analyzer temp_analyzer(&s);
	int rate1,rate2,rate3;
	double food_density=temp_analyzer.best_way()[0].weight+temp_analyzer.best_way()[1].weight+temp_analyzer.best_way()[2].weight;
	int extra_ability=0;
	for (int i=SkillType(LONG_ATTACK);i<=SkillType(HEALTH_UP);i++)
	{
		for (int k=0;k<state->status->objects[0].skill_level[i];k++)
			extra_ability+=kBasicSkillPrice[i]*pow(2,k);
	}
	//Enemy in sight
	if ((sizeof(state->map->objects)/sizeof(Object)==1)&&(state->map->objects[0].team_id>=0))
	{
		//initializing data
		int possible_extra_hp,hp_price=pow(2,state->status->objects[0].skill_level[SkillType(HEALTH_UP)]);
		int temp_ability=state->status->objects[0].ability;
		double kill_rate=pow(1.2,3),death_rate=1/kill_rate;
		while (temp_ability>hp_price)
		{
			possible_extra_hp+=500;
			temp_ability-=hp_price;
			hp_price=2*hp_price;
		}
		double original_hp_rate=state->status->objects[0].health/health(state->map->objects[0].radius);
		double extra_hp_rate_self=(state->status->objects[0].health+possible_extra_hp)/health(state->map->objects[0].radius);
		double distance=Distance(state->status->objects[0].pos,state->map->objects[0].pos);


		//setting game over state
		gameover_state=0;
		if ((original_hp_rate<=death_rate&&distance<state->map->objects[0].radius)||(state->status->objects[0].health<0.25*state->status->objects[0].max_health))
			gameover_state=2;
		if ((original_hp_rate>=kill_rate&&distance<state->map->objects[0].radius)||(health(state->map->objects[0].radius)<0.25*(health(state->enemy->maxr))))
			gameover_state=1;


		//evaluation
		if (1/*how to know that we can defeat the enemy?)*/)
		{
			int rate4;
			int rate5,rate6;//rate5: shield status, rate6: dash status
			return original_hp_rate*state->status->objects[0].health+
				rate1*(state->status->objects[0].ability-3/* 3 should be ability points of enemy, do we have that?*/)+
				rate4*distance+
				rate5*(state->status->objects[0].shield_time>0)+
				rate6*(state->status->objects[0].dash_time>0);
		}
		else
		{
			int rate4;
			int rate5,rate6;//rate5: shield status, rate6: dash status
			return original_hp_rate*state->status->objects[0].health+
				rate4*distance+
				rate5*(state->status->objects[0].shield_time>0)+
				rate6*(state->status->objects[0].dash_time>0);
		}
	}
	else if (sizeof(state->map->objects)/sizeof(Object)==0)									//Regular mode
	{
		int rate4;
		return state->status->objects[0].health+
			rate1*extra_ability+
			rate2*food_density+
			rate3*(state->map->time); //distance to center?
	}
	else if((sizeof(state->map->objects)/sizeof(Object)==1)&&(state->map->objects[0].team_id==-2))//Only boss in sight
	{
		//initializing data
		int rate4;
		int possible_extra_hp,hp_price=pow(2,state->status->objects[0].skill_level[SkillType(HEALTH_UP)]);
		int temp_ability=state->status->objects[0].ability;
		double kill_rate=pow(1.2,3),death_rate=1/kill_rate;
		while (temp_ability>hp_price)
		{
			possible_extra_hp+=500;
			temp_ability-=hp_price;
			hp_price=2*hp_price;
		}
		double original_hp_rate=state->status->objects[0].health/health(state->map->objects[0].radius);
		double extra_hp_rate_self=(state->status->objects[0].health+possible_extra_hp)/health(state->map->objects[0].radius);
		double distance=Distance(state->status->objects[0].pos,state->map->objects[0].pos);

		//setting game over state
		gameover_state=0;
		if ((original_hp_rate<=death_rate&&distance<state->map->objects[0].radius)||(state->status->objects[0].health<0.25*state->status->objects[0].max_health))
			gameover_state=2;
		if ((original_hp_rate>=kill_rate&&distance<state->map->objects[0].radius)||(health(state->map->objects[0].radius)<0.25*(health(state->enemy->maxr))))
			gameover_state=1;

		//evaluating
		return state->status->objects[0].health+
			rate1*extra_ability-rate3*(state->map->time)+
			rate2*food_density+
			rate3*state->map->time+
			rate4*distance;
	}
	else																					  //Both enemy and boss in sight
	{
		//initializing data
		int possible_extra_hp,hp_price=pow(2,state->status->objects[0].skill_level[SkillType(HEALTH_UP)]);
		int temp_ability=state->status->objects[0].ability;
		double kill_rate=pow(1.2,3),death_rate=1/kill_rate;
		while (temp_ability>hp_price)
		{
			possible_extra_hp+=500;
			temp_ability-=hp_price;
			hp_price=2*hp_price;
		}
		int boss_id;
		double Enemy_original_hp_rate;
		double Enemy_extra_hp_rate_self;
		double Enemy_distance;
		double Boss_original_hp_rate;
		double Boss_extra_hp_rate_self;
		double Boss_distance;
		if (state->map->objects[0].id==-2)
		{
			boss_id=0;
			double Boss_original_hp_rate=state->status->objects[0].health/health(state->map->objects[0].radius);
			double Boss_extra_hp_rate_self=(state->status->objects[0].health+possible_extra_hp)/health(state->map->objects[0].radius);
			double Boss_distance=Distance(state->status->objects[0].pos,state->map->objects[0].pos);
			double Enemy_original_hp_rate=state->status->objects[0].health/health(state->map->objects[1].radius);
			double Enemy_extra_hp_rate_self=(state->status->objects[0].health+possible_extra_hp)/health(state->map->objects[1].radius);
			double Enemy_distance=Distance(state->status->objects[0].pos,state->map->objects[1].pos);
		}
		else
		{
			boss_id=1;
			double Enemy_original_hp_rate=state->status->objects[0].health/health(state->map->objects[0].radius);
			double Enemy_extra_hp_rate_self=(state->status->objects[0].health+possible_extra_hp)/health(state->map->objects[0].radius);
			double Enemy_distance=Distance(state->status->objects[0].pos,state->map->objects[0].pos);
			double Boss_original_hp_rate=state->status->objects[0].health/health(state->map->objects[1].radius);
			double Boss_extra_hp_rate_self=(state->status->objects[0].health+possible_extra_hp)/health(state->map->objects[1].radius);
			double Boss_distance=Distance(state->status->objects[0].pos,state->map->objects[1].pos);
		}

		//setting game over state
		gameover_state=0;
		if ((Boss_original_hp_rate<=death_rate&&Boss_distance<state->map->objects[0].radius)||
			(state->status->objects[0].health<0.25*state->status->objects[0].max_health)||
			(Enemy_original_hp_rate<=death_rate&&Enemy_distance<state->map->objects[0].radius))
			gameover_state=2;
		if ((Boss_original_hp_rate>=kill_rate&&Boss_distance<state->map->objects[0].radius)||
			(health(state->map->objects[1-boss_id].radius)<0.25*(health(state->enemy->maxr)))||
			(Enemy_original_hp_rate>=kill_rate&&Enemy_distance<state->map->objects[0].radius)||
			(health(state->map->objects[boss_id].radius)<1))
			gameover_state=1;
	}
}