#include <iostream>
#include <cmath>
#include "../headers/instructions.h"
#include "../headers/processor.h"
#include "../headers/teamstyle17.h"
#include"../headers/basic.h"
#include"../headers/analyzer.h"
#include"../headers/common.h"

#define PI 3.1415926
using namespace std;
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

void LIFO::push(Instruction t)
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

void FIFO::push(Instruction t)
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

processor::processor(SceneState* s)
{
	this->l1=LIFO (5),this->l2=LIFO (5),this->f1=FIFO (5),this->f2=FIFO (5);
	scene=s;
	state=new analyzer(scene);
	temp_in.priority=0;
	cout<<"Processor initialized"<<endl;
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

int processor::judge_condition()  //doesn't work yet
{
	lnNode* i=temp_in.i.conditons;
	return 1;	//0 if object doesn't meet the conditions
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

void APIOrder::if_elseFinish()
{
	Order* Current_order = new Order;
	if ((*Current_order).next == NULL)
		free(Current_order);
	else next_order();
}

bool APIOrder::if_elseNeed()
{
	Enemy* Current_enemy = new Enemy;
	now_instruction* Current_instruction = new now_instruction;
	analyzer* Current_analyzer = (analyzer*)malloc(sizeof(analyzer));
	if ((*Current_instruction).i.type == Skill_Shield && (*(*Current_analyzer).status).objects[0].shield_time != 0)
		return 1;
	else if ((*Current_instruction).i.type == Skill_Dash && (*(*Current_analyzer).status).objects[0].dash_time != 0)
		return 1;
	else if ((*Current_instruction).i.type == Skill_LongAttack)
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
			return 0;
		else
			return 1;
		free(Current_enemy_position);
		free(Current_self_position);
	}
	else return 0;
	free(Current_analyzer);
	free(Current_enemy);
	free(Current_instruction);
}

void APIOrder::next_order()
{
	Order* Next_order = new Order;
	delete order;
	order = (*Next_order).next;
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
	if ((*Current_instruction).i.type == Skill_Dash&&if_elseNeed()==0)
	{
		type = API_Dash;
		implement(*this);
	}
	if ((*Current_instruction).i.type == Skill_Shield&&if_elseNeed()==0)
	{
		type = API_Shield;
		implement(*this);
	}
	if ((*Current_instruction).i.type == Skill_LongAttack&&if_elseNeed()==0)
	{
		type = API_LongAttack;
		implement(*this);
	}
	if ((*Current_instruction).i.type == MovePosition|| (*Current_instruction).i.type==Approach|| (*Current_instruction).i.type == EatAdvancedEnergy|| (*Current_instruction).i.type == Flee)
	{
		int i, flag = 0;	//flag=0 means no object is inway;
		for (i = PLAYER; i < kObjectTypes;i++)
			if ((*Current_analyzer).inway(ObjectType(i), *(Position*)(*(*Current_instruction).i.argvs).dataptr) != NULL)
			{
				flag = 1;
				break;
			}
		if(flag==0)
		{
			type = API_Switchspeed;
			(*(Position*)p2) = Displacement((*(*Current_analyzer).status).objects[0].pos, *(Position*)(*(*Current_instruction).i.argvs).dataptr);
			implement(*this);
		}
		else
		{
			if((*Current_analyzer).inway(DEVOUR, *(Position*)(*(*Current_instruction).i.argvs).dataptr)!=NULL)
			type = API_Switchspeed;
			(*(Position*)p2) = CrossProduct((*(*Current_analyzer).status).objects[0].pos, *(Position*)(*(*Current_instruction).i.argvs).dataptr);
			implement(*this);
		}
	}
}

APIOrder::APIOrder()
{
	//Objected included in processor
}


void processor::temp_set_ins()
{
	switch(temp_in.i.type)
	{
	case InstructionType(MovePosition):
		{
			Position *p=(Position*)temp_in.i.argvs->dataptr;
			if (scene->status->objects[0].pos.x==p->x&&
				scene->status->objects[0].pos.y==p->y&&
				scene->status->objects[0].pos.z==p->z)
				temp_in.priority=0;
		}
	case InstructionType(EatAdvancedEnergy):
		{
			Position *p=(Position*)temp_in.i.argvs->dataptr;
			if (scene->status->objects[0].pos.x==p->x&&
				scene->status->objects[0].pos.y==p->y&&
				scene->status->objects[0].pos.z==p->z)
				temp_in.priority=0;
		}
	case InstructionType(Approach):
		{
			Object* p=(Object*)temp_in.i.argvs->dataptr;
			if (Distance(scene->status->objects[0].pos,p->pos)<=int(temp_in.i.argvs->next->dataptr))
				temp_in.priority=0;
		}
	case InstructionType(Flee):
		{
			if (Distance(scene->status->objects[0].pos,scene->enemy->player.pos)<=int(temp_in.i.argvs->dataptr))
				temp_in.priority=0;
		}
	case InstructionType(MultiInstructions):
		{
			multiple_temp=temp_in.i.argvs;
			temp_in.i=*(Instruction*)multiple_temp->dataptr;
			multiple=temp_in.priority;
		}
	default:
		{
			if (temp_in.valid_time!=GetTime())
			temp_in.priority=0;
		}
	}
	cout<<"Instruction set to "<<temp_in.i.type<<endl;
	if (multiple_temp!=NULL&&temp_in.priority==0)
	{
		temp_in.priority=multiple;
		temp_in.i=*(Instruction*)multiple_temp->dataptr;
		multiple_temp=multiple_temp->next;
	}
	if ((l1.return_bottom()!=0) && (temp_in.priority<4))
		temp_in.i=l1.pop(),temp_in.priority=4,temp_in.valid_time=GetTime(),multiple_temp=NULL,cout<<"Instruction updated"<<endl;
	else if ((f1.return_top()!=0) && (temp_in.priority<3))
		temp_in.i=f1.pop(),temp_in.priority=3,temp_in.valid_time=GetTime(),cout<<"Instruction updated"<<endl;
	else if ((l2.return_bottom()!=0) && (temp_in.priority<2))
		temp_in.i=l2.pop(),temp_in.priority=2,temp_in.valid_time=GetTime(),cout<<"Instruction updated"<<endl;
	else if ((f2.return_top()!=0) && (temp_in.priority<1))
		temp_in.i=f2.pop(),temp_in.priority=1,temp_in.valid_time=GetTime(),cout<<"Instruction updated"<<endl;
}
void processor::temp_implement()
{
	int user_id=scene->status->objects[0].id;
	cout<<"Implementing "<<temp_in.i.type<<endl;
	switch(this->temp_in.i.type)
	{
	case InstructionType(MovePosition):
		{
			Position temp;
			Position* p=(Position*)temp_in.i.argvs->dataptr;
			temp.x=p->x-scene->status->objects[0].pos.x;
			temp.y=p->y-scene->status->objects[0].pos.y;
			temp.z=p->z-scene->status->objects[0].pos.z;
			Move(user_id,temp);
		}
	case InstructionType(EatAdvancedEnergy):
		{
			Position temp;
			Position* p=(Position*)temp_in.i.argvs->dataptr;
			temp.x=p->x-scene->status->objects[0].pos.x;
			temp.y=p->y-scene->status->objects[0].pos.y;
			temp.z=p->z-scene->status->objects[0].pos.z;
			Move(user_id,temp);
		}
	case InstructionType(Approach):
		{
			Object* p=(Object*)temp_in.i.argvs->dataptr;
			Position temp;
			Position t=p->pos;
			temp.x=t.x-scene->status->objects[0].pos.x;
			temp.y=t.y-scene->status->objects[0].pos.y;
			temp.z=t.z-scene->status->objects[0].pos.z;
			Move(user_id,temp);
		}
	case InstructionType(Flee):
		{
			Object* p=(Object*)temp_in.i.argvs->dataptr;
			Position temp;
			Position t=scene->enemy->player.pos;
			temp.x=-t.x+scene->status->objects[0].pos.x;
			temp.y=-t.y+scene->status->objects[0].pos.y;
			temp.z=-t.z+scene->status->objects[0].pos.z;
			Move(user_id,temp);
		}
	case InstructionType(Skill_ShortAttack):
		{
			ShortAttack(user_id);
		}
	case InstructionType(Skill_LongAttack):
		{
			LongAttack(user_id,(int)temp_in.i.argvs);
		}
	case InstructionType(Skill_Dash):
		{
			Dash(user_id);
		}
	case InstructionType(Skill_Shield):
		{
			Shield(user_id);
		}
	case InstructionType(Skill_HealthUp):
		{
			HealthUp(user_id);
		}
	case InstructionType(Skill_UpgradeSkill):
		{
			UpgradeSkill(user_id,SkillType((int)temp_in.i.argvs->dataptr));
		}
	}
}


void processor::AddInstruction(Instruction* i, int p)
{
	switch(p)
	{
	case 1:f2.push(*i);
	case 2:l2.push(*i);
	case 3:f1.push(*i);
	case 4:l1.push(*i);
	}
	cout<<"Instruction added"<<endl;
}

bool processor::update(SceneState* s)
{
	if (scene!=s)
	{
		scene=s;
		cout<<"Processor scene updated"<<endl;
		return 1;
	}
	else
	{
		scene=s;
		cout<<"Processor scene updated"<<endl;
		return 0;
	}
}