#include "../headers/teamstyle17.h"
#include "../headers/instructions.h"
#include "../headers/analyzer.h"
#include "../headers/processor.h"
void AIMain() {
	// Initialize
	static Enemy* enemy;
	enemy=new Enemy;
	enemy->addheal=0;
	enemy->maxr=1000;
	enemy->minr=1000;
	enemy->skills[0]=0;
	enemy->skills[1]=0;
	enemy->skills[2]=0;
	enemy->skills[3]=0;
	enemy->skills[4]=0;
	analyzer* s=new analyzer(enemy,GetStatus(),GetMap());
	processor processor(s);
	//Main AI


	//Process and execute order
	processor.return_Order();
}
