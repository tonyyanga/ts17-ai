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
#include "../headers/searchtree.h"

using namespace std;

namespace ai{
	Enemy* enemy;
	SceneState* state;
	processor* proc;
	int time;
	Boss boss;

	void AIMain();
	Enemy* Enemy_init();
	void search();

	void AIMain() {
		cout<<"AIMain Start."<<endl;
		// Initialize
		enemy = Enemy_init();
		state=(SceneState*)malloc(sizeof(SceneState));
		state->enemy=enemy;
		state->status= GetStatus();
		state->map=GetMap();
		cout<<"Going to start processor."<<endl;
		proc = new processor(state);
		cout<<"Processor init ends."<<endl;
		time = GetTime();
		thread search(search);
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
	void search() {
		int t;
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
				temp=state;
				state=NULL;
				delete temp;
				temp=(SceneState*)malloc(sizeof(SceneState));
				temp->enemy=enemy; // keep enemy updated
				temp->status= GetStatus();
				temp->map=GetMap();
				state=temp;
				cout<<"New state created. Calling proc update."<<endl;
				proc->update(state);
				//implement(proc->return_Order());
				delete temp;

				//do search
				cout<<"Search Tree init Start."<<endl;
				SearchTree* tree = new SearchTree((const SceneState*)state);

				// time critical
				cout<<"BFS Start."<<endl;
				tree->BFS();
				cout<<"DFS Start."<<endl;
				tree->DFS();

				// TODO: analyze?
				{
					cout<<"Search ends, analyzing."<<endl;
					SearchNode* SelectedNode;
					lnNode* orders;
					SelectedNode = tree->GetBestNode();
					orders = SelectedNode->getInstructionChain();
					cout<<"proc add instruction."<<endl;
					proc->AddInstruction((Instruction*) orders->dataptr, 1);
				}
				cout<<"do proc."<<endl;
				proc->temp_set_ins();
				proc->temp_implement();

				delete tree;
			}
		}
	}

}

void AIMain() {
	ai::AIMain();
}

