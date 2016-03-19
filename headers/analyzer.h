#include "basic.h"

PlayerObject Self;
//通用的一些
Position cloest(ObjectType A);	//返回最近的type为A的物体的位置
Position* pos(ObjectType A);	//返回视野内所有type为A的物体的位置
int count_type(ObjectType A);	//返回视野内type为A的物体的数量


//关于player的函数与结构
struct enemy		//己方对敌方的信息储存
{
	int ablilty;
	int maxhealth;
	int skills[5];	//分别代表敌方五个技能的等级
	int speed;
	bool addheal;	//判断敌方是否已经加过1000的血
	bool to_me;		//判断敌方是否向我方运动
};

//关于devour的函数
Position* dev_inway(Speed);				//判断是否有devour在输入速度上，若有则输出该刺球位置，无则输出NONE

//关于光之隧道的函数
struct adv_memory
{
	Position pos;
	int oldtime;		//上一次该光之隧道被吃掉的时间
	Position pos_dev;	//该光之隧道附近最近的刺球位置
}













