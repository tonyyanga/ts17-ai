#include "../headers/common.h"
#include "../headers/teamstyle17.h"
#include <cmath>
#include <iostream>

int max(int x,int y,int z)
{
	int m;
	if (x>y)
	{
		if (x>z) m=x;
		else m=z;
	}
	else
	{
		if (y>z) m=y;
		else m=z;
	}
	return(m);
}

int Usetime(bool add,Position &p,Position &p0,PlayerObject &Self)	//�������ٶȷ���ı�Ļغ�ʱ,���ش�p��p0�Ļغ���,addΪ�������ʹ�ü��ټ���
{
	int time,da_time;
	int speed=100;
	double d;
	d=Distance(p0,p);
	da_time=Self.dash_time;
	if (add==false)
	{
		time=int(d/speed);
		time++;
		return(time);
	}
	else
	{
		speed+=20*Self.skill_level[DASH];
		time=int(d/speed);
		if (da_time==0) da_time=40;
		if (time<=da_time) return time;	//����ʱ���ھ͵�����Ŀ��λ��
		else
		{
			d=d-speed*(da_time);
			time=da_time+int(d/100)+1;
			return(time);
		}
	}
}

double angle(Position p1,Position p2)
{
	return(acos(DotProduct(p1,p2)/(Norm(p1)*Norm(p2))));
}

double health(double radius)
{
	return((pow(radius/10,3)));
}

double get_radius(double Health)
{
	double r;
	r=100*pow(Health,1.0/3);
	return(r);
}

struct Map filter(struct Map input, PlayerObject* player) {
	struct Map result;
	long length=0;
	long count;
	result.time=input.time;
	Object* result_obj=(Object*)malloc(input.objects_number*sizeof(Object));
	Object* temp=result_obj;
	lnNode* rawfood=new lnNode;
	rawfood->next=NULL;
	lnNode* temp2=rawfood;
	for (count=0;count<=input.objects_number-1;count++) {
		if (input.objects[count].id!=ENERGY) {
			*temp=input.objects[count];
			temp++;
			length++;
		} else {
			temp2->next=new lnNode;
			temp2=temp2->next;
			temp2->dataptr=&input.objects[count];
			temp2->next=NULL;
		}
	}
	rawfood=rawfood->next;
	lnNode* parsedlists=best_way(rawfood, &player->pos, player->radius);
	temp2=parsedlists;
	while(temp2) {
		*temp=*(Object*)temp2->dataptr;
		temp2=temp2->next;
		length++;
		temp++;
	}
	result.objects_number=length;
	memcpy(result.objects, result_obj, length*sizeof(Object));
	return result;
}