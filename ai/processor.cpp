#include <iostream>
#include <cmath>
#include "../headers/instructions.h"
#include "../headers/processor.h"
#include "../headers/teamstyle17.h"
#include"../headers/basic.h"
#include"../headers/analyzer.h"
#include"../headers/common.h"

#define PI 3.1415926
LIFO::LIFO(int size=5)
{
	s=new Instruction[size];
	bottom=0;
}

Instruction LIFO::pop()		//Pop first Instruction out
{
	Instruction i=s[0];
	s[0]=s[1],s[1]=s[2],s[2]=s[3],s[3]=s[4];
	bottom--;
	return i;
}

void LIFO::push(Instruction t,int priority)
{
	s[4]=s[3],s[3]=s[2],s[2]=s[1],s[1]=s[0],s[0]=t;
	bottom++;
}

Instruction LIFO::get_top()
{
	return s[0];
}

int LIFO::return_bottom()
{
	return bottom;
}

void LIFO::clear()
{
	bottom=0;
}

FIFO::FIFO(int size=5)
{
	top=0;
	s=new Instruction[size];
}

Instruction FIFO::pop()	
{
	return s[top--];
}

int FIFO::return_top()
{
	return top;
}

void FIFO::push(Instruction t,int priority)
{
	s[4]=s[3],s[3]=s[2],s[2]=s[1],s[1]=s[0],s[0]=t;
	top++;
}

Instruction FIFO::get_top()
{
	return s[top];
}

void FIFO::clear()
{
	top=0;
}

processor::processor(LIFO l1,LIFO l2,FIFO f1,FIFO f2,SceneState* s)
{
	this->l1=l1,this->l2=l2,this->f1=f1,this->f2=f2;
	temp_in.priority=0;
	this->CurrentState=s;
}

void processor::choose_instruction()
{
	if ((l1.return_bottom()!=0) && (temp_in.priority<4))
		temp_in.i=l1.pop(),temp_in.priority=4;
	else if ((f1.return_top()!=0) && (temp_in.priority<3))
		temp_in.i=f1.pop(),temp_in.priority=3;
	else if ((l2.return_bottom()!=0) && (temp_in.priority<2))
		temp_in.i=l2.pop(),temp_in.priority=2;
	else if ((f2.return_top()!=0) && (temp_in.priority<1))
		temp_in.i=f2.pop(),temp_in.priority=1;
}

int processor::judge_condition()
{
	lnNode* i=temp_in.i.conditons;
	while (i != NULL)
	{
		//Todo
	}
	return 0;
}

APIOrder processor::return_Order()
{
	if (judge_condition()==0)
	{
		temp_in.priority=0;
		choose_instruction();	
	}
	//Set now_order to new order (added a new order type Finished
	if (now_order.type==Finished)
	{
		temp_in.priority=0;
		choose_instruction();	
	}	
}

void implement(APIOrder x)
{
	switch (x.type)
	{
	case API_StayStill:; break;
	case API_Switchspeed:Move(*(x.p1), *((Speed*)x.p2)); break;
	case API_UpgradeSkill:UpgradeSkill(*(x.p1), *(SkillType*)x.p2); break;
	case API_ShortAttack:ShortAttack(*(x.p1)); break;
	case API_LongAttack:LongAttack(*(x.p1), *((int*)x.p2)); break;
	case API_Dash:Dash(*(x.p1)); break;
	case API_Shield:Shield(*(x.p1)); break;
	case API_HealthUp:HealthUp(*(x.p1)); break;
	}
}

bool APIOrder::if_elseFinish()
{
	now_instruction* Current_instruction = new now_instruction;
	analyzer* Current_analyzer = (analyzer*)malloc(sizeof(analyzer));
	if ((*Current_instruction).i.type == Skill_Shield && (*(*Current_analyzer).status).objects[0].shield_time != 0)
		return 1;
	else if ((*Current_instruction).i.type == Skill_Dash && (*(*Current_analyzer).status).objects[0].dash_time != 0)
		return 1;
	else return 0;
	delete Current_instruction;
	free(Current_analyzer);
}

bool APIOrder::if_elseNeed()
{
	Enemy* Current_enemy = new Enemy;
	now_instruction* Current_instruction = new now_instruction;
	analyzer* Current_analyzer = (analyzer*)malloc(sizeof(analyzer));
	if ((*Current_instruction).i.type == Skill_HealthUp)
	{
		Position* Current_enemy_position = new Position;
		Position* Current_self_position = new Position;
		(*Current_enemy_position).x = (*Current_enemy).player.pos.x + (*Current_enemy).speed.x*(*(*Current_analyzer).status).objects[0].long_attack_casting;
		(*Current_enemy_position).y = (*Current_enemy).player.pos.y + (*Current_enemy).speed.y*(*(*Current_analyzer).status).objects[0].long_attack_casting;
		(*Current_enemy_position).x = (*Current_enemy).player.pos.z + (*Current_enemy).speed.z*(*(*Current_analyzer).status).objects[0].long_attack_casting;
		(*Current_self_position).x = (*(*Current_analyzer).status).objects[0].pos.x + (*(*Current_analyzer).status).objects[0].speed.x*(*(*Current_analyzer).status).objects[0].long_attack_casting;
		(*Current_self_position).y = (*(*Current_analyzer).status).objects[0].pos.y + (*(*Current_analyzer).status).objects[0].speed.y*(*(*Current_analyzer).status).objects[0].long_attack_casting;
		(*Current_self_position).z = (*(*Current_analyzer).status).objects[0].pos.z + (*(*Current_analyzer).status).objects[0].speed.z*(*(*Current_analyzer).status).objects[0].long_attack_casting;
		if (Distance(*Current_enemy_position, *Current_self_position) <= 3000 + (*(*Current_analyzer).status).objects[0].skill_level[LONG_ATTACK] * 500)
			return 1;
	}
}

void APIOrder::Do_it()
{
	now_instruction* Current_instruction = new now_instruction;
	analyzer* Current_analyzer = (analyzer*)malloc(sizeof(analyzer));
	if ((*Current_instruction).i.type == Skill_ShortAttack)
	{
		type = API_ShortAttack;
		implement(*this);
	}
	if ((*Current_instruction).i.type == Skill_HealthUp)
	{
		type = API_HealthUp;
		implement(*this);
	}
	if ((*Current_instruction).i.type == Skill_Dash&&if_elseFinish() == 0)
	{
		type = API_Dash;
		implement(*this);
	}
	if ((*Current_instruction).i.type == Skill_Shield&&if_elseFinish() == 0)
	{
		type = API_Shield;
		implement(*this);
	}
	if ((*Current_instruction).i.type == Skill_LongAttack&&if_elseNeed() == 1)
	{
		type = API_LongAttack;
		implement(*this);
	}
}

APIOrder::APIOrder()
{
	analyzer* Current_analyzer = (analyzer*)malloc(sizeof(analyzer));
	(*p1) = (*(*Current_analyzer).status).objects[0].id;
	free(Current_analyzer);
}