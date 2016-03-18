#include "basic.h"

int usetime(bool add,Position &p,Position &p0,PlayerObject &Self);	//不计算速度方向改变的回合时,返回从p到p0的回合数,add为假则代表不使用加速技能
