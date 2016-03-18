#ifndef PROCESSOR_H_
#define PROCESSOR_H_
#include "instructions.h"
#include "teamstyle17.h"


class LIFO
{
private:
	Instruction *s;
public:
	void init();
	Instruction pop();			//Pop last Instruction out
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
	void init();
	Instruction pop();
	void push(Instruction t);
	Instruction get_top();
	void clear();
};
#endif