#include "../headers/common.h"
#include "../headers/teamstyle17.h"
#include "../headers/analyzer.h"
#include<stdio.h>


void observe(Object A,Enemy* enemy)
{
	if (enemy!=NULL)
	{
		double s=Norm(enemy->speed);
		enemy->speed=Displacement(A.pos,enemy->player.pos);
		if (A.radius>enemy->maxr) enemy->maxr=A.radius;
		if (A.radius<enemy->minr) enemy->minr=A.radius;
		if (health(A.radius)-health(enemy->player.radius)>400) enemy->addheal++;
		enemy->player=A;
		if (A.shield_time>0) enemy->skills[2]=1;
		if (A.long_attack_casting>0) enemy->skills[1]=1;
		if (s>102) enemy->skills[3]=int((s-100)/20);
	}
	else
	{
		enemy=new Enemy;
		enemy->addheal=0;
		enemy->maxr=A.radius;
		enemy->minr=A.radius;
		enemy->player.pos=A.pos;
		enemy->skills[0]=0;
		enemy->skills[1]=0;
		enemy->skills[2]=0;
		enemy->skills[3]=0;
		enemy->skills[4]=0;
	}
}

analyzer::analyzer(const Status* status=GetStatus(),const Map* map=GetMap())
{
	int i;
	int m=0,n=0,q=0;
	num_energy=0;
	num_adv_energy=0;
	num_devour=0;
	for(i=0;i<map->objects_number;i++)
	{
		switch(map->objects[i].type)
		{
		case ENERGY:num_energy++;break;
		case ADVANCED_ENERGY:num_adv_energy++;break;
		case DEVOUR:num_devour++;break;
		}
	}
	pos_adv_energy=new Position[num_adv_energy];
	pos_devour=new Position[num_devour];
	pos_energy=new Position[num_energy];
	for(i=0;i<map->objects_number;i++)
	{
		switch(map->objects[i].type)
		{
		case ENERGY:
			{
				pos_energy[m]=map->objects[i].pos;
				m++;
				break;
			}
		case ADVANCED_ENERGY:
			{
				pos_adv_energy[n]=map->objects[i].pos;
				n++;
				break;
			}
		case DEVOUR:
			{
				pos_adv_energy[q]=map->objects[i].pos;
				q++;
				break;
			}
		case PLAYER:
			{
				*pos_player=map->objects[i].pos;
				observe(map->objects[i]);
				break;
			}
		}
	}
}

Position* analyzer::get_pos(ObjectType A)
{
	switch(A)
	{
	case ENERGY:return(pos_energy);
	case ADVANCED_ENERGY:return(pos_adv_energy);
	case DEVOUR:return(pos_devour);
	case PLAYER:return(pos_player);
	default:return(NULL);
	}
}

Position analyzer::cloest(ObjectType A,Position p0)
{
	int cloest=0,i;
	Position* posA=get_pos(A);
	for(i=1;i<count_type(A);i++)
	{
		if (Distance(p0,posA[i])<Distance(p0,posA[cloest]))
			cloest=i;
	}
	return(posA[cloest]);
}

Position analyzer::cloest(ObjectType A)
{
	int cloest=0,i;
	Position* posA=get_pos(A);
	Position p0=status->objects[0].pos;
	for(i=1;i<count_type(A);i++)
	{
		if (Distance(p0,posA[i])<Distance(p0,posA[cloest]))
			cloest=i;
	}
	return(posA[cloest]);
}

Position* analyzer::inway(ObjectType B,Speed A)
{
	double distance;	//Bµ½speedÏß¶ÎµÄ¾àÀë
	int i;
	double r=status->objects[0].radius;
	Position* posd=get_pos(B);
	Position linepoint=Displacement(status->objects[0].pos,A);
	for(i=0;i<count_type(B);i++)
	{
		distance=PointLineDistance(posd[i],status->objects[0].pos,linepoint);
		if (distance<r) return(&posd[i]);
	}
	return(NULL);
}

analyzer::~analyzer()
{
	delete[] pos_adv_energy;
	delete[] pos_devour;
	delete[] pos_energy;
	delete pos_player;
}












