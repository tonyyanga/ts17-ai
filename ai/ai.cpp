#include <iostream>
#include <thread>

#include <ctime>

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
#include "../headers/common.h"

using namespace std;

namespace ai{

	void AIMain();
	Enemy* Enemy_init();
	void search();
	Boss* Boss_init();
	void Search_EXEC(SearchTree* tree);

	Enemy* enemy;
	SceneState* state;
	processor* proc;
	int time;
	Boss* boss=Boss_init();
	
	void AIMain() {
		long start;
		cout<<"AIMain Start."<<endl;
		// Initialize
		start=clock();
		enemy = Enemy_init();
		state=(SceneState*)malloc(sizeof(SceneState));
		state->enemy=enemy;
		state->status=GetStatus();
		state->map=GetMap();
		state->adv=NULL;
		state->boss=boss;
		cout<<"Going to start processor. Time used "<<clock()-start<<endl;
		start=clock();
		proc = new processor(state);
		cout<<"Processor init ends. Time used "<<clock()-start<<endl;
		time = GetTime();
		search();
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

	Boss* Boss_init()
	{
		Boss* boss=new Boss;		
		return boss;
	}

	Store_adv* firstnode_init()
	{
		Store_adv* firstnode=NULL;
		return(firstnode);
	}

	void Search_EXEC(SearchTree* tree) {
	long start;
	// time critical
	cout<<"BFS Start."<<endl;
	start=clock();
	tree->BFS();
	cout<<"DFS Start. BFS time "<<clock()-start<<endl;
	start=clock();
	tree->DFS();
	cout<<"DFS time "<<clock()-start<<endl;
	// TODO: analyze?
				{
					cout<<"Search ends, analyzing."<<endl;
					SearchNode* SelectedNode;
					lnNode* orders;
					SelectedNode = tree->GetBestNode();
					orders = SelectedNode->getInstructionChain();
					cout<<"proc add instruction."<<endl;
					proc->AddInstruction((Instruction*) orders->dataptr, 2);
				}
	delete tree;
}
	void search() {
		int t;
		long start;
		cout<<"Search Thread Start."<<endl;
		while(true){
			#ifdef WIN32
			Sleep(1);
			#else
			usleep(1);
			#endif
			t=GetTime();
			if (t!=time) {
				cout<<"New Time."<<endl;
				// update, split it from the search tree for real-time performance
				SceneState* temp;
				time=t;
				start=clock();
				temp=(SceneState*)malloc(sizeof(SceneState));
				temp->enemy=enemy; // keep enemy updated
				temp->status= GetStatus();
				temp->map=GetMap();
				temp->boss=boss;
				temp->adv=NULL;
				memcpy(state, temp, sizeof(SceneState));
				free(temp);
				cout<<"New state created. Calling proc update. Time used "<<clock()-start<<endl;
				proc->update(state);
				//implement(proc->return_Order());

				//do search
				cout<<"Search Tree init Start."<<endl;
				SceneState* newstate=new SceneState;
				if (state->adv) {
					newstate->adv=new Store_adv(*state->adv);
				} else {
					newstate->adv=NULL;
				}
				newstate->boss=new Boss(*state->boss);
				newstate->enemy=new Enemy(*state->enemy);
				newstate->map=new Map(*state->map);
				newstate->status=new Status(*state->status);

				SearchTree* tree = new SearchTree(newstate);

				thread* EXEC=new thread(ai::Search_EXEC, tree);
				
				cout<<"do proc."<<endl;
				proc->temp_set_ins();
				proc->temp_implement();
			}
		}
	}
}



void AIMain() {
	ai::AIMain();
}

