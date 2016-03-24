#ifndef ANALYZER
#define ANALYZER

#include "basic.h"
#include"processor.h"

class analyzer
{
private:

public:
	const Map* map;
	const Status* status;								//当前状态
	int num_energy,num_adv_energy,num,num_devour;
	Position* pos_energy;
	Position* pos_adv_energy;
	Position* pos_devour;
	Position* pos_player;
	analyzer(Enemy* enemy,const Status* status,const Map* map);		//构造函数
	Position closest(ObjectType A,Position p0);			//返回据p0最近的物体A的位置，默认p0为自身位置
	Position closest(ObjectType A);			
	Position* inway(ObjectType B,Speed A);				//判断speed方向上是否会碰到B，是则返回B的位置，不是则返回null）
	~analyzer();
	Position best_way();								//通过自身为中心的半径一定范围内的食物集群情况，返回能吃到最多energy的矢量方向
														//注意！这里返回的只是速度的一个矢量方向，无实际大小
};

//关于player的函数与结构

struct Enemy		//己方对敌方的信息储存
{
	double maxr;
	double minr;
	int skills[5];	//分别代表敌方五个技能的等级
	Speed speed;
	bool addheal;	//判断敌方是否已经加过1000的血
	bool to_me;		//判断敌方是否向我方运动
	int valid_time;	//说明数据来源的回合
	Object player;
};

void observe(Object A,Enemy* enemy);	//用于观察敌人信息

//关于光之隧道的函数

#endif











