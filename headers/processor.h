#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include "instructions.h"
#include "common.h"

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
	void push(Instruction t,int priority);	//Push Instruction in
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
	void push(Instruction t,int priority);
	Instruction get_top();
	void clear();
};
struct now_instruction
{
	Instruction i;
	int priority;
};
class processor
{
private:
	LIFO l1,l2;
	FIFO f1,f2;
	
	//Below by cxn
	SceneState* CurrentState; // Derek, please keep this up to date


public:
	processor(LIFO l1,LIFO l2,FIFO f1,FIFO f2,SceneState* s);
	now_instruction temp_in;
	APIOrder now_order;
	void choose_instruction();
	int judge_condition();
	APIOrder return_Order();	//Cxn, need you to complete this part (may need to use now_order)
// implementation by cxn
private:
	APIOrder* Process_MovePosition(Position dest);
};

#endif