#include "basic.h"

int usetime(bool add,Position &p,Position &p0,PlayerObject &Self);	//�������ٶȷ���ı�Ļغ�ʱ,���ش�p��p0�Ļغ���,addΪ�������ʹ�ü��ټ���

typedef struct SceneState {
	// below, ptr?
	Map map;
	Status status;
};

double angle(Position p1,Position p2);		//����p1,p2ʸ���нǣ�����arccosֵ��