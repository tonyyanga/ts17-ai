#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include "instructions.h"

const int stack_size=5;

class LIFO
{
private:
	Instruction *s;
public:
	void init();
	Instruction pop();			//Pop last Instruction out
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
	void init();
	Instruction pop();
	void push(Instruction t,int priority);
	Instruction get_top();
	void clear();
};
class processor
{
private:
	LIFO l1,l2;
	FIFO f1,f2;
	now_instruction temp_in;
public:
	void init();
	void do_instruction();
}

struct now_instruction
{
	Instruction i;
	int priority;
}
#endif