
#ifndef ANALYZER
#define ANALYZER

#include "basic.h"


class analyzer
{
private:

public:
	const Map* map;
	const Status* status;
	int num_energy,num_adv_energy,num,num_devour;
	Position* pos_energy;
	Position* pos_adv_energy;
	Position* pos_devour;
	Position* pos_player;
	analyzer(const Status* status,const Map* map);		//构造函数
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
<<<<<<< HEAD
	bool addheal;	//判断敌方是否已经加过1000的血
	bool to_me;		//判断敌方是否向我方运动
};

//关于devour的函数
Position* dev_inway(Speed);		//判断是否有devour在输入速度上，若有则输出该刺球位置，无则输出NONE
=======
	int addheal;	//判断敌方是否已经加血次数
	Object player;
	int valid_time;	//说明数据来源的回合
};

>>>>>>> c1f5d4297038b9a72ddc83978c53bf61d873f5f1

//关于光之隧道的函数
struct adv_memory
{
	Position pos;
	int oldtime;		//上一次该光之隧道被吃掉的时间		
};

#endif











