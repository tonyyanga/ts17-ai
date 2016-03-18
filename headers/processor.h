#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include "instructions.h"

const int stack_size=5;

class LIFO
{
private:
	Instruction *s;
	int bottom;
public:
	void init();
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
	void init();
	Instruction pop();
	int return_top();
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
	void choose_instruction();
	void do_instruction():		//Todo
}

struct now_instruction
{
	Instruction i;
	int priority;
}
#endif