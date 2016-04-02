#ifndef ANALYZER
#define ANALYZER

#include "basic.h"
#include "processor.h"
#include "common.h"

struct density			//���ڱ�ʾʳ����ܶȷֲ�
	{
		Position pos;
		int number;
	};

struct Boss
{
	Object boss;
	int valid_time;
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
	Position* pos_boss;
	analyzer(struct Enemy* enemy,struct Boss* boss,struct Store_adv* firstnode,const Status* status,const Map* map);		//���캯��
	analyzer(struct SceneState* state);
	Position* closest(ObjectType A,Position p0);			//���ؾ�p0���������A��λ�ã�Ĭ��p0Ϊ����λ��
	Position* closest(ObjectType A);			
	Position* inway(ObjectType B,Speed A);				//�ж�speed�������Ƿ������B�����򷵻�B��λ�ã������򷵻�null��
	~analyzer();							
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
	double Health;
};

void observe(Object A,Enemy* enemy);	//���ڹ۲������Ϣ
void observe_boss(Object A,Boss* Boss);	//���ڹ۲�boss��Ϣ

//���ڹ�֮����ĺ���
struct Store_adv
{
	int valid_time;		//���������֮��������¿�����ʱ��
	bool exist;			//�����֮���û�б��Ե����ʹ��棬��������ڣ��ʹ�false
	Position pos;
	Store_adv* next;
};

void store_adv(Object A,Store_adv *Node);		//ÿ�η��ֹ�֮���A���Զ��洢

#endif











