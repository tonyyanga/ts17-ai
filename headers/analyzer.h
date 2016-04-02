#ifndef ANALYZER
#define ANALYZER

#include "basic.h"
#include "processor.h"
#include "common.h"

struct density			//用于表示食物的密度分布
	{
		Position pos;
		int number;
	};

struct Boss
{
	Object boss;
	int valid_time;
};

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
	Position* pos_boss;
	analyzer(struct Enemy* enemy,struct Boss* boss,struct Store_adv* firstnode,const Status* status,const Map* map);		//构造函数
	analyzer(struct SceneState* state);
	Position* closest(ObjectType A,Position p0);			//返回据p0最近的物体A的位置，默认p0为自身位置
	Position* closest(ObjectType A);			
	Position* inway(ObjectType B,Speed A);				//判断speed方向上是否会碰到B，是则返回B的位置，不是则返回null）
	~analyzer();							
};

//关于player的函数与结构

typedef struct Enemy		//己方对敌方的信息储存
{
	double maxr;
	double minr;
	int skills[5];	//分别代表敌方五个技能的等级
	Speed speed;
	bool addheal;	//判断敌方是否已经加过1000的血
	bool to_me;		//判断敌方是否向我方运动
	int valid_time;	//说明数据来源的回合
	Object player;
	double Health;
};

void observe(Object A,Enemy* enemy);	//用于观察敌人信息
void observe_boss(Object A,Boss* Boss);	//用于观察boss信息

//关于光之隧道的函数
struct Store_adv
{
	int valid_time;		//如果看到光之隧道，存下看到的时间
	bool exist;			//如果光之隧道没有被吃掉，就存真，如果不存在，就存false
	Position pos;
	Store_adv* next;
};

void store_adv(Object A,Store_adv *Node);		//每次发现光之隧道A就自动存储

#endif











