#include "../headers/common.h"
#include "../headers/teamstyle17.h"

int usetime(bool add,Position &p,Position &p0,PlayerObject &Self)	//�������ٶȷ���ı�Ļغ�ʱ,���ش�p��p0�Ļغ���,addΪ�������ʹ�ü��ټ���
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