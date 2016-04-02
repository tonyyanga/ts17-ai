#include <stddef.h>
#include <new>
#include <iostream>
#include "../headers/common.h"
#include "../headers/teamstyle17.h"
#include "../headers/analyzer.h"
#include "../headers/basic.h"

using namespace std;

void observe(Object A,Enemy* enemy)
{
	if (enemy!=NULL)
	{
		double s=100;
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
		enemy->Health=health(A.radius);
	}
}

void observe_boss(Object A,Boss* Boss)
{
	Boss->boss.pos.x=A.pos.x;
	Boss->boss.pos.y=A.pos.y;
	Boss->boss.pos.z=A.pos.z;
	Boss->boss.radius=A.radius;
	Boss->boss.type=A.type;
	Boss->valid_time=GetTime();
}

void store_adv(Object A,Store_adv *first)
{
	Store_adv *node;
	node=first;
	bool flag=true;
	while (flag==true)
	{
		if (node==NULL) break;
		if (node->pos.x==A.pos.x && node->pos.y==A.pos.y && node->pos.z==A.pos.z)
			flag=false;
		node=node->next;
	}
	if (flag)
		node=new Store_adv;
		node->next=NULL;
		node->pos=A.pos;
		node->valid_time=GetTime();
		node->exist=1;
}



analyzer::analyzer(Enemy* enemy,Boss* boss,Store_adv* firstnode,const Status* status=GetStatus(),const Map* map=GetMap())

{
	int i;
	int m=0,n=0,q=0;
	this->status=status;
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
	/*cout<<"num_enengy"<<num_energy;
	cout<<"num_adv_enengy"<<num_adv_energy;
	cout<<"num_devour"<<num_devour;*/
	if (num_adv_energy!=0)
		pos_adv_energy=new Position[num_adv_energy];
	else
		pos_adv_energy=NULL;
	pos_devour=new Position[num_devour];
	pos_energy=new Position[num_energy];
	pos_player=new Position;
	pos_boss=new Position;
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
				if (num_adv_energy==0)
					break;								//*****************************//
				pos_adv_energy[n]=map->objects[i].pos;
				n++;
				store_adv(map->objects[i],firstnode);
				break;
			}
		case DEVOUR:
			{
				pos_devour[q]=map->objects[i].pos;
				q++;
				break;
			}
		case PLAYER:
			{
				*pos_player=map->objects[i].pos;
				observe(map->objects[i],enemy);
				break;
			}
		case BOSS:
			{
				*pos_boss=map->objects[i].pos;
				observe_boss(map->objects[i],boss);
				break;
			}
		}
	}
}

analyzer::analyzer(SceneState* state) {
	new (this) analyzer(state->enemy, state->boss,state->adv,state->status, state->map);
}

Position* analyzer::closest(Position p0)
{
	int closest=0,i,n;
	Position* posA;
	posA=pos_energy;
	n=num_energy;
	for(i=1;i<n;i++)
	{
		if (Distance(p0,posA[i])<Distance(p0,posA[closest]))
			closest=i;
	}
	cout<<"cloest A:x="<<posA[closest].x;
	return(&posA[closest]);
}

Position* analyzer::closest(ObjectType A=ENERGY)
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
	cout<<"cloest A:x="<<posA[closest].x;
	return(&posA[closest]);
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
	case BOSS:
		{
			posA=pos_boss;
			n=1;
		}
	}
	for(i=0;i<n;i++)
	{
		distance=PointLineDistance(posA[i],status->objects[0].pos,linepoint);
		if (distance<r) return(&posA[i]);
	}
	return(NULL);
}


/*density* divide(lnNode* first,Position *self,double r)
{
	lnNode* firstnode;
	lnNode* currentnode=first;
	Object* obj;
	Position pos_en;
	int dis;
	int x,y,z,i=0;
	density m[4][4][4];
	density t;
	for(x=0;x<4;x+=1)
	{
		for(y=0;y<4;y+=1)
		{
			for(z=0;z<4;z+=1)
			{
				m[x][y][z].pos.x=self->x+(x-2.5)*r;
				m[x][y][z].pos.y=self->y+(y-2.5)*r;
				m[x][y][z].pos.z=self->z+(z-2.5)*r;
				m[x][y][z].number=0;
				i++;
			}
		}
	}
	for(i=0;i<26;i++)
	{
		for(x=0;x<num_energy;x++)
		{
			d=PointLineDistance(pos_energy[x],selfpos,Displacement(selfpos,m[i].speed));
			if (d<r) 
			{
					dis=int(Norm(Displacement(pos_energy[x],selfpos)));				//为节省运算资源，将其作为int计算
					dis/=100;
					dis=dis*dis;
					m[i].weight+=1.0/dis;
					m[i].number++;
			}
		}
	}
	x=0;
	for(i=0;i<26;i++)
	{
		for(x=i+1;x<26;x++)
		{
			if (m[i].weight>m[x].weight) 
			{
				t[0]=m[i];
				m[i]=m[x];
				m[x]=t[0];
			}
		}
	}
	for(i=0;i<3;i++)
	{
		t[i]=m[i];
	}
	return(t);
}
=======
	while (currentnode!=NULL)
	{
		pos_en=*(Position*)currentnode->dataptr;
		x=(int)(pos_en.x+2.5*r-self->x)/r;
		y=(int)(pos_en.y+2.5*r-self->y)/r;
		z=(int)(pos_en.z+2.5*r-self->z)/r;
		m[x][y][z].number++;
		currentnode=currentnode->next;
	}
	return(&m[0][0][0]);
}

density* order(density m[4][4][4],Position *pos)
{
	int x,y,z;
	int a=0,b=0,c=0;
	for(x=0;x<4;x+=1)
	{
		for(y=0;y<4;y+=1)
		{
			for(z=0;z<4;z+=1)
			{
				if (m[x][y][z].number>m[a][b][c].number)
				{
					a=x;
					b=y;
					c=z;
				}
			}
		}
	}
	return(&m[a][b][c]);
}
*/


analyzer::~analyzer()
{
	/*delete[] pos_adv_energy;
	delete[] pos_devour;
	delete[] pos_energy;
	delete pos_player;*/
}












