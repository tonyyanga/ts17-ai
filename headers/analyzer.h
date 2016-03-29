#ifndef ANALYZER
#define ANALYZER

#include "basic.h"
#include "processor.h"
#include "common.h"

struct density
	{
		Speed speed;
		int number;
		double weight;
	};

class analyzer
{
private:

public:
	const Map* map;
	const Status* status;								//��ǰ״̬
	int num_energy,num_adv_energy,num,num_devour;
	Position* pos_energy;
	Position* pos_adv_energy;
	Position* pos_devour;
	Position* pos_player;
	analyzer(struct Enemy* enemy,const Status* status,const Map* map);		//���캯��
	analyzer(struct SceneState* state);
	Object boss;				//��boss�Ļ���״̬
	Position closest(ObjectType A,Position p0);			//���ؾ�p0���������A��λ�ã�Ĭ��p0Ϊ����λ��
	Position closest(ObjectType A);			
	Position* inway(ObjectType B,Speed A);				//�ж�speed�������Ƿ������B�����򷵻�B��λ�ã������򷵻�null��
	~analyzer();
	density* best_way();								//ͨ������Ϊ���ĵİ뾶һ����Χ�ڵ�ʳ�ＯȺ����������ܳԵ����energy��density����
														//�ֲ��þ���ƽ�����Ƚ���Ȩֵ����
														//ע�⣡���ﷵ�ص�ֻ���ٶȵ�һ��ʸ��������ʵ�ʴ�С
};

//����player�ĺ�����ṹ

typedef struct Enemy		//�����Եз�����Ϣ����
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











