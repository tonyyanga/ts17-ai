#include <iostream>

#include "../headers/teamstyle17.h"
#include "../headers/instructions.h"
#include "../headers/analyzer.h"
#include "../headers/processor.h"


void AIMain() {
	// Initialize
	static Enemy* enemy = Ememy_init();
	analyzer* stage=new analyzer(enemy,GetStatus(),GetMap());
	processor* proc = new processor(stage);
	//Main AI


	//Process and execute order
	proc->return_Order();
}

Enemy* Ememy_init() {
	static Enemy* enemy=new Enemy;
	enemy->addheal=0;
	enemy->maxr=1000;
	enemy->minr=1000;
	enemy->skills[0]=0;
	enemy->skills[1]=0;
	enemy->skills[2]=0;
	enemy->skills[3]=0;
	enemy->skills[4]=0;
	return enemy;
}
