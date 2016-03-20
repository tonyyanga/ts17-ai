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


APIOrder::APIOrder()
{
	(*p1) = Self.id;
}

void APIOrder::switchspeed()
{
	Position min = closest(PLAYER);
	int i;
	for (i = ENERGY; i < kObjectTypes; i++)
		if (Distance(Self.pos, closest((ObjectType)i)) < Distance(Self.pos, min))
			min = closest((ObjectType)i);
	if (dev_inway == NULL) type = API_StayStill;
	else
	{
		type = API_Switchspeed;
		Position temp;
		double length = Norm(CrossProduct(Self.speed, Displacement(Self.pos, *dev_inway(Self.speed))));
		temp.x = CrossProduct(Self.speed, Displacement(Self.pos, *dev_inway(Self.speed))).x / length;
		temp.y = CrossProduct(Self.speed, Displacement(Self.pos, *dev_inway(Self.speed))).y / length;
		temp.z = CrossProduct(Self.speed, Displacement(Self.pos, *dev_inway(Self.speed))).z / length;
		*((Position*)p2) = temp;
	}
}

void APIOrder::Attack()
{
	Enemy closest_enemy;
	if (Self.skill_cd[SHORT_ATTACK] == 0 && Distance(Self.pos, closest(PLAYER)) <= 90 + 10 * Self.skill_level[SHORT_ATTACK])
		type = API_ShortAttack;
	else type = API_StayStill;
	if (Self.skill_level[LONG_ATTACK] > 0)
	{
		Position Final_position_enemy, Final_position_self;
		Final_position_enemy.x = closest(PLAYER).x + closest_enemy.speed.x*Self.long_attack_casting;
		Final_position_enemy.y = closest(PLAYER).y + closest_enemy.speed.y*Self.long_attack_casting;
		Final_position_enemy.z = closest(PLAYER).z + closest_enemy.speed.z*Self.long_attack_casting;
		Final_position_self.x = Self.pos.x + Self.speed.x*Self.long_attack_casting;
		Final_position_self.y = Self.pos.y + Self.speed.y*Self.long_attack_casting;
		Final_position_self.z = Self.pos.z + Self.speed.z*Self.long_attack_casting;
		if (Distance(Final_position_enemy, Final_position_self) <= 3000 + 500 * Self.skill_level[LONG_ATTACK])
			type = API_LongAttack;
	}
	else type = API_StayStill;
}
