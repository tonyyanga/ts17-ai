#ifndef ANALYZER_H_
#define ANALYZER_H_

#include "basic.h"


//ͨ�õ�һЩ
Position cloest(ObjectType A);	//���������typeΪA�������λ��
Position* pos(ObjectType A);	//������Ұ������typeΪA�������λ��
int count_type(ObjectType A);	//������Ұ��typeΪA�����������

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
	analyzer(const Status* status,const Map* map);		//���캯��
	Position* get_pos(ObjectType A);					//����A�������λ�ã�position���У�
	Position cloest(ObjectType A,Position p0);			//���ؾ�p0���������A��λ�ã�Ĭ��p0Ϊ����λ��
	Position cloest(ObjectType A);			
	Position* inway(ObjectType B,Speed A);				//�ж�speed�������Ƿ������B�����򷵻�B��λ�ã������򷵻�null��
	~analyzer();
};

//����player�ĺ�����ṹ
void observe(Object A);	//���ڹ۲������Ϣ

struct Enemy		//�����Եз�����Ϣ����
{
	double maxr;
	double minr;
	int skills[5];	//�ֱ����з�������ܵĵȼ�
	Speed speed;
	int addheal;	//�жϵз��Ƿ��Ѿ���Ѫ����
	Object player;
};


//���ڹ�֮����ĺ���
struct adv_memory
{
	Position pos;
	int oldtime;		//��һ�θù�֮������Ե���ʱ��		
};

#endif











