#ifndef ANALYZER_H_
#define ANALYZER_H_

#include "basic.h"


//通用的一些
Position cloest(ObjectType A);	//返回最近的type为A的物体的位置
Position* pos(ObjectType A);	//返回视野内所有type为A的物体的位置
int count_type(ObjectType A);	//返回视野内type为A的物体的数量

class analyzer
{
private:
	int num_energy,num_adv_energy,num,num_devour;
	Position* pos_energy;
	Position* pos_adv_energy;
	Position* pos_devour;
	Position* pos_player;

public:
	const Map* map;
	const Status* status;
	analyzer(const Status* status,const Map* map);		//构造函数
	Position* get_pos(ObjectType A);					//返回A类物体的位置（position数列）
	Position cloest(ObjectType A,Position p0);			//返回据p0最近的物体A的位置，默认p0为自身位置
	Position cloest(ObjectType A);			
	Position* inway(ObjectType B,Speed A);				//判断speed方向上是否会碰到B，是则返回B的位置，不是则返回null）
	~analyzer();
};

//关于player的函数与结构
void observe(Object A);	//用于观察敌人信息

struct Enemy		//己方对敌方的信息储存
{
	double maxr;
	double minr;
	int skills[5];	//分别代表敌方五个技能的等级
	Speed speed;
	int addheal;	//判断敌方是否已经加血次数
	Object player;
};


//关于光之隧道的函数
struct adv_memory
{
	Position pos;
	int oldtime;		//上一次该光之隧道被吃掉的时间		
};

#endif











