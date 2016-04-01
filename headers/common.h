#ifndef COMMON_H_
#define COMMON_H_

#include "basic.h"
#include "analyzer.h"

int Usetime(bool add, Position &p, Position &p0, PlayerObject &Self);	//�������ٶȷ���ı�Ļغ�ʱ,���ش�p��p0�Ļغ���,addΪ�������ʹ�ü��ټ���

typedef struct SceneState {
	const struct Map* map;
	const struct Status* status;
	struct Enemy* enemy;
	struct Boss* boss;
	struct Store_adv* adv;
};

double angle(Position p1,Position p2);		//����p1,p2ʸ���нǣ�����arccosֵ��

double health(double radius);				//����뾶����Ѫ��
double get_radius(double Health);

#endif
