#include "basic.h"

//ͨ�õ�һЩ
int usetime(Position p);		//���ص���p������Ļغ���
Position cloest(ObjectType A);	//���������typeΪA�������λ��
Position* pos(ObjectType A);	//������Ұ������typeΪA�������λ��


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
bool dev_inway(Speed);				//�ж��Ƿ���devour�������ٶ���
Position avoid_newway(Position A);	//����Ҫ����A��Ҫ��·�����м��

//���ڹ�֮����ĺ���
struct adv_memory
{
	Position pos;
	int oldtime;		//��һ�θù�֮������Ե���ʱ��
	Position pos_dev;	//�ù�֮�����������Ĵ���λ��
}













