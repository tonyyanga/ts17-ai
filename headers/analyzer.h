#include "basic.h"

PlayerObject Self;
//ͨ�õ�һЩ
Position cloest(ObjectType A);	//���������typeΪA�������λ��
Position* pos(ObjectType A);	//������Ұ������typeΪA�������λ��
int count_type(ObjectType A);	//������Ұ��typeΪA�����������


//����player�ĺ�����ṹ
struct enemy		//�����Եз�����Ϣ����
{
	int ablilty;
	int maxhealth;
	int skills[5];	//�ֱ����з�������ܵĵȼ�
	int speed;
	bool addheal;	//�жϵз��Ƿ��Ѿ��ӹ�1000��Ѫ
	bool to_me;		//�жϵз��Ƿ����ҷ��˶�
};

//����devour�ĺ���
Position* dev_inway(Speed);				//�ж��Ƿ���devour�������ٶ��ϣ�����������ô���λ�ã��������NONE

//���ڹ�֮����ĺ���
struct adv_memory
{
	Position pos;
	int oldtime;		//��һ�θù�֮������Ե���ʱ��
	Position pos_dev;	//�ù�֮�����������Ĵ���λ��
}













