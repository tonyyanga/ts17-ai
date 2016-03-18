#include "../headers/instructions.h"
#include "../headers/processor.h"

int stack_size=5;
int priority_level=2;
void LIFO::init()
{
	s=new Instruction[stack_size];
};

Instruction LIFO::pop()		//Pop first Instruction out
{
	Instruction i=s[0];
	s[0]=s[1],s[1]=s[2],s[2]=s[3],s[3]=s[4];
	return i;
}

void LIFO::push(Instruction t)
{
	s[4]=s[3],s[3]=s[2],s[2]=s[1],s[1]=s[0],s[0]=t;
}

Instruction LIFO::get_top()
{
	return s[0];
}

void LIFO::clear()
{
}

void FIFO::init()
{
	top=0;
	s=new Instruction[stack_size];
};

Instruction FIFO::pop()	
{
	return s[top--];
}

void FIFO::push(Instruction t)
{
	s[4]=s[3],s[3]=s[2],s[2]=s[1],s[1]=s[0],s[0]=t;
	top++;
}

Instruction FIFO::get_top()
{
	return s[top];
}

void FIFO::clear()
{
	top=0;
}