#ifndef INSTRUCTIONS
#define INSTRUCTIONS

#include "basictypes.h"

typedef struct Instruction {
	int type; // see enum InstructionType
	lnNode* argvs;
	lnNode* conditons; // dataptr as InstructionType*
};

typedef struct InstructionCondition {
	int type; // see enum InstructionConditonType
	lnNode* argvs;
};

//below by cxn
class APIOrder {
public:
	int type;// see enum APIorderType
	int *p1;
	void *p2;
	APIOrder();
	bool if_elseFinish();// �ж�now_instruction�Ƿ��Ѿ���ִ��,����Ѿ�ִ�У��򷵻�1�����򷵻�0
	bool if_elseNeed();//�ж�ִ��now_instruction�Ƿ��б�Ҫ������б�Ҫ���򷵻�1�����򷵻�0
	void Do_it();
};


enum InstructionType {
	MovePosition,
	EatAdvancedEnergy,
	Approach,
	Escape,
	Skill_ShortAttack,
	Skill_LongAttack,
	Skill_Dash,
	Skill_Shield,
	Skill_HealthUp,
	Skill_UpgradeSkill,
	MultiInstructions,
	
	kInstructionType
};

enum InstructionConditionType{
	MoveDistance,
	MinSelfHealth,
	MaxTargetHealth,
	MinSpeedAdvantage,
	MinSkillPoint,

	kInstructionConditionType
};

enum APIorderType {
	API_StayStill,
	API_Switchspeed,
	API_UpgradeSkill,
	API_ShortAttack,
	API_LongAttack,
	API_Dash,
	API_Shield,
	API_HealthUp,
	MultiAPIorder,
	Finished,

	kAPIorederType
};
#endif