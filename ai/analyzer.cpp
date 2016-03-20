#include "../headers/common.h"
#include "../headers/teamstyle17.h"
#include "../headers/analyzer.h"
#include<stddef.h>


void observe(Object A,Enemy* enemy)
{
	if (enemy!=NULL)
	{
		double s;
		int t=GetTime();		//当前回合数
		if (t-enemy->valid_time==1)				//连续两回合都在视野内
		{
			enemy->speed=Displacement(A.pos,enemy->player.pos);
			s=Norm(enemy->speed);
		}
		if (A.radius>enemy->maxr) enemy->maxr=A.radius;
		if (A.radius<enemy->minr) enemy->minr=A.radius;
		if (health(A.radius)-health(enemy->player.radius)>400) enemy->addheal++;
		enemy->player=A;
		if (A.shield_time>0) enemy->skills[2]=1;
		if (A.long_attack_casting>0) enemy->skills[1]=1;
		if (s>102) enemy->skills[3]=int((s-100)/20);
		enemy->valid_time=t;
	}
}

analyzer::analyzer(Enemy* enemy,const Status* status=GetStatus(),const Map* map=GetMap())
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
				observe(map->objects[i],enemy);
				break;
			}
		}
	}
}

Position analyzer::closest(ObjectType A,Position p0)
{
	int closest=0,i,n;
	Position* posA;
	switch (A)
	{
	case ENERGY:
		{
			posA=pos_energy;
			n=num_energy;
			break;
		}
	case ADVANCED_ENERGY:
		{
			posA=pos_adv_energy;
			n=num_adv_energy;
			break;
		}
	case DEVOUR:
		{
			posA=pos_devour;
			n=num_devour;
			break;
		}
	}
	for(i=1;i<n;i++)
	{
		if (Distance(p0,posA[i])<Distance(p0,posA[closest]))
			closest=i;
	}
	return(posA[closest]);
}

Position analyzer::closest(ObjectType A)
{
	int closest=0,i,n;
	Position* posA;
	Position p0=status->objects[0].pos;
	switch (A)
	{
	case ENERGY:
		{
			posA=pos_energy;
			n=num_energy;
			break;
		}
	case ADVANCED_ENERGY:
		{
			posA=pos_adv_energy;
			n=num_adv_energy;
			break;
		}
	case DEVOUR:
		{
			posA=pos_devour;
			n=num_devour;
			break;
		}
	}
	for(i=1;i<n;i++)
	{
		if (Distance(p0,posA[i])<Distance(p0,posA[closest]))
			closest=i;
	}
	return(posA[closest]);
}

Position* analyzer::inway(ObjectType B,Speed A)
{
	double distance;	//B到speed线段的距离
	int i,n;
	double r=status->objects[0].radius;
	Position linepoint=Displacement(status->objects[0].pos,A);
	Position* posA;
	switch (B)
	{
	case ENERGY:
		{
			posA=pos_energy;
			n=num_energy;
			break;
		}
	case ADVANCED_ENERGY:
		{
			posA=pos_adv_energy;
			n=num_adv_energy;
			break;
		}
	case DEVOUR:
		{
			posA=pos_devour;
			n=num_devour;
			break;
		}
	}
	for(i=0;i<n;i++)
	{
		distance=PointLineDistance(posA[i],status->objects[0].pos,linepoint);
		if (distance<r) return(&posA[i]);
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












