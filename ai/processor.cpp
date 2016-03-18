#include "../headers/instructions.h"
#include "../headers/processor.h"
#include "../headers/teamstyle17.h"
LIFO::LIFO(int size)
{
	s=new Instruction[size];
	bottom=0;
};

Instruction LIFO::pop()		//Pop first Instruction out
{
	Instruction i=s[0];
	s[0]=s[1],s[1]=s[2],s[2]=s[3],s[3]=s[4];
	bottom--;
	return i;
}

void LIFO::push(Instruction t,int priority)
{
	s[4]=s[3],s[3]=s[2],s[2]=s[1],s[1]=s[0],s[0]=t;
	bottom++;
}

Instruction LIFO::get_top()
{
	return s[0];
}

int LIFO::return_bottom()
{
	return bottom;
}

void LIFO::clear()
{
}

FIFO::FIFO(int size)
{
	top=0;
	s=new Instruction[size];
};

Instruction FIFO::pop()	
{
	return s[top--];
}

int FIFO::return_top()
{
	return top;
}

void FIFO::push(Instruction t,int priority)
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

void processor::init()
{
	temp_in.priority=0;
}

void processor::choose_instruction()
{
	if ((l1.return_bottom()!=0) && (temp_in.priority<4))
		temp_in.i=l1.pop();
	else if ((f1.return_top()!=0) && (temp_in.priority<3))
		temp_in.i=f1.pop();
	else if ((l2.return_bottom()!=0) && (temp_in.priority<2))
		temp_in.i=l2.pop();
	else if ((f2.return_top()!=0) && (temp_in.priority<1))
		temp_in.i=f2.pop();
}