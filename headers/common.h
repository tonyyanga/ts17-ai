#ifndef COMMON_H_
#define COMMON_H_

#include "basic.h"
#include "analyzer.h"

int Usetime(bool add, Position &p, Position &p0, PlayerObject &Self);	//不计算速度方向改变的回合时,返回从p到p0的回合数,add为假则代表不使用加速技能

typedef struct SceneState {
	const struct Map* map;
	const struct Status* status;
	struct Enemy* enemy;
	struct Boss* boss;
	struct Store_adv* adv;
};

double angle(Position p1,Position p2);		//计算p1,p2矢量夹角（返回arccos值）

double health(double radius);				//输入半径返回血量
double get_radius(double Health);

#endif
