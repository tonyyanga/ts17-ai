#include <iostream>
#include <cmath>
#include "../headers/basictypes.h"
#include "../headers/searchtree.h"
#include "../headers/teamstyle17.h"
#include "../headers/analyzer.h"
#include "../headers/evaluate_rates.h"

int SearchNode::count=0;

SearchNode::SearchNode(const SceneState* state, SearchNode* father, Instruction* order) {
	this->state=state;
	this->father=father;
	this->order=order;
	this->spanned=false;
	this->children=NULL;
	this->number=this->evaluate();
	if (father) {
		this->depth=father->depth+1;
	} else {
		this->depth=0;
	}
	count++;
	std::cout<<count<<std::endl;
}

SearchNode::~SearchNode() {

}

#ifdef SORTING
lnNode* SearchNode::rankchildren() {
	lnNode* temp=this->children;
	MaxHeap sorter;
	if (!temp)
		return NULL;
	while(temp->next) {
		sorter.additem(static_cast<MaxHeapNode*>(temp->dataptr));
		temp=temp->next;
	}
	return sorter.returnall();
}
#else
lnNode* SearchNode::rankchildren() {
	return this->children;
}
#endif

lnNode* SearchNode::getInstructionChain() {
	lnNode* temp;
	lnNode* root;
	SearchNode* track=this->father;
	if (this->order) {
		root=(lnNode*)malloc(sizeof(lnNode));
		root->next=NULL;
		root->dataptr=this->order;
		while(track->order) {
			temp=(lnNode*)malloc(sizeof(lnNode));
			temp->dataptr=track->order;
			temp->next=root;
			root=temp;
			track=track->father;
		}
		return root;
	} else
		return NULL;
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

void SearchNode::AddChild(const SceneState* state, Instruction* order) {
	SearchNode* child=new SearchNode(state, this, order);
	lnNode* temp = (lnNode*)malloc(sizeof(lnNode));
	temp->next=this->children;
	temp->dataptr=child;
	this->children=temp;
}

SceneState* SearchNode::Estimate(const Instruction* order) {
	SceneState* estimate=new SceneState(*state);
	return (estimate);
	int t,s=0,n=0;
	double heal;
	Position* new_position;
	int type=order->type;
	lnNode *argv=order->argvs;
	Status status=*(state->status);
	PlayerObject* Self=status.objects;
	Map map=*state->map;
	Enemy enemy=*state->enemy;
	Boss boss=*state->boss;
	switch(order->type)
	{
	case 0:
		{
			new_position=(Position *)(argv->dataptr);
			map.time+=Usetime(!Self->skill_cd[DASH],*new_position,Self->pos,*Self);
			estimate->map=&map;
			Self->pos.x=new_position->x;
			Self->pos.y=new_position->y;
			Self->pos.z=new_position->z;
			break;
		}
	case 1:
		{
			new_position=(Position *)(argv->dataptr);
			map.time+=Usetime(!Self->skill_cd[DASH],*new_position,Self->pos,*Self);
			estimate->map=&map;
			Self->pos.x=new_position->x;
			Self->pos.y=new_position->y;
			Self->pos.z=new_position->z;
			Self->ability+=3;
			estimate->map=NULL;
			break;
		}
	case 2:
		{
			new_position=(Position *)(argv->dataptr);
			map.time+=Usetime(!Self->skill_cd[DASH],*new_position,Self->pos,*Self);
			estimate->map=&map;
			Self->pos.x=new_position->x;
			Self->pos.y=new_position->y;
			Self->pos.z=new_position->z;
		}
	case 3:
		{
			new_position=(Position *)(argv->dataptr);
			map.time+=Usetime(!Self->skill_cd[DASH],*new_position,Self->pos,*Self);
			estimate->map=&map;
			Self->pos.x=new_position->x;
			Self->pos.y=new_position->y;
			Self->pos.z=new_position->z;
		}
	case 5:
		{
			Self->skill_cd[0]=80;
			if (Norm(Displacement(enemy.player.pos,Self->pos))<=(2000+500*(Self->skill_level[0])+Self->radius+enemy.player.radius))
				{
					enemy.Health-=100*(Self->skill_level[0]);
					enemy.player.radius=get_radius(enemy.Health);
			}
			if (Norm(Displacement(boss.boss.pos,Self->pos))<=(2000+500*(Self->skill_level[0])+Self->radius+boss.boss.radius))
			{
				heal=health(boss.boss.radius);
				heal-=100*(Self->skill_level[0]);
				boss.boss.radius=get_radius(heal);
			}
			map.time+=1;
			estimate->map=&map;
			break;
		}
	case 4:
		{
			Self->skill_cd[1]=80;
			if (Norm(Displacement(enemy.player.pos,Self->pos))<=(1100+300*(Self->skill_level[1])+Self->radius+enemy.player.radius))
				{
					enemy.Health-=200+300*(Self->skill_level[1]);
					enemy.player.radius=get_radius(enemy.Health);
			}
			if (Norm(Displacement(boss.boss.pos,Self->pos))<=(1100+300*(Self->skill_level[1]+Self->radius+boss.boss.radius)))
			{
				heal=health(boss.boss.radius);
				heal-=200+300*(Self->skill_level[1]);
				boss.boss.radius=get_radius(heal);
			}
			map.time+=1;
			estimate->map=&map;
			break;
		}
	case 7:
		{
			Self->shield_time=20+10*Self->skill_level[2];
			Self->skill_cd[2]=100;
			map.time+=1;
			estimate->map=&map;
			break;
		}
	case 6:
		{
			Self->skill_cd[3]=100;
			map.time+=1;
			estimate->map=&map;
			break;
		}
	case 8:
		{
			Self->health+=500;
			if (Self->skill_level[5])
				Self->ability-=pow(2,Self->skill_level[5]);
			else
			{
				for(t=0;t<6;t++)
				{
					if (Self->skill_level[t]) s++;
					Self->ability-=pow(2,s);
				}
			}
			Self->skill_level[5]++;
			map.time+=1;
			estimate->map=&map;
			break;
		}
	case 9:
		{
			n=*(int *)(argv->dataptr);
			if (Self->skill_level[n])
			{
				if (n==2 || n==4)
					Self->ability-=2*pow(2,Self->skill_level[n]);
				else 
					Self->ability-=pow(2,Self->skill_level[n]);
			}
			else
			{
				for(t=0;t<6;t++)
				{
					if (Self->skill_level[t]) s++;
					if (n==2 || n==4)
						Self->ability-=2*pow(2,s);
					else 
						Self->ability-=pow(2,s);
				}
			}
			Self->skill_level[n]++;
			map.time+=1;
			estimate->map=&map;
			break;
		}
	}
	estimate->status=&status;
	estimate->boss=&boss;
	estimate->enemy=&enemy;
	return(estimate);
}

lnNode* SearchNode::CheckPossibleOrders()
{
	lnNode* l=new lnNode;
	lnNode* head=l;
	Instruction* t;
	int time=GetTime();
	lnNode* temp;

	if ((state->boss->valid_time==time)||(state->enemy->valid_time==time))
	{
		Position* positions=new Position[3];
		SceneState s=*state;
		analyzer temp_analyzer(&s);
		for (int k=SkillType(LONG_ATTACK);k<=SkillType(HEALTH_UP);k++)
		{
			if (state->status->objects[0].ability>int(pow(2,state->status->objects[0].skill_level[k])*kBasicSkillPrice[k]))
			{
				t=new Instruction;
				t->type=InstructionType(Skill_UpgradeSkill);
				lnNode* n=new lnNode;
				t->argvs=n;
				t->argvs->dataptr=new int(k);                        //*****************************************//
				temp=new lnNode;
				l->dataptr=t;
				l->next=temp;
				l=l->next;
			}
		}
		for (int i=0;i<3;i++)
		{
			positions[i].x=(temp_analyzer.best_way())[i].speed.x*100+state->status->objects[0].pos.x;
			positions[i].y=(temp_analyzer.best_way())[i].speed.y*100+state->status->objects[0].pos.y;
			positions[i].z=(temp_analyzer.best_way())[i].speed.z*100+state->status->objects[0].pos.z;
			t=new Instruction;
			t->type=InstructionType(MovePosition);
			lnNode* n=new lnNode;
			t->argvs=n;
			t->argvs->dataptr=&positions[i];
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
			l=l->next;
		}
		t=new Instruction;
		t->type=InstructionType(EatAdvancedEnergy);
		lnNode* n=new lnNode;
		n->dataptr=&temp_analyzer.pos_adv_energy[0];
		t->argvs=n;
		temp=new lnNode;
		l->dataptr=t;
		l->next=temp;
		if (state->boss->valid_time==time)
		{
			l=l->next;
			t=new Instruction;
			t->type=InstructionType(Approach);
			t->argvs=new lnNode;
			t->argvs->dataptr=new Object(state->boss->boss);
			t->argvs->next=new lnNode;
			t->argvs->next->dataptr=new double(state->status->objects[0].radius);
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
		}
		if (state->enemy->valid_time==time)
		{
			l=l->next;
			t=new Instruction;
			t->type=InstructionType(Approach);
			t->argvs=new lnNode;
			t->argvs->dataptr=new Object(state->enemy->player);
			t->argvs->next=new lnNode;
			t->argvs->next->dataptr=new double(state->status->objects[0].radius);
			temp=new lnNode;
			l->dataptr=t;
		}
		l->next=new lnNode;
		l=l->next;
		t=new Instruction;
		t->type=InstructionType(Flee);
		n=new lnNode;
		t->argvs=n;
		t->argvs->dataptr=new double(5000);
		temp=new lnNode;
		l->dataptr=t;
		(l->next)=temp;
		if (kShortAttackRange[state->status->objects[0].skill_level[SkillType(SHORT_ATTACK)]]>Distance(state->enemy->player.pos,state->enemy->player.pos)||
			kShortAttackRange[state->status->objects[0].skill_level[SkillType(SHORT_ATTACK)]]>Distance(state->enemy->player.pos,state->boss->boss.pos))
		{
			l=l->next;
			t=new Instruction;
			t->type=InstructionType(Skill_ShortAttack);
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
		}
		if (kShortAttackRange[state->status->objects[0].skill_level[SkillType(SHORT_ATTACK)]]>Distance(state->enemy->player.pos,state->enemy->player.pos))
		{
			l=l->next;
			t=new Instruction;
			t->type=InstructionType(Skill_LongAttack);
			lnNode* n=new lnNode;
			t->argvs=n;
			t->argvs->dataptr=new Object(state->enemy->player);
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
		}
		if (kShortAttackRange[state->status->objects[0].skill_level[SkillType(SHORT_ATTACK)]]>Distance(state->enemy->player.pos,state->boss->boss.pos))
		{
			l=l->next;
			t=new Instruction;
			t->type=InstructionType(Skill_LongAttack);
			lnNode* n=new lnNode;
			t->argvs=n;
			t->argvs->dataptr=new Object(state->boss->boss);
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
		}
		if (state->status->objects[0].skill_level[SkillType(DASH)]>0)
		{
			l=l->next;
			t=new Instruction;
			t->type=InstructionType(Skill_Dash);
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
		}
		if (state->status->objects[0].skill_level[SkillType(SHIELD)]>0)
		{
			l=l->next;
			t=new Instruction;
			t->type=InstructionType(Skill_Shield);
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
		}
		l->next=NULL;
		delete temp;
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
			t->argvs=new lnNode;
			t->argvs->dataptr=&positions[i];
			t->argvs->next=NULL;
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
			l=l->next;
		}
		t=new Instruction;
		t->type=InstructionType(EatAdvancedEnergy);
		t->argvs=new lnNode;
		t->argvs->dataptr=new Position(temp_analyzer.pos_adv_energy[0]);
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
	double food_density=temp_analyzer.best_way()[0].weight+temp_analyzer.best_way()[1].weight+temp_analyzer.best_way()[2].weight;
	int extra_ability=0;
	for (int i=SkillType(LONG_ATTACK);i<=SkillType(HEALTH_UP);i++)
	{
		for (int k=0;k<state->status->objects[0].skill_level[i];k++)
			extra_ability+=kBasicSkillPrice[i]*pow(2,k);
	}
	int time=GetTime();
	bool enemy_in_sight=(state->enemy->valid_time==time);
	bool boss_in_sight=(state->boss->valid_time==time);
	//Enemy in sight
	if (enemy_in_sight==1&&boss_in_sight==0)
	{
		//initializing data
		int possible_extra_hp=0,hp_price=pow(2,state->status->objects[0].skill_level[SkillType(HEALTH_UP)]);
		int temp_ability=state->status->objects[0].ability;
		double kill_rate=pow(1.2,3),death_rate=1/kill_rate;
		while (temp_ability>hp_price)
		{
			possible_extra_hp+=500;
			temp_ability-=hp_price;
			hp_price=2*hp_price;
		}
		double original_hp_rate=state->status->objects[0].health/health(state->enemy->player.radius);
		double extra_hp_rate_self=(state->status->objects[0].health+possible_extra_hp)/health(state->enemy->player.radius);
		double distance=Distance(state->status->objects[0].pos,state->enemy->player.pos);


		//setting game over state
		gameover_state=0;
		if ((original_hp_rate<=death_rate&&distance<state->enemy->player.radius)||(state->status->objects[0].health<0.25*state->status->objects[0].max_health))
			gameover_state=2;
		if ((original_hp_rate>=kill_rate&&distance<state->status->objects[0].radius)||(health(state->enemy->player.radius)<0.25*(health(state->enemy->maxr))))
			gameover_state=1;


		//evaluation
		if (original_hp_rate>1) //Add enemy's ability points into consideration!
		{
			//rate5: shield status, rate6: dash status
			return original_hp_rate*state->status->objects[0].health+
				rate1*(state->status->objects[0].ability-3/* 3 should be ability points of enemy, do we have that?*/)+
				rate4*distance+
				rate5*(state->status->objects[0].shield_time>0)+
				rate6*(state->status->objects[0].dash_time>0)*(state->status->objects[0].skill_level[SkillType(DASH)]);
		}
		else
		{
			//rate5: shield status, rate6: dash status
			return original_hp_rate*state->status->objects[0].health+
				rate4*distance+
				rate5*(state->status->objects[0].shield_time>0)+
				rate6*(state->status->objects[0].dash_time>0)*(state->status->objects[0].skill_level[SkillType(DASH)]);
		}
	}
	else if (enemy_in_sight==0&&boss_in_sight==0)									//Regular mode
	{
		return state->status->objects[0].health+
			rate1*extra_ability+
			rate2*food_density+
			rate3*(time); //distance to center?
	}
	else if(enemy_in_sight==0&&boss_in_sight==1)//Only boss in sight
	{
		//initializing data
		int possible_extra_hp=0,hp_price=pow(2,state->status->objects[0].skill_level[SkillType(HEALTH_UP)]);
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
		double distance=Distance(state->status->objects[0].pos,state->boss->boss.pos);

		//setting game over state
		gameover_state=0;
		if ((original_hp_rate<=death_rate&&distance<state->boss->boss.radius)||(state->status->objects[0].health<0.25*state->status->objects[0].max_health))
			gameover_state=2;
		if ((original_hp_rate>=kill_rate&&distance<state->status->objects[0].radius)||(state->boss->boss.radius<1))
			gameover_state=1;

		//evaluating
		return state->status->objects[0].health+
			rate1*extra_ability+
			rate2*food_density+
			rate3*state->map->time+
			rate4*(state->status->objects[0].vision-distance)/state->boss->boss.radius;
	}
	else																					  //Both enemy and boss in sight
	{
		//initializing data
		int possible_extra_hp=0,hp_price=pow(2,state->status->objects[0].skill_level[SkillType(HEALTH_UP)]);
		int temp_ability=state->status->objects[0].ability;
		double kill_rate=pow(1.2,3),death_rate=1/kill_rate;
		while (temp_ability>hp_price)
		{
			possible_extra_hp+=500;
			temp_ability-=hp_price;
			hp_price=2*hp_price;
		}
		double Enemy_original_hp_rate=state->status->objects[0].health/health(state->enemy->player.radius);
		double Enemy_extra_hp_rate_self=(state->status->objects[0].health+possible_extra_hp)/health(state->enemy->player.radius);
		double Enemy_distance=Distance(state->status->objects[0].pos,state->enemy->player.pos);
		double Boss_original_hp_rate=state->status->objects[0].health/health(state->boss->boss.radius);
		double Boss_extra_hp_rate_self=(state->status->objects[0].health+possible_extra_hp)/health(state->boss->boss.radius);
		double Boss_distance=Distance(state->status->objects[0].pos,state->boss->boss.pos);

		//setting game over state
		gameover_state=0;
		if ((Boss_original_hp_rate<=death_rate&&Boss_distance<state->boss->boss.radius)||
			(state->status->objects[0].health<0.25*state->status->objects[0].max_health)||
			(Enemy_original_hp_rate<=death_rate&&Enemy_distance<state->enemy->player.radius))
			gameover_state=2;
		if ((Boss_original_hp_rate>=kill_rate&&Boss_distance<state->status->objects[0].radius)||
			(health(state->enemy->player.radius)<0.25*(health(state->enemy->maxr)))||
			(Enemy_original_hp_rate>=kill_rate&&Enemy_distance<state->status->objects[0].radius)||
			(health(state->boss->boss.radius)<1))
			gameover_state=1;

		//evaluating
		double enemy_evaluation_point;
		if (Enemy_original_hp_rate>1) //Add enemy's ability points into consideration!
		{
			//rate5: shield status, rate6: dash status
			enemy_evaluation_point=	
				Enemy_original_hp_rate*state->status->objects[0].health+
				rate1*(state->status->objects[0].ability-3/* 3 should be ability points of enemy, do we have that?*/)+
				rate4*Enemy_distance+
				rate5*(state->status->objects[0].shield_time>0)+
				rate6*(state->status->objects[0].dash_time>0)*(state->status->objects[0].skill_level[SkillType(DASH)]);
		}
		else
		{
			//rate5: shield status, rate6: dash status
			enemy_evaluation_point=
				Enemy_original_hp_rate*state->status->objects[0].health+
				rate4*Enemy_distance+
				rate5*(state->status->objects[0].shield_time>0)+
				rate6*(state->status->objects[0].dash_time>0)*(state->status->objects[0].skill_level[SkillType(DASH)]);
		}
		return 
			(rate_boss)*
			(state->status->objects[0].health+
			rate1*extra_ability+
			rate2*food_density+
			rate3*state->map->time+
			rate_boss_distance*(state->status->objects[0].vision-Boss_distance)/state->boss->boss.radius)+
			(rate_enemy)*enemy_evaluation_point;
	}
}