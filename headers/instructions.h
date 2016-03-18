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

enum InstructionType {
	MovePosition,
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

#endif