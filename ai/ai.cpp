#include <iostream>
#include <thread>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "../headers/teamstyle17.h"
#include "../headers/instructions.h"
#include "../headers/analyzer.h"
#include "../headers/processor.h"

using namespace std;

namespace ai{

	Enemy* enemy;
	SceneState* state;
	processor* proc;
	int time;

	void AIMain();
	Enemy* Enemy_init();
	void update_status();

	void AIMain() {
		// Initialize
		enemy = Enemy_init();
		state=(SceneState*)malloc(sizeof(SceneState));
		state->enemy=enemy;
		state->status= GetStatus();
		state->map=GetMap();
		proc = new processor(state);
		time = GetTime();
		thread update(update_status);

		//Main AI


		//Process and execute order
		proc->return_Order();
	}

	Enemy* Enemy_init() {
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

	void update_status() {
		int t;
		while(true){
			#ifdef WIN32
			Sleep(1000);
			#else
			sleep(2);
			#endif
			t=GetTime();
			if (t!=time) {
				time=t;

			}
		}		


	}

}

void AIMain() {
	ai::AIMain();
}