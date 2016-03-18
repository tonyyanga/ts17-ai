#include "../headers/common.h"
#include "../headers/teamstyle17.h"

int usetime(bool add,Position &p,Position &p0,PlayerObject &Self)	//不计算速度方向改变的回合时,返回从p到p0的回合数,add为假则代表不使用加速技能
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
		if (time<=da_time) return time;	//加速时间内就到达了目标位置
		else
		{
			d=d-speed*(da_time);
			time=da_time+int(d/100)+1;
			return(time);
		}
	}
}