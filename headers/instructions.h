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
typedef struct order
{
	int x;				//order type
	lnNode* argvs;		//arguments
	order* next;		//next order
};
class APIOrder {
	//Creates a series of orders according to given instruction
	//Contains the order to be done now, arguments and next order.
	//If all order of this instruction is done, next order is NULL
public:
	int type;			// see enum APIorderType			**should be in order
	int *p1;			//object id,						**could be seen in processor
	void *p2;			//arguments							**should be in order
	order order;		
	APIOrder();			
	void create_orders();//									**creates orders according to instructions
	bool if_elseFinish();// 判断now_instruction是否已经被执行,如果已经执行，则返回1，否则返回0   **simply check if order.next==NULL, if NULL then free all orders
	bool if_elseNeed();//判断执行now_instruction是否有必要，如果有必要，则返回0，否则返回1
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