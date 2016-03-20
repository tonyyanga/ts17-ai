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
	analyzer(Enemy* enemy,const Status* status,const Map* map);		//���캯��
	Position closest(ObjectType A,Position p0);			//���ؾ�p0���������A��λ�ã�Ĭ��p0Ϊ����λ��
	Position closest(ObjectType A);			
	Position* inway(ObjectType B,Speed A);				//�ж�speed�������Ƿ������B�����򷵻�B��λ�ã������򷵻�null��
	~analyzer();
};

//����player�ĺ�����ṹ

struct Enemy		//�����Եз�����Ϣ����
{
	double maxr;
	double minr;
	int skills[5];	//�ֱ����з�������ܵĵȼ�
	Speed speed;
	bool addheal;	//�жϵз��Ƿ��Ѿ��ӹ�1000��Ѫ
	bool to_me;		//�жϵз��Ƿ����ҷ��˶�
	int valid_time;	//˵��������Դ�Ļغ�
	Object player;
};

void observe(Object A,Enemy* enemy);	//���ڹ۲������Ϣ

//���ڹ�֮����ĺ���

#endif











