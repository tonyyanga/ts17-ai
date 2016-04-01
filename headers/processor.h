#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include "instructions.h"
#include "common.h"
#include "analyzer.h"

const int stack_size=5;

class LIFO
{
private:
	Instruction *s;
	int bottom;
public:
	LIFO(int size);
	Instruction pop();			//Pop last Instruction out
	int return_bottom();
	void push(Instruction t);	//Push Instruction in
	Instruction get_top();		//Get last Instruction
	void clear();				//Clear all
};
class FIFO
{
private:
	Instruction *s;
	int top;	
public:
	FIFO(int size);
	Instruction pop();
	int return_top();
	void push(Instruction t);
	Instruction get_top();
	void clear();
};
struct now_instruction
{
	Instruction i;
	int priority;
	int last_round;
	int valid_time;
};
class processor
{
public:
	processor(struct SceneState *s);
	bool update(struct SceneState *s);

	void temp_implement();
	void temp_set_ins();
	//TODO
	void AddInstruction(Instruction* order, int priority); // abstract interface to add orders, ## deep copy needed ##
	void choose_instruction();
	int judge_condition();
// implementation by cxn
private:
	APIOrder* Process_MovePosition(Position dest);
	struct SceneState* scene;
	class analyzer* state;
	now_instruction temp_in;	//Instruction to be done
	lnNode* multiple_temp;
	int multiple;
	LIFO l1,l2;
	FIFO f1,f2;
};

void implement(APIOrder x);

#endif