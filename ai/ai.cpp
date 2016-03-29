﻿#include <iostream>
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
#include "../headers/searchtree.h"

using namespace std;

namespace ai{
	Enemy* enemy;
	SceneState* state;
	processor* proc;
	int time;

	void AIMain();
	Enemy* Enemy_init();
	void do_search();

	void AIMain() {
		// Initialize
		enemy = Enemy_init();
		state=(SceneState*)malloc(sizeof(SceneState));
		state->enemy=enemy;
		state->status= GetStatus();
		state->map=GetMap();
		proc = new processor(state);
		time = GetTime();
		thread search(do_search);
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

	void search() {
		int t;
		while(true){
			#ifdef WIN32
			Sleep(100);
			#else
			usleep(100);
			#endif
			t=GetTime();
			if (t!=time) {
				// update
				SceneState* temp;
				time=t;
				temp=state;
				state=NULL;
				delete temp;
				temp=(SceneState*)malloc(sizeof(SceneState));
				temp->enemy=enemy; // keep enemy updated
				temp->status= GetStatus();
				temp->map=GetMap();
				state=temp;
				proc->update(state);
				//implement(proc->return_Order());
				delete temp;

				//do search
				SearchTree* tree = new SearchTree((const SceneState*)state);

				// time critical
				tree->BFS();
				tree->DFS();

				// TODO: analyze?
				{
					SearchNode* SelectedNode;
					lnNode* orders;
					SelectedNode = tree->GetBestNode();
					orders = SelectedNode->getInstructionChain();

					//TODO: remove some items in this order chain, or choose one
					//TODO: add instructions to processor
				}

				implement(proc->return_Order());
			}
		}
	}

}

void AIMain() {
	ai::AIMain();
}

