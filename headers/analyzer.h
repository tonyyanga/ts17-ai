
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
	analyzer(const Status* status,const Map* map);		//���캯��
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
<<<<<<< HEAD
	bool addheal;	//�жϵз��Ƿ��Ѿ��ӹ�1000��Ѫ
	bool to_me;		//�жϵз��Ƿ����ҷ��˶�
};

//����devour�ĺ���
Position* dev_inway(Speed);		//�ж��Ƿ���devour�������ٶ��ϣ�����������ô���λ�ã��������NONE
=======
	int addheal;	//�жϵз��Ƿ��Ѿ���Ѫ����
	Object player;
	int valid_time;	//˵��������Դ�Ļغ�
};

>>>>>>> c1f5d4297038b9a72ddc83978c53bf61d873f5f1

//���ڹ�֮����ĺ���
struct adv_memory
{
	Position pos;
	int oldtime;		//��һ�θù�֮������Ե���ʱ��		
};

#endif











