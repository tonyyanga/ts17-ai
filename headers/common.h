#include "basic.h"

int usetime(bool add,Position &p,Position &p0,PlayerObject &Self);	//不计算速度方向改变的回合时,返回从p到p0的回合数,add为假则代表不使用加速技能

typedef struct SceneState {
	// below, ptr?
	Map map;
	Status status;
};

double angle(Position p1,Position p2);		//计算p1,p2矢量夹角（返回arccos值）