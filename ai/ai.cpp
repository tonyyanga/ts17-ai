//#define SORTING
#include <stdlib.h>
#include <new>
#include <iostream>
#include <cmath>
#include <cstring>
#include <thread>
#include <ctime>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

//#include "../communicate/basic.h"
//#include "teamstyle17.h"

#include "../headers/basic.h"
#include "../headers/teamstyle17.h"


#define _CD const double
#define PI 3.1415926

using namespace std;

typedef struct SceneState {
	const struct Map* map;
	const struct Status* status;
	struct Enemy* enemy;
	struct Boss* boss;
	struct Store_adv* adv;
};

struct Map filter(struct Map input);

double angle(Position p1,Position p2);		//计算p1,p2矢量夹角（返回arccos值）

double health(double radius);				//输入半径返回血量
double get_radius(double Health);

typedef struct lnNode{
	// a basic node of a linked list
	// make .next = NULL to make the end
	void* dataptr;
	lnNode* next;
};

typedef struct binaryNode{
	// a basic node of a linked list
	// make .next = NULL to make the end
	void* dataptr;
	lnNode* left;
	lnNode* right;
};

class MaxHeapNode{
public:
	double number;
};

class MaxHeap{
	// sort items with their pointers, by ptr->number

	// for temporary implementation, use insert sort
public:
	//constructor
	MaxHeap();
	~MaxHeap();

	// get the best several items, item pointer when n=1, linkedlistNode when n>1
	MaxHeapNode** getmax(int n=1);
	lnNode* returnall();
	void additem(class SearchNode* item);

private:
	lnNode* root;
};

struct density			//用于表示食物的密度分布
	{
		Speed speed;
		int number;
	};

struct Boss
{
	Object boss;
	int valid_time;
};
typedef struct Instruction {
	int type; // see enum InstructionType
	lnNode* argvs;
	lnNode* conditons; // dataptr as InstructionType*
};

typedef struct InstructionCondition {
	int type; // see enum InstructionConditonType
	lnNode* argvs;
};

//below by cxn
typedef struct Order
{
	int x;				//order type
	lnNode* argvs;		//arguments
	Order* next;		//next order
};
class APIOrder {
	//Creates a series of orders according to given instruction
	//Contains the order to be done now, arguments and next order.
	//If all order of this instruction is done, next order is NULL
public:
	int type;			// see enum APIorderType			**should be in order
	int *p1;			//object id,						**could be seen in processor
	void *p2;			//arguments							**should be in order
	Order* order;		
	APIOrder();			
	void Do_it();//									**creates orders according to instructions
	void if_elseFinish();//							**simply check if order.next==NULL, if NULL then free all orders
	bool if_elseNeed();//判断执行now_instruction是否有必要，如果有必要，则返回0，否则返回1
	void next_order();//									**delete current order and move to next one
	void new_order();//										**delete all order and create new orders
};

enum InstructionType {
	MovePosition,
	EatAdvancedEnergy,
	Approach,
	Flee,
	Skill_ShortAttack,
	Skill_LongAttack,
	Skill_Dash,
	Skill_Shield,
	Skill_HealthUp,
	Skill_UpgradeSkill,
	MultiInstructions,
	
	kInstructionType
};

const int stack_size=5;

class LIFO
{
private:
	Instruction *s;
	int bottom;
public:
	LIFO(int size);
	Instruction pop();			//Pop last Instruction out
	int return_bottom();
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
	FIFO(int size);
	Instruction pop();
	int return_top();
	void push(Instruction t);
	Instruction get_top();
	void clear();
};
struct now_instruction
{
	Instruction i;
	int priority;
	int last_round;
	int valid_time;
};
class processor
{
public:
	processor(struct SceneState *s);
	bool update(struct SceneState *s);

	void temp_implement();
	void temp_set_ins();
	//TODO
	void AddInstruction(Instruction* order, int priority); // abstract interface to add orders, ## deep copy needed ##
	void choose_instruction();
	int judge_condition();
// implementation by cxn
private:
	APIOrder* Process_MovePosition(Position dest);
	struct SceneState* scene;
	class analyzer* state;
	now_instruction temp_in;	//Instruction to be done
	lnNode* multiple_temp;
	int multiple;
	LIFO l1,l2;
	FIFO f1,f2;
};

class SearchNode:public MaxHeapNode {
public:
	int depth; // tree depth
	bool spanned;
	SearchNode* father; // pointer to father node
	Instruction* order;
	double number; // work with MaxHeap
private:
	lnNode* children; // linked list to child nodes
	const SceneState* state;
	int gameover_state;

public:
	// constructor
	SearchNode(const SceneState* state, SearchNode* father, Instruction* order);
	~SearchNode();

	double evaluate(); // return evaluated value of this state AND SET gameover_state
	SearchNode* getbestchild(); // return the best child
	lnNode* rankchildren(); // return sorted child linked list, the first should be the best
	lnNode* reverserankchildren(); // return sorted child linked list, the first should be the worst
	int span(); // span this node, return number of children
	int gameover(); //return 0 as not over yet, 1 as winning, -1 as losing, 2 as eaten adv_energy
	bool haschildren();
	lnNode* getInstructionChain();

private:
	SceneState* Estimate(const Instruction* order); // calculate the next state
	lnNode* CheckPossibleOrders(); // return a linked list of possible orders
	void AddChild(const SceneState* state, Instruction* Order);
	static int count;
};

class SearchTree{
private:
	SearchNode* root;
	lnNode* HeapsAtDepth; // linked list to store heaps for nodes at certain depth
	
	//bool Ended
	int DFSdepth;

public:
	//constructor
	SearchTree(const SceneState* rootstate, int maxDepth=8); // root is at depth = 0
	~SearchTree();
	lnNode* EndedNodes; // heap for ended nodes pointers, TODO: provide a method to analyze it
	//return True when there are end nodes
	bool BFS(int BFSdepth=3);
	bool DFS(int width=1);
	
	SearchNode* GetBestNode(int depth=-1);
	lnNode* GetAllNodes(int depth=-1);
private:
	MaxHeap* GetHeap(int depth);

	bool search_layer_exec(const lnNode* layerNodes);
	bool search_layer_exec(const SearchNode* node);
	MaxHeap* extendheap();
};




void implement(APIOrder x);


enum InstructionConditionType{
	MoveDistance,
	MinSelfHealth,
	MaxTargetHealth,
	MinSpeedAdvantage,
	MinSkillPoint,

	kInstructionConditionType
};

enum APIorderType {
	API_StayStill,
	API_Switchspeed,
	API_UpgradeSkill,
	API_ShortAttack,
	API_LongAttack,
	API_Dash,
	API_Shield,
	API_HealthUp,
	MultiAPIorder,
	Finished,

	kAPIorederType
};

_CD rate1=99999;
_CD	rate2=1;
_CD rate3=1;
_CD rate4=100000;
_CD	rate5=1;
_CD rate6=1;
_CD rate_boss=1;
_CD rate_enemy=1;
_CD rate_boss_distance=1;


int Usetime(bool add, Position &p, Position &p0, PlayerObject &Self);	//不计算速度方向改变的回合时,返回从p到p0的回合数,add为假则代表不使用加速技能


class analyzer
{
private:

public:
	const Map* map;
	const Status* status;								//当前状态
	int num_energy,num_adv_energy,num,num_devour;
	Position* pos_energy;
	Position* pos_adv_energy;
	Position* pos_devour;
	Position* pos_player;
	Position* pos_boss;
	analyzer(struct Enemy* enemy,struct Boss* boss,struct Store_adv* firstnode,const Status* status,const Map* map);		//构造函数
	analyzer(struct SceneState* state);
	Position* closest(Position p0);			//返回据p0最近的物体A的位置，默认p0为自身位置
	Position* closest(ObjectType A);			
	Position* inway(ObjectType B,Speed A);				//判断speed方向上是否会碰到B，是则返回B的位置，不是则返回null）
	~analyzer();
	density* best_way();								//通过自身为中心的半径一定范围内的食物集群情况，返回能吃到最多energy的density数组
														//现采用距离平方反比进行权值估算
														//注意！这里返回的只是速度的一个矢量方向，无实际大小
};

//关于player的函数与结构

typedef struct Enemy		//己方对敌方的信息储存
{
	double maxr;
	double minr;
	int skills[5];	//分别代表敌方五个技能的等级
	Speed speed;
	bool addheal;	//判断敌方是否已经加过1000的血
	bool to_me;		//判断敌方是否向我方运动
	int valid_time;	//说明数据来源的回合
	Object player;
	double Health;
};

void observe(Object A,Enemy* enemy);	//用于观察敌人信息
void observe_boss(Object A,Boss* Boss);	//用于观察boss信息

//关于光之隧道的函数
struct Store_adv
{
	int valid_time;		//如果看到光之隧道，存下看到的时间
	bool exist;			//如果光之隧道没有被吃掉，就存真，如果不存在，就存false
	Position pos;
	Store_adv* next;
};

void store_adv(Object A,Store_adv *Node);		//每次发现光之隧道A就自动存储
















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
					lnNode* temp=tree->EndedNodes;
					while(temp) {
						SelectedNode=static_cast<SearchNode*>(temp->dataptr);	
						if (SelectedNode->gameover()==1) {
							Instruction* command = new Instruction;
							command->argvs = SelectedNode->getInstructionChain();
							cout<<"proc add multi instruction."<<endl;
							command->type=MultiInstructions;
							proc->AddInstruction(command, 4);
							break;
						} else if (SelectedNode->gameover()==2) {
							Instruction* command = new Instruction;
							command->argvs = SelectedNode->getInstructionChain();
							cout<<"proc add multi instruction."<<endl;
							command->type=MultiInstructions;
							proc->AddInstruction(command, 3);
						}
						temp=temp->next;
					}
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
				if (t%4==0)
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

void observe(Object A,Enemy* enemy)
{
	if (enemy!=NULL)
	{
		double s=100;
		int t=GetTime();		//当前回合数
		if (t-enemy->valid_time==1)				//连续两回合都在视野内
		{
			enemy->speed=Displacement(A.pos,enemy->player.pos);
			s=Norm(enemy->speed);
		}
		if (A.radius>enemy->maxr) enemy->maxr=A.radius;
		if (A.radius<enemy->minr) enemy->minr=A.radius;
		if (health(A.radius)-health(enemy->player.radius)>400) enemy->addheal++;
		enemy->player=A;
		if (A.shield_time>0) enemy->skills[2]=1;
		if (A.long_attack_casting>0) enemy->skills[1]=1;
		if (s>102) enemy->skills[3]=int((s-100)/20);
		enemy->valid_time=t;
		enemy->Health=health(A.radius);
	}
}

void observe_boss(Object A,Boss* Boss)
{
	Boss->boss.pos.x=A.pos.x;
	Boss->boss.pos.y=A.pos.y;
	Boss->boss.pos.z=A.pos.z;
	Boss->boss.radius=A.radius;
	Boss->boss.type=A.type;
	Boss->valid_time=GetTime();
}

void store_adv(Object A,Store_adv *first)
{
	Store_adv *node;
	node=first;
	bool flag=true;
	while (flag==true)
	{
		if (node==NULL) break;
		if (node->pos.x==A.pos.x && node->pos.y==A.pos.y && node->pos.z==A.pos.z)
			flag=false;
		node=node->next;
	}
	if (flag)
		node=new Store_adv;
		node->next=NULL;
		node->pos=A.pos;
		node->valid_time=GetTime();
		node->exist=1;
}



analyzer::analyzer(Enemy* enemy,Boss* boss,Store_adv* firstnode,const Status* status=GetStatus(),const Map* map=GetMap())

{
	int i;
	int m=0,n=0,q=0;
	this->status=status;
	num_energy=0;
	num_adv_energy=0;
	num_devour=0;
	for(i=0;i<map->objects_number;i++)
	{
		switch(map->objects[i].type)
		{
		case ENERGY:num_energy++;break;
		case ADVANCED_ENERGY:num_adv_energy++;break;
		case DEVOUR:num_devour++;break;
		}
	}
	/*cout<<"num_enengy"<<num_energy;
	cout<<"num_adv_enengy"<<num_adv_energy;
	cout<<"num_devour"<<num_devour;*/
	if (num_adv_energy!=0)
		pos_adv_energy=new Position[num_adv_energy];
	else
		pos_adv_energy=NULL;
	pos_devour=new Position[num_devour];
	pos_energy=new Position[num_energy];
	pos_player=new Position;
	pos_boss=new Position;
	for(i=0;i<map->objects_number;i++)
	{
		switch(map->objects[i].type)
		{
		case ENERGY:
			{
				pos_energy[m]=map->objects[i].pos;
				m++;
				break;
			}
		case ADVANCED_ENERGY:
			{
				if (num_adv_energy==0)
					break;								//*****************************//
				pos_adv_energy[n]=map->objects[i].pos;
				n++;
				store_adv(map->objects[i],firstnode);
				break;
			}
		case DEVOUR:
			{
				pos_devour[q]=map->objects[i].pos;
				q++;
				break;
			}
		case PLAYER: //****************** SSR: PLAYER IS NOT ENEMY!!!
			{
				*pos_player=map->objects[i].pos;
				if (i!=0)
					observe(map->objects[i],enemy);
				break;
			}
		case BOSS:
			{
				*pos_boss=map->objects[i].pos;
				observe_boss(map->objects[i],boss);
				break;
			}
		}
	}
}

analyzer::analyzer(SceneState* state) {
	new (this) analyzer(state->enemy, state->boss,state->adv,state->status, state->map);
}

Position* analyzer::closest(Position p0)
{
	int closest=0,i,n;
	Position* posA;
	posA=pos_energy;
	n=num_energy;
	for(i=1;i<n;i++)
	{
		if (Distance(p0,posA[i])<Distance(p0,posA[closest]))
			closest=i;
	}
	cout<<"cloest A:x="<<posA[closest].x;
	return(&posA[closest]);
}

Position* analyzer::closest(ObjectType A=ENERGY)
{
	int closest=0,i,n;
	Position* posA;
	Position p0=status->objects[0].pos;
	switch (A)
	{
	case ENERGY:
		{
			posA=pos_energy;
			n=num_energy;
			break;
		}
	case ADVANCED_ENERGY:
		{
			posA=pos_adv_energy;
			n=num_adv_energy;
			break;
		}
	case DEVOUR:
		{
			posA=pos_devour;
			n=num_devour;
			break;
		}
	}
	if (n==0) 
	{
		posA=NULL;
	}
	for(i=1;i<n;i++)
	{
		if (Distance(p0,posA[i])<Distance(p0,posA[closest]))
			closest=i;
	}
	//cout<<"cloest A:x="<<posA[closest].x;
	return(&posA[closest]);
}

Position* analyzer::inway(ObjectType B,Speed A)
{
	double distance;	//B到speed线段的距离
	int i,n;
	double r=status->objects[0].radius;
	Position linepoint=Displacement(status->objects[0].pos,A);
	Position* posA;
	switch (B)
	{
	case ENERGY:
		{
			posA=pos_energy;
			n=num_energy;
			break;
		}
	case ADVANCED_ENERGY:
		{
			posA=pos_adv_energy;
			n=num_adv_energy;
			break;
		}
	case DEVOUR:
		{
			posA=pos_devour;
			n=num_devour;
			break;
		}
	case BOSS:
		{
			posA=pos_boss;
			n=1;
		}
	}
	for(i=0;i<n;i++)
	{
		distance=PointLineDistance(posA[i],status->objects[0].pos,linepoint);
		if (distance<r) return(&posA[i]);
	}
	return(NULL);
}


/*density* divide(lnNode* first,Position *self,double r)
{
	lnNode* firstnode;
	lnNode* currentnode=first;
	Object* obj;
	Position pos_en;
	int dis;
	int x,y,z,i=0;
	density m[4][4][4];
	density t;
	for(x=0;x<4;x+=1)
	{
		for(y=0;y<4;y+=1)
		{
			for(z=0;z<4;z+=1)
			{
				m[x][y][z].pos.x=self->x+(x-2.5)*r;
				m[x][y][z].pos.y=self->y+(y-2.5)*r;
				m[x][y][z].pos.z=self->z+(z-2.5)*r;
				m[x][y][z].number=0;
				i++;
			}
		}
	}
	for(i=0;i<26;i++)
	{
		for(x=0;x<num_energy;x++)
		{
			d=PointLineDistance(pos_energy[x],selfpos,Displacement(selfpos,m[i].speed));
			if (d<r) 
			{
					dis=int(Norm(Displacement(pos_energy[x],selfpos)));				//为节省运算资源，将其作为int计算
					dis/=100;
					dis=dis*dis;
					m[i].weight+=1.0/dis;
					m[i].number++;
			}
		}
	}
	x=0;
	for(i=0;i<26;i++)
	{
		for(x=i+1;x<26;x++)
		{
			if (m[i].weight>m[x].weight) 
			{
				t[0]=m[i];
				m[i]=m[x];
				m[x]=t[0];
			}
		}
	}
	for(i=0;i<3;i++)
	{
		t[i]=m[i];
	}
	return(t);
}
=======
	while (currentnode!=NULL)
	{
		pos_en=*(Position*)currentnode->dataptr;
		x=(int)(pos_en.x+2.5*r-self->x)/r;
		y=(int)(pos_en.y+2.5*r-self->y)/r;
		z=(int)(pos_en.z+2.5*r-self->z)/r;
		m[x][y][z].number++;
		currentnode=currentnode->next;
	}
	return(&m[0][0][0]);
}

density* order(density m[4][4][4],Position *pos)
{
	int x,y,z;
	int a=0,b=0,c=0;
	for(x=0;x<4;x+=1)
	{
		for(y=0;y<4;y+=1)
		{
			for(z=0;z<4;z+=1)
			{
				if (m[x][y][z].number>m[a][b][c].number)
				{
					a=x;
					b=y;
					c=z;
				}
			}
		}
	}
	return(&m[a][b][c]);
}
*/


analyzer::~analyzer()
{
	/*delete[] pos_adv_energy;
	delete[] pos_devour;
	delete[] pos_energy;
	delete pos_player;*/
}

MaxHeap::MaxHeap() {
	this->root=NULL;
}

MaxHeap::~MaxHeap() {

}

void MaxHeap::additem(class SearchNode* item) {
	lnNode* temp=this->root;
	if (!temp) {
		root=(lnNode*)malloc(sizeof(lnNode));
		root->next=NULL;
		root->dataptr=item;
		return;
	}
	while(temp->next) {
		if (((MaxHeapNode*)(temp->dataptr))->number < item->number) {
			lnNode* newnode=(lnNode*)malloc(sizeof(lnNode));
			newnode->dataptr=temp->dataptr;
			newnode->next=temp->next;
			temp->next=newnode;
			temp->dataptr=item;
			return;
		}
		else
			temp=temp->next;
	}
	if (temp!=this->root||((MaxHeapNode*)(temp->dataptr))->number >= item->number) {
		lnNode* newnode=(lnNode*)malloc(sizeof(lnNode));
		newnode->dataptr=item;
		newnode->next=NULL;
		temp->next=newnode;
	} else {
		lnNode* newnode=(lnNode*)malloc(sizeof(lnNode));
		newnode->dataptr=item;
		newnode->next=this->root;
		this->root=newnode;
	}
}

lnNode* MaxHeap::returnall() {
	return this->root;//TODO: deep copy
}

MaxHeapNode** MaxHeap::getmax(int n) {
	MaxHeapNode** newroot=(MaxHeapNode**)malloc(n*sizeof(MaxHeapNode*));
	lnNode* track=this->root;
	MaxHeapNode** temp=newroot;
	int count;
	for (count=0;count<=n-1;count++) {
		if (!track)
			break;
		temp=newroot+count;
		*temp=(MaxHeapNode*)track->dataptr;
		track=track->next;
	}
	return newroot;
	// who shall clean up new root?
}

int Usetime(bool add,Position &p,Position &p0,PlayerObject &Self)	//不计算速度方向改变的回合时,返回从p到p0的回合数,add为假则代表不使用加速技能
{
	int time,da_time;
	int speed=100;
	double d;
	d=Distance(p0,p);
	da_time=Self.dash_time;
	if (add==false)
	{
		time=int(d/speed);
		time++;
		return(time);
	}
	else
	{
		speed+=20*Self.skill_level[DASH];
		time=int(d/speed);
		if (da_time==0) da_time=40;
		if (time<=da_time) return time;	//加速时间内就到达了目标位置
		else
		{
			d=d-speed*(da_time);
			time=da_time+int(d/100)+1;
			return(time);
		}
	}
}

double angle(Position p1,Position p2)
{
	return(acos(DotProduct(p1,p2)/(Norm(p1)*Norm(p2))));
}

double health(double radius)
{
	return((pow(radius/100,3)));
}

double get_radius(double Health)
{
	double r;
	r=100*pow(Health,1.0/3);
	return(r);
}

struct Map filter(struct Map input) {
	struct Map result;
	long length=0;
	long count;
	result.time=input.time;
	Object* result_obj=(Object*)malloc(input.objects_number*sizeof(Object));
	Object* temp=result_obj;
	for (count=0;count<=input.objects_number-1;count++) {
		if (input.objects[count].id!=ENERGY) {
			*temp=input.objects[count];
			temp++;
			length++;
		}
	}
	lnNode* parsedlists;
	lnNode* temp2=parsedlists;
	while(temp2) {
		*temp=*(Object*)temp2->dataptr;
		temp2=temp2->next;
		length++;
		temp++;
	}
	result.objects_number=length;
	memcpy(result.objects, result_obj, length*sizeof(Object));
	return result;
}

LIFO::LIFO(int size=5)
{
	s=new Instruction[size];
	bottom=0;
}

Instruction LIFO::pop()		//Pop first Instruction out
{
	Instruction i=s[0];
	s[0]=s[1],s[1]=s[2],s[2]=s[3],s[3]=s[4];
	bottom--;
	return i;
}

void LIFO::push(Instruction t)
{
	s[4]=s[3],s[3]=s[2],s[2]=s[1],s[1]=s[0],s[0]=t;
	if (bottom<4)
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
	bottom=0;
}

FIFO::FIFO(int size=5)
{
	top=0;
	s=new Instruction[size];
}

Instruction FIFO::pop()	
{
	return s[(top--)-1];
}

int FIFO::return_top()
{
	return top;
}

void FIFO::push(Instruction t)
{
	s[4]=s[3],s[3]=s[2],s[2]=s[1],s[1]=s[0],s[0]=t;
	if (top<4)
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

processor::processor(SceneState* s)
{
	this->l1=LIFO (5),this->l2=LIFO (5),this->f1=FIFO (5),this->f2=FIFO (5);
	scene=s;
	state=new analyzer(scene);
	temp_in.priority=0;
	temp_in.i.type=0;
	temp_in.i.argvs=new lnNode;
	Position* q=new Position(s->status->objects[0].pos);
	q->x+=200;
	q->y+=200;
	q->z+=200;
	temp_in.i.argvs->dataptr=q;
	cout<<"Processor initialized"<<endl;
}

void processor::choose_instruction()
{
	if ((l1.return_bottom()!=0) && (temp_in.priority<4))
		temp_in.i=l1.pop(),temp_in.priority=4;
	else if ((f1.return_top()!=0) && (temp_in.priority<3))
		temp_in.i=f1.pop(),temp_in.priority=3;
	else if ((l2.return_bottom()!=0) && (temp_in.priority<2))
		temp_in.i=l2.pop(),temp_in.priority=2;
	else if ((f2.return_top()!=0) && (temp_in.priority<1))
		temp_in.i=f2.pop(),temp_in.priority=1;
}

int processor::judge_condition()  //doesn't work yet
{
	lnNode* i=temp_in.i.conditons;
	return 1;	//0 if object doesn't meet the conditions
}


void implement(APIOrder x)
{
	switch (x.type)
	{
	case API_StayStill:; break;
	case API_Switchspeed:Move(*(x.p1), *((Speed*)x.p2)); break;
	case API_UpgradeSkill:UpgradeSkill(*(x.p1), *(SkillType*)x.p2); break;
	case API_ShortAttack:ShortAttack(*(x.p1)); break;
	case API_LongAttack:LongAttack(*(x.p1), *((int*)x.p2)); break;
	case API_Dash:Dash(*(x.p1)); break;
	case API_Shield:Shield(*(x.p1)); break;
	case API_HealthUp:HealthUp(*(x.p1)); break;
	}
}

void APIOrder::if_elseFinish()
{
	Order* Current_order = new Order;
	if ((*Current_order).next == NULL)
		free(Current_order);
	else next_order();
}

bool APIOrder::if_elseNeed()
{
	Enemy* Current_enemy = new Enemy;
	now_instruction* Current_instruction = new now_instruction;
	analyzer* Current_analyzer = (analyzer*)malloc(sizeof(analyzer));
	if ((*Current_instruction).i.type == Skill_Shield && (*(*Current_analyzer).status).objects[0].shield_time != 0)
		return 1;
	else if ((*Current_instruction).i.type == Skill_Dash && (*(*Current_analyzer).status).objects[0].dash_time != 0)
		return 1;
	else if ((*Current_instruction).i.type == Skill_LongAttack)
	{
		Position* Current_enemy_position = new Position;
		Position* Current_self_position = new Position;
		(*Current_enemy_position).x = (*Current_enemy).player.pos.x + (*Current_enemy).speed.x*(*(*Current_analyzer).status).objects[0].long_attack_casting;
		(*Current_enemy_position).y = (*Current_enemy).player.pos.y + (*Current_enemy).speed.y*(*(*Current_analyzer).status).objects[0].long_attack_casting;
		(*Current_enemy_position).x = (*Current_enemy).player.pos.z + (*Current_enemy).speed.z*(*(*Current_analyzer).status).objects[0].long_attack_casting;
		(*Current_self_position).x = (*(*Current_analyzer).status).objects[0].pos.x + (*(*Current_analyzer).status).objects[0].speed.x*(*(*Current_analyzer).status).objects[0].long_attack_casting;
		(*Current_self_position).y = (*(*Current_analyzer).status).objects[0].pos.y + (*(*Current_analyzer).status).objects[0].speed.y*(*(*Current_analyzer).status).objects[0].long_attack_casting;
		(*Current_self_position).z = (*(*Current_analyzer).status).objects[0].pos.z + (*(*Current_analyzer).status).objects[0].speed.z*(*(*Current_analyzer).status).objects[0].long_attack_casting;
		if (Distance(*Current_enemy_position, *Current_self_position) <= 3000 + (*(*Current_analyzer).status).objects[0].skill_level[LONG_ATTACK] * 500)
			return 0;
		else
			return 1;
		free(Current_enemy_position);
		free(Current_self_position);
	}
	else return 0;
	free(Current_analyzer);
	free(Current_enemy);
	free(Current_instruction);
}

void APIOrder::next_order()
{
	Order* Next_order = new Order;
	delete order;
	order = (*Next_order).next;
}

void APIOrder::Do_it()
{
	now_instruction* Current_instruction = new now_instruction;
	analyzer* Current_analyzer = (analyzer*)malloc(sizeof(analyzer));
	if ((*Current_instruction).i.type == Skill_ShortAttack)
	{
		type = API_ShortAttack;
		implement(*this);
	}
	if ((*Current_instruction).i.type == Skill_HealthUp)
	{
		type = API_HealthUp;
		implement(*this);
	}
	if ((*Current_instruction).i.type == Skill_Dash&&if_elseNeed()==0)
	{
		type = API_Dash;
		implement(*this);
	}
	if ((*Current_instruction).i.type == Skill_Shield&&if_elseNeed()==0)
	{
		type = API_Shield;
		implement(*this);
	}
	if ((*Current_instruction).i.type == Skill_LongAttack&&if_elseNeed()==0)
	{
		type = API_LongAttack;
		implement(*this);
	}
	if ((*Current_instruction).i.type == MovePosition|| (*Current_instruction).i.type==Approach|| (*Current_instruction).i.type == EatAdvancedEnergy|| (*Current_instruction).i.type == Flee)
	{
		int i, flag = 0;	//flag=0 means no object is inway;
		for (i = PLAYER; i < kObjectTypes;i++)
			if ((*Current_analyzer).inway(ObjectType(i), *(Position*)(*(*Current_instruction).i.argvs).dataptr) != NULL)
			{
				flag = 1;
				break;
			}
		if(flag==0)
		{
			type = API_Switchspeed;
			(*(Position*)p2) = Displacement((*(*Current_analyzer).status).objects[0].pos, *(Position*)(*(*Current_instruction).i.argvs).dataptr);
			implement(*this);
		}
		else
		{
			if((*Current_analyzer).inway(DEVOUR, *(Position*)(*(*Current_instruction).i.argvs).dataptr)!=NULL)
			type = API_Switchspeed;
			(*(Position*)p2) = CrossProduct((*(*Current_analyzer).status).objects[0].pos, *(Position*)(*(*Current_instruction).i.argvs).dataptr);
			implement(*this);
		}
	}
}

APIOrder::APIOrder()
{
	//Objected included in processor
}


void processor::temp_set_ins()
{
	if ((l1.return_bottom()!=0) && (temp_in.priority<4))
		temp_in.i=l1.pop(),temp_in.priority=4,temp_in.valid_time=GetTime(),multiple_temp=NULL,cout<<"Instruction updated"<<endl;
	else if ((f1.return_top()!=0) && (temp_in.priority<3))
		temp_in.i=f1.pop(),temp_in.priority=3,temp_in.valid_time=GetTime(),cout<<"Instruction updated"<<endl;
	else if ((l2.return_bottom()!=0) && (temp_in.priority<2))
		temp_in.i=l2.pop(),temp_in.priority=2,temp_in.valid_time=GetTime(),cout<<"Instruction updated"<<endl;
	else if ((f2.return_top()!=0) && (temp_in.priority<1))
		temp_in.i=f2.pop(),temp_in.priority=1,temp_in.valid_time=GetTime(),cout<<"Instruction updated"<<endl;
	switch(temp_in.i.type)
	{
	case InstructionType(MovePosition):
		{
			Position *p=(Position*)temp_in.i.argvs->dataptr;
			if (abs(scene->status->objects[0].pos.x-p->x)<100&&
				abs(scene->status->objects[0].pos.y-p->y)<100&&
				abs(scene->status->objects[0].pos.z-p->z)<100)
				temp_in.priority=0;
			multiple_temp=NULL;
			break;
		}
	case InstructionType(EatAdvancedEnergy):
		{
			Position *p=(Position*)temp_in.i.argvs->dataptr;
			if (abs(scene->status->objects[0].pos.x-p->x)<100&&
				abs(scene->status->objects[0].pos.y-p->y)<100&&
				abs(scene->status->objects[0].pos.z-p->z)<100)
				temp_in.priority=0;
			multiple_temp=NULL;
			break;
		}
	case InstructionType(Approach):
		{
			Object* p=(Object*)temp_in.i.argvs->dataptr;
			if (Distance(scene->status->objects[0].pos,p->pos)<=*(int*)(temp_in.i.argvs->next->dataptr))
				temp_in.priority=0;
			multiple_temp=NULL;
			break;
		}
	case InstructionType(Flee):
		{
			if (Distance(scene->status->objects[0].pos,scene->enemy->player.pos)<=*(int*)(temp_in.i.argvs->dataptr))
				temp_in.priority=0;
			multiple_temp=NULL;
			break;
		}
	case InstructionType(MultiInstructions):
		{
			multiple_temp=temp_in.i.argvs;
			temp_in.i=*(Instruction*)multiple_temp->dataptr;
			multiple=temp_in.priority;
			break;
		}
	default:
		{
			if (temp_in.valid_time!=GetTime())
			temp_in.priority=0;
			multiple_temp=NULL;
			break;
		}
	}
	cout<<"Instruction set to "<<temp_in.i.type<<endl;
	if (multiple_temp!=NULL&&temp_in.priority==0)
	{
		temp_in.priority=multiple;
		temp_in.i=*(Instruction*)multiple_temp->dataptr;
		multiple_temp=multiple_temp->next;
	}
}
void processor::temp_implement()
{
	int user_id=scene->status->objects[0].id;
	cout<<"Implementing "<<temp_in.i.type<<endl;
	switch(this->temp_in.i.type)
	{
	case InstructionType(MovePosition):
		{
			Position temp;
			double multiply;
			Position* p=(Position*)temp_in.i.argvs->dataptr;
			temp.x=p->x-scene->status->objects[0].pos.x;
			temp.y=p->y-scene->status->objects[0].pos.y;
			temp.z=p->z-scene->status->objects[0].pos.z;
			if (p->x<=scene->status->objects[0].radius||p->x>=kMapSize-scene->status->objects[0].radius)
				temp.x=kMapSize/2-scene->status->objects[0].pos.x;
			if (p->y<=scene->status->objects[0].radius||p->y>=kMapSize-scene->status->objects[0].radius)
				temp.y=kMapSize/2-scene->status->objects[0].pos.y;
			if (p->z<=scene->status->objects[0].radius||p->z>=kMapSize-scene->status->objects[0].radius)
				temp.z=kMapSize/2-scene->status->objects[0].pos.z;
			multiply=100 / Norm(temp);
			temp=Scale(multiply, temp);
			Move(user_id,temp);
			printf("MOVING TO %lf, %lf, %lf\n", temp.x, temp.y, temp.z);
			printf("PLAYER AT %lf, %lf, %lf\n", scene->status->objects[0].pos.x, scene->status->objects[0].pos.y, scene->status->objects[0].pos.z);
			break;
		}
	case InstructionType(EatAdvancedEnergy):
		{
			Position temp;
			Position* p=(Position*)temp_in.i.argvs->dataptr;
			temp.x=p->x-scene->status->objects[0].pos.x;
			temp.y=p->y-scene->status->objects[0].pos.y;
			temp.z=p->z-scene->status->objects[0].pos.z;
			Move(user_id,temp);
			break;
		}
	case InstructionType(Approach):
		{
			Object* p=(Object*)temp_in.i.argvs->dataptr;
			Position temp;
			Position t=p->pos;
			temp.x=t.x-scene->status->objects[0].pos.x;
			temp.y=t.y-scene->status->objects[0].pos.y;
			temp.z=t.z-scene->status->objects[0].pos.z;
			Move(user_id,temp);
			break;
		}
	case InstructionType(Flee):
		{
			Object* p=(Object*)temp_in.i.argvs->dataptr;
			Position temp;
			Position t=scene->enemy->player.pos;
			temp.x=-t.x+scene->status->objects[0].pos.x;
			temp.y=-t.y+scene->status->objects[0].pos.y;
			temp.z=-t.z+scene->status->objects[0].pos.z;
			Move(user_id,temp);
			break;
		}
	case InstructionType(Skill_ShortAttack):
		{
			ShortAttack(user_id);
			break;
		}
	case InstructionType(Skill_LongAttack):
		{
			LongAttack(user_id,*(int*)temp_in.i.argvs->dataptr);
			break;
		}
	case InstructionType(Skill_Dash):
		{
			Dash(user_id);
			break;
		}
	case InstructionType(Skill_Shield):
		{
			Shield(user_id);
			break;
		}
	case InstructionType(Skill_HealthUp):
		{
			HealthUp(user_id);
			break;
		}
	case InstructionType(Skill_UpgradeSkill):
		{
			UpgradeSkill(user_id,SkillType(*(int*)temp_in.i.argvs->dataptr));
			break;
		}
	}
}


void processor::AddInstruction(Instruction* i, int p)
{
	if (i->type==0&&i->argvs->dataptr==NULL)
	{
		Position* temp=new Position;
		temp->x=scene->status->objects[0].pos.x+200;
		temp->y=scene->status->objects[0].pos.y+200;
		temp->z=scene->status->objects[0].pos.z+200;
		i->argvs->dataptr=temp;
	}
	switch(p)
	{
	case 1:{
		if (i->type==0&&temp_in.i.type==0)
		{
			Position p=*(Position*)(i->argvs->dataptr);
			p=Displacement(scene->status->objects[0].pos,p);
			Position p2=Displacement(scene->status->objects[0].pos,*(Position*)temp_in.i.argvs->dataptr);
			if (Norm(CrossProduct(p,p2))<=0.1)
				break;
		}
		f2.push(*i);
		break;
		   }
	case 2:{
		if (i->type==0&&temp_in.i.type==0)
		{
			Position p=*(Position*)(i->argvs->dataptr);
			p=Displacement(scene->status->objects[0].pos,p);
			Position p2=Displacement(scene->status->objects[0].pos,*(Position*)temp_in.i.argvs->dataptr);
			if (Norm(CrossProduct(p,p2))<=0.1)
				break;
		}
		l2.push(*i);
		break;
		   }
	case 3:{
		if (i->type==0&&temp_in.i.type==0)
		{
			Position p=*(Position*)(i->argvs->dataptr);
			p=Displacement(scene->status->objects[0].pos,p);
			Position p2=Displacement(scene->status->objects[0].pos,*(Position*)temp_in.i.argvs->dataptr);
			if (Norm(CrossProduct(p,p2))<=0.1)
				break;
		}
		f1.push(*i);
		break;
		   }
	case 4:{
		if (i->type==0&&temp_in.i.type==0)
		{
			Position p=*(Position*)(i->argvs->dataptr);
			p=Displacement(scene->status->objects[0].pos,p);
			Position p2=Displacement(scene->status->objects[0].pos,*(Position*)temp_in.i.argvs->dataptr);
			if (Norm(CrossProduct(p,p2))<=0.1)
				break;
		}
		l1.push(*i);
		break;
		   }
	}
	cout<<"Instruction added"<<endl;
}

bool processor::update(SceneState* s)
{
	if (scene!=s)
	{
		scene=s;
		cout<<"Processor scene updated"<<endl;
		return 1;
	}
	else
	{
		scene=s;
		cout<<"Processor scene updated"<<endl;
		return 0;
	}
}

int SearchNode::count=0;

SearchNode::SearchNode(const SceneState* state, SearchNode* father, Instruction* order) {
	this->state=state;
	this->father=father;
	this->order=order;
	this->spanned=false;
	this->children=NULL;
	long start=clock();
	this->number=this->evaluate();
	cout<<"EVAL TIME = !!!!!!!!!!!!!! "<<clock()-start<<endl;
	if (father) {
		this->depth=father->depth+1;
	} else {
		this->depth=0;
	}
	count++;
	//std::cout<<count<<std::endl;
}

SearchNode::~SearchNode() {

}

#ifdef SORTING
lnNode* SearchNode::rankchildren() {
	lnNode* temp=this->children;
	MaxHeap sorter;
	if (!temp)
		return NULL;
	while(temp->next) {
		sorter.additem(static_cast<MaxHeapNode*>(temp->dataptr));
		temp=temp->next;
	}
	return sorter.returnall();
}
#else
lnNode* SearchNode::rankchildren() {
	return this->children;
}
#endif

lnNode* SearchNode::getInstructionChain() {
	lnNode* temp;
	lnNode* root;
	SearchNode* track=this->father;
	if (this->order) {
		root=(lnNode*)malloc(sizeof(lnNode));
		root->next=NULL;
		root->dataptr=this->order;
		while(track->order) {
			temp=(lnNode*)malloc(sizeof(lnNode));
			temp->dataptr=track->order;
			temp->next=root;
			root=temp;
			track=track->father;
		}
		return root;
	} else
		return NULL;
}

bool SearchNode::haschildren() {
	return (this->children != NULL);
}

int SearchNode::span() {
	int count=0;
	long start=clock();
	lnNode* temp = this->CheckPossibleOrders();
	cout<<"CHECK ORDER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<clock()-start<<endl;
	
	while(temp) {
		this->AddChild(this->Estimate((Instruction*)(temp->dataptr)), (Instruction*)(temp->dataptr));
		temp = temp->next;
		count++;
	}
	this->spanned=true;
	return count;
}

void SearchNode::AddChild(const SceneState* state, Instruction* order) {
	SearchNode* child=new SearchNode(state, this, order);
	lnNode* temp = (lnNode*)malloc(sizeof(lnNode));
	temp->next=this->children;
	temp->dataptr=child;
	this->children=temp;
}

int SearchNode::gameover() {
	return this->gameover_state;
}

SceneState* SearchNode::Estimate(const Instruction* order) {
	long start=clock();
	cout<<"ENTER ESTIMATE"<<endl;
	SceneState* estimate=new SceneState(*state);
	int t,s=0,n=0;
	double heal;
	Position* new_position=new Position;
	Object obj;
	int type=order->type;
	lnNode *argv=order->argvs;
	Status status=*(state->status);
	PlayerObject* Self=status.objects;
	Map map=*state->map;
	Enemy enemy=*state->enemy;
	Boss boss=*state->boss;
	switch(order->type)
	{
	case 0:
		{
			if (argv->dataptr==NULL)
			{
				new_position->x=Self->pos.x+200;
				new_position->y=Self->pos.y+200;
				new_position->z=Self->pos.z+200;
			}
			else new_position=(Position *)(argv->dataptr);
			map.time+=Usetime(!Self->skill_cd[DASH],*new_position,Self->pos,*Self);
			for(t=0;t<n;t++)
			{
				if (PointLineDistance(map.objects[t].pos,Self->pos,*new_position)<Self->radius)
				{
					obj=map.objects[t];
					if (obj.type==ENERGY) Self->health+=40;
					map.objects[t]=map.objects[n];
					map.objects[n]=obj;
					n--;
				}
			}
			map.objects_number=n;
			estimate->map=&map;
			Self->pos.x=new_position->x;
			Self->pos.y=new_position->y;
			Self->pos.z=new_position->z;
			break;
		}
	case 1:
		{
			if (argv->dataptr==NULL)
			{
				new_position->x=Self->pos.x+200;
				new_position->y=Self->pos.y+200;
				new_position->z=Self->pos.z+200;
			}
			else new_position=(Position *)(argv->dataptr);
			map.time+=Usetime(!Self->skill_cd[DASH],*new_position,Self->pos,*Self);
			for(t=0;t<n;t++)
			{
				if (PointLineDistance(map.objects[t].pos,Self->pos,*new_position)<Self->radius)
				{
					obj=map.objects[t];
					if (obj.type==ENERGY) Self->health+=40;
					map.objects[t]=map.objects[n];
					map.objects[n]=obj;
					n--;
				}
			}
			map.objects_number=n;
			estimate->map=&map;
			Self->pos.x=new_position->x;
			Self->pos.y=new_position->y;
			Self->pos.z=new_position->z;
			Self->ability+=3;
			estimate->map=NULL;
			break;
		}
	case 2:
		{
			if (argv->dataptr==NULL)
			{
				new_position->x=Self->pos.x+200;
				new_position->y=Self->pos.y+200;
				new_position->z=Self->pos.z+200;
			}
			else new_position=(Position *)(argv->dataptr);
			map.time+=Usetime(!Self->skill_cd[DASH],*new_position,Self->pos,*Self);
			for(t=0;t<n;t++)
			{
				if (PointLineDistance(map.objects[t].pos,Self->pos,*new_position)<Self->radius)
				{
					obj=map.objects[t];
					if (obj.type==ENERGY) Self->health+=40;
					map.objects[t]=map.objects[n];
					map.objects[n]=obj;
					n--;
				}
			}
			map.objects_number=n;
			estimate->map=&map;
			Self->pos.x=new_position->x;
			Self->pos.y=new_position->y;
			Self->pos.z=new_position->z;
		}
	case 3:
		{
			if (argv->dataptr==NULL)
			{
				new_position->x=Self->pos.x+200;
				new_position->y=Self->pos.y+200;
				new_position->z=Self->pos.z+200;
			}
			else new_position=(Position *)(argv->dataptr);
			map.time+=Usetime(!Self->skill_cd[DASH],*new_position,Self->pos,*Self);
			for(t=0;t<n;t++)
			{
				if (PointLineDistance(map.objects[t].pos,Self->pos,*new_position)<Self->radius)
				{
					obj=map.objects[t];
					if (obj.type==ENERGY) Self->health+=40;
					map.objects[t]=map.objects[n];
					map.objects[n]=obj;
					n--;
				}
			}
			map.objects_number=n;
			estimate->map=&map;
			Self->pos.x=new_position->x;
			Self->pos.y=new_position->y;
			Self->pos.z=new_position->z;
		}
	case 5:
		{
			Self->skill_cd[0]=80;
			if (Norm(Displacement(enemy.player.pos,Self->pos))<=(2000+500*(Self->skill_level[0])+Self->radius+enemy.player.radius))
				{
					enemy.Health-=100*(Self->skill_level[0]);
					enemy.player.radius=get_radius(enemy.Health);
			}
			if (Norm(Displacement(boss.boss.pos,Self->pos))<=(2000+500*(Self->skill_level[0])+Self->radius+boss.boss.radius))
			{
				heal=health(boss.boss.radius);
				heal-=100*(Self->skill_level[0]);
				boss.boss.radius=get_radius(heal);
			}
			map.time+=1;
			estimate->map=&map;
			break;
		}
	case 4:
		{
			Self->skill_cd[1]=80;
			if (Norm(Displacement(enemy.player.pos,Self->pos))<=(1100+300*(Self->skill_level[1])+Self->radius+enemy.player.radius))
				{
					enemy.Health-=200+300*(Self->skill_level[1]);
					enemy.player.radius=get_radius(enemy.Health);
			}
			if (Norm(Displacement(boss.boss.pos,Self->pos))<=(1100+300*(Self->skill_level[1]+Self->radius+boss.boss.radius)))
			{
				heal=health(boss.boss.radius);
				heal-=200+300*(Self->skill_level[1]);
				boss.boss.radius=get_radius(heal);
			}
			map.time+=1;
			estimate->map=&map;
			break;
		}
	case 7:
		{
			Self->shield_time=20+10*Self->skill_level[2];
			Self->skill_cd[2]=100;
			map.time+=1;
			estimate->map=&map;
			break;
		}
	case 6:
		{
			Self->skill_cd[3]=100;
			map.time+=1;
			estimate->map=&map;
			break;
		}
	case 8:
		{
			Self->health+=500;
			if (Self->skill_level[5])
				Self->ability-=pow(2,Self->skill_level[5]);
			else
			{
				for(t=0;t<6;t++)
				{
					if (Self->skill_level[t]) s++;
					Self->ability-=pow(2,s);
				}
			}
			Self->skill_level[5]++;
			map.time+=1;
			estimate->map=&map;
			break;
		}
	case 9:
		{
			n=*(int *)(argv->dataptr);
			if (Self->skill_level[n])
			{
				if (n==2 || n==4)
					Self->ability-=2*pow(2,Self->skill_level[n]);
				else 
					Self->ability-=pow(2,Self->skill_level[n]);
			}
			else
			{
				for(t=0;t<6;t++)
				{
					if (Self->skill_level[t]) s++;
					if (n==2 || n==4)
						Self->ability-=2*pow(2,s);
					else 
						Self->ability-=pow(2,s);
				}
			}
			Self->skill_level[n]++;
			map.time+=1;
			estimate->map=&map;
			break;
		}
	}
	estimate->status=&status;
	estimate->boss=&boss;
	estimate->enemy=&enemy;
	cout<<"EXIT ESTIMATE, time = "<<clock()-start<<endl;
	return(estimate);
}

lnNode* SearchNode::CheckPossibleOrders()
{
	lnNode* l=new lnNode;
	lnNode* head=l;
	Instruction* t;
	lnNode* n=new lnNode;
	int time;
	if (state->map!=NULL)
		time=state->map->time;
	else
		time=0;
	lnNode* temp;
	if (this->gameover_state)
		return NULL;
	if ((state->boss->valid_time==time)||(state->enemy->valid_time==time))
	{
		Position* positions=new Position[3];
		SceneState s=*state;
		analyzer temp_analyzer(&s);
		for (int k=SkillType(LONG_ATTACK);k<=SkillType(HEALTH_UP);k++)
		{
			if (state->status->objects[0].ability>int(pow(2,state->status->objects[0].skill_level[k])*kBasicSkillPrice[k]))
			{
				t=new Instruction;
				t->type=InstructionType(Skill_UpgradeSkill);
				n=new lnNode;
				t->argvs=n;
				t->argvs->dataptr=new int(k);                        //*****************************************//
				temp=new lnNode;
				l->dataptr=t;
				l->next=temp;
				l=l->next;
			}
		}
		t=new Instruction;
		t->type=InstructionType(MovePosition);
		n=new lnNode;
		t->argvs=n;
		t->argvs->dataptr=temp_analyzer.closest(ENERGY);
		temp=new lnNode;
		l->dataptr=t;
		l->next=temp;
		l=l->next;
		if (temp_analyzer.pos_adv_energy!=NULL)
		{
			t=new Instruction;
			t->type=InstructionType(EatAdvancedEnergy);
			lnNode* n=new lnNode;
			n->dataptr=&temp_analyzer.pos_adv_energy[0];
			t->argvs=n;
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
			l=l->next;
		}
		if (state->boss->valid_time==time)
		{
			t=new Instruction;
			t->type=InstructionType(Approach);
			t->argvs=new lnNode;
			t->argvs->dataptr=new Object(state->boss->boss);
			t->argvs->next=new lnNode;
			t->argvs->next->dataptr=new double(state->status->objects[0].radius);
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
			l=l->next;
		}
		if (state->enemy->valid_time==time)
		{
			t=new Instruction;
			t->type=InstructionType(Approach);
			t->argvs=new lnNode;
			t->argvs->dataptr=new Object(state->enemy->player);
			t->argvs->next=new lnNode;
			t->argvs->next->dataptr=new double(state->status->objects[0].radius);
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
			l=l->next;
		}
		t=new Instruction;
		t->type=InstructionType(Flee);
		n=new lnNode;
		t->argvs=n;
		t->argvs->dataptr=new double(5000);
		temp=new lnNode;
		l->dataptr=t;
		(l->next)=temp;
		if (kShortAttackRange[state->status->objects[0].skill_level[SkillType(SHORT_ATTACK)]]>Distance(state->enemy->player.pos,state->enemy->player.pos)||
			kShortAttackRange[state->status->objects[0].skill_level[SkillType(SHORT_ATTACK)]]>Distance(state->enemy->player.pos,state->boss->boss.pos))
		{
			l=l->next;
			t=new Instruction;
			t->type=InstructionType(Skill_ShortAttack);
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
		}
		if (kShortAttackRange[state->status->objects[0].skill_level[SkillType(SHORT_ATTACK)]]>Distance(state->enemy->player.pos,state->enemy->player.pos))
		{
			l=l->next;
			t=new Instruction;
			t->type=InstructionType(Skill_LongAttack);
			lnNode* n=new lnNode;
			t->argvs=n;
			t->argvs->dataptr=new Object(state->enemy->player);
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
		}
		if (kShortAttackRange[state->status->objects[0].skill_level[SkillType(SHORT_ATTACK)]]>Distance(state->enemy->player.pos,state->boss->boss.pos))
		{
			l=l->next;
			t=new Instruction;
			t->type=InstructionType(Skill_LongAttack);
			lnNode* n=new lnNode;
			t->argvs=n;
			t->argvs->dataptr=new Object(state->boss->boss);
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
		}
		if (state->status->objects[0].skill_level[SkillType(DASH)]>0)
		{
			l=l->next;
			t=new Instruction;
			t->type=InstructionType(Skill_Dash);
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
		}
		if (state->status->objects[0].skill_level[SkillType(SHIELD)]>0)
		{
			l=l->next;
			t=new Instruction;
			t->type=InstructionType(Skill_Shield);
			temp=new lnNode;
			l->dataptr=t;
			l->next=temp;
		}
		l->next=NULL;
		delete temp;
		return head;
}
	else
	{
		SceneState s=*state;
		analyzer temp_analyzer(&s);
		if (temp_analyzer.pos_adv_energy!=NULL)
		{
			t=new Instruction;
			t->type=InstructionType(EatAdvancedEnergy);
			t->argvs=new lnNode;
			t->argvs->dataptr=new Position(temp_analyzer.pos_adv_energy[0]);
			t->argvs->next=NULL;
			l->dataptr=t;
			l->next=new lnNode;
			l=l->next;
		}
		t=new Instruction;
		t->type=InstructionType(MovePosition);
		n=new lnNode;
		t->argvs=n;
		t->argvs->dataptr=temp_analyzer.closest(ENERGY);
		t->argvs->next=NULL;
		temp=new lnNode;
		l->dataptr=t;
		l->next=NULL;
		return head;
	}
		

}

double SearchNode::evaluate()
{
	
	//Self: hp, skill point(different skills converted to ability), density_max, time
	//Enemy: hp, skill point
	//Boss: hp
	//Regular mode: density_max, hp, skill point, time ,distance to center
	//Battle mode: Power evaluating: hp_self/hp_enemy (0--1), self skill point-enemy skill point
	//			   If weaker:	Distance, Shield, Dash status
	//			   If stronger: Distance, Shield, Dash status, attack level
	//Boss mode:   Works after a certain time or if boss's hp is low enough
	//rate1: all skill points
	//rate2: total density of foods
	//rate3: time
	//rate4: distance to different positions in different cases
	if (state->map==NULL)
	{
		gameover_state=2;
		return this->father->evaluate()+rate1;
	}
	analyzer temp_analyzer((SceneState*)state);
	double food_density=0;
	int extra_ability=0;
	for (int i=SkillType(LONG_ATTACK);i<=SkillType(HEALTH_UP);i++)
	{
		for (int k=0;k<state->status->objects[0].skill_level[i];k++)
			extra_ability+=kBasicSkillPrice[i]*pow(2,k);
	}
	int time=state->map->time;
	bool enemy_in_sight=(state->enemy->valid_time==time);
	bool boss_in_sight=(state->boss->valid_time==time);
	//Enemy in sight
	if (enemy_in_sight==1&&boss_in_sight==0)
	{
		//initializing data
		int possible_extra_hp=0,hp_price=pow(2,state->status->objects[0].skill_level[SkillType(HEALTH_UP)]);
		int temp_ability=state->status->objects[0].ability;
		double kill_rate=pow(1.2,3),death_rate=1/kill_rate;
		while (temp_ability>hp_price)
		{
			possible_extra_hp+=500;
			temp_ability-=hp_price;
			hp_price=2*hp_price;
		}
		double original_hp_rate=state->status->objects[0].health/health(state->enemy->player.radius);
		double extra_hp_rate_self=(state->status->objects[0].health+possible_extra_hp)/health(state->enemy->player.radius);
		double distance=Distance(state->status->objects[0].pos,state->enemy->player.pos);


		//setting game over state
		gameover_state=0;
		if ((original_hp_rate<=death_rate&&distance<state->enemy->player.radius)||(state->status->objects[0].health<0.25*state->status->objects[0].max_health))
			gameover_state=-1;
		if ((original_hp_rate>=kill_rate&&distance<state->status->objects[0].radius)||(health(state->enemy->player.radius)<0.25*(health(state->enemy->maxr))))
			gameover_state=1;


		//evaluation
		if (original_hp_rate>1) //Add enemy's ability points into consideration!
		{
			//rate5: shield status, rate6: dash status
			return original_hp_rate*state->status->objects[0].health+
				rate1*((extra_ability+state->status->objects[0].ability)-3/* 3 should be ability points of enemy, do we have that?*/)+
				rate4*distance+
				rate5*(state->status->objects[0].shield_time>0)+
				rate6*(state->status->objects[0].dash_time>0)*(state->status->objects[0].skill_level[SkillType(DASH)])
				+9999*gameover_state;
		}
		else
		{
			//rate5: shield status, rate6: dash status
			return original_hp_rate*state->status->objects[0].health+
				rate4*distance+
				rate5*(state->status->objects[0].shield_time>0)+
				rate6*(state->status->objects[0].dash_time>0)*(state->status->objects[0].skill_level[SkillType(DASH)])
				+9999*gameover_state;
		}
	}
	else if (enemy_in_sight==0&&boss_in_sight==0)									//Regular mode
	{
		gameover_state=0;
		return state->status->objects[0].health+
			rate1*(extra_ability+state->status->objects[0].ability)+
			rate2*food_density+
			rate3*(time); //distance to center?
	}
	else if(enemy_in_sight==0&&boss_in_sight==1)//Only boss in sight
	{
		//initializing data
		int possible_extra_hp=0,hp_price=pow(2,state->status->objects[0].skill_level[SkillType(HEALTH_UP)]);
		int temp_ability=state->status->objects[0].ability;
		double kill_rate=pow(1.2,3),death_rate=1/kill_rate;
		while (temp_ability>hp_price)
		{
			possible_extra_hp+=500;
			temp_ability-=hp_price;
			hp_price=2*hp_price;
		}
		double original_hp_rate=state->status->objects[0].health/health(state->map->objects[0].radius);
		double extra_hp_rate_self=(state->status->objects[0].health+possible_extra_hp)/health(state->map->objects[0].radius);
		double distance=Distance(state->status->objects[0].pos,state->boss->boss.pos);

		//setting game over state
		gameover_state=0;
		if ((original_hp_rate<=death_rate&&distance<state->boss->boss.radius)||(state->status->objects[0].health<0.25*state->status->objects[0].max_health))
			gameover_state=-1;
		if ((original_hp_rate>=kill_rate&&distance<state->status->objects[0].radius)||(state->boss->boss.radius<1))
			gameover_state=1;

		//evaluating
		return state->status->objects[0].health+
			rate1*(extra_ability+state->status->objects[0].ability)+
			rate2*food_density+
			rate3*state->map->time+
			rate4*(state->status->objects[0].vision-distance)/state->boss->boss.radius
			+9999*gameover_state;
	}
	else																					  //Both enemy and boss in sight
	{
		//initializing data
		int possible_extra_hp=0,hp_price=pow(2,state->status->objects[0].skill_level[SkillType(HEALTH_UP)]);
		int temp_ability=state->status->objects[0].ability;
		double kill_rate=pow(1.2,3),death_rate=1/kill_rate;
		while (temp_ability>hp_price)
		{
			possible_extra_hp+=500;
			temp_ability-=hp_price;
			hp_price=2*hp_price;
		}
		double Enemy_original_hp_rate=state->status->objects[0].health/health(state->enemy->player.radius);
		double Enemy_extra_hp_rate_self=(state->status->objects[0].health+possible_extra_hp)/health(state->enemy->player.radius);
		double Enemy_distance=Distance(state->status->objects[0].pos,state->enemy->player.pos);
		double Boss_original_hp_rate=state->status->objects[0].health/health(state->boss->boss.radius);
		double Boss_extra_hp_rate_self=(state->status->objects[0].health+possible_extra_hp)/health(state->boss->boss.radius);
		double Boss_distance=Distance(state->status->objects[0].pos,state->boss->boss.pos);

		//setting game over state
		gameover_state=0;
		if ((Boss_original_hp_rate<=death_rate&&Boss_distance<state->boss->boss.radius)||
			(state->status->objects[0].health<0.25*state->status->objects[0].max_health)||
			(Enemy_original_hp_rate<=death_rate&&Enemy_distance<state->enemy->player.radius))
			gameover_state=-1;
		if ((Boss_original_hp_rate>=kill_rate&&Boss_distance<state->status->objects[0].radius)||
			(health(state->enemy->player.radius)<0.25*(health(state->enemy->maxr)))||
			(Enemy_original_hp_rate>=kill_rate&&Enemy_distance<state->status->objects[0].radius)||
			(health(state->boss->boss.radius)<1))
			gameover_state=1;

		//evaluating
		double enemy_evaluation_point;
		if (Enemy_original_hp_rate>1) //Add enemy's ability points into consideration!
		{
			//rate5: shield status, rate6: dash status
			enemy_evaluation_point=	
				Enemy_original_hp_rate*state->status->objects[0].health+
				rate1*((extra_ability+state->status->objects[0].ability)-3/* 3 should be ability points of enemy, do we have that?*/)+
				rate4*Enemy_distance+
				rate5*(state->status->objects[0].shield_time>0)+
				rate6*(state->status->objects[0].dash_time>0)*(state->status->objects[0].skill_level[SkillType(DASH)])
				+9999*gameover_state;
		}
		else
		{
			//rate5: shield status, rate6: dash status
			enemy_evaluation_point=
				Enemy_original_hp_rate*state->status->objects[0].health+
				rate4*Enemy_distance+
				rate5*(state->status->objects[0].shield_time>0)+
				rate6*(state->status->objects[0].dash_time>0)*(state->status->objects[0].skill_level[SkillType(DASH)])
				+9999*gameover_state;
		}
		return 
			(rate_boss)*
			(state->status->objects[0].health+
			rate1*(extra_ability+state->status->objects[0].ability)+
			rate2*food_density+
			rate3*state->map->time+
			rate_boss_distance*(state->status->objects[0].vision-Boss_distance)/state->boss->boss.radius)+
			(rate_enemy)*enemy_evaluation_point
			+9999*gameover_state;
	}
}

SearchTree::SearchTree(const SceneState* rootstate, int DFSdepth) {
	// init
	lnNode* HeapList;
	MaxHeap* newheap = new MaxHeap;
	this->root = new SearchNode(rootstate, NULL, NULL);
	this->DFSdepth = DFSdepth;

	HeapList=(lnNode*)malloc(sizeof(lnNode));
	HeapList->next=NULL;
	newheap->additem(root);
	HeapList->dataptr=newheap;
	this->HeapsAtDepth = HeapList;
	HeapList=(lnNode*)malloc(sizeof(lnNode));
	HeapList->next=NULL;
	HeapList->dataptr = new MaxHeap;
	this->EndedNodes = HeapList;
	std::cout<<"search tree finished"<<std::endl;
}

MaxHeap* SearchTree::GetHeap(int depth) {
	int count=0;
	lnNode* ptr=this->HeapsAtDepth;
	while(ptr->next) {
		count++;
		ptr = ptr->next;
		if (count==depth)
			break;
	}
	if (count!=depth && depth != -1)
		return NULL;
	else
		return (MaxHeap*)ptr->dataptr;
}

SearchNode* SearchTree::GetBestNode(int depth) {
	MaxHeap* heap = this->GetHeap(depth);
	return *((SearchNode**)(heap->getmax()));
}

lnNode* SearchTree::GetAllNodes(int depth) {
	MaxHeap* heap = this->GetHeap(depth);
	return (lnNode*)(heap->returnall());
}

bool SearchTree::BFS(int BFSdepth) {
	lnNode* ptr;
	int count;
	int result=0;
	for(count=1;count<=BFSdepth;count++){
		ptr = this->GetAllNodes(count-1);
		result+=search_layer_exec(ptr);
	}
	return result;
}

bool SearchTree::DFS(int width) {
	MaxHeap* heap;
	int depth=this->DFSdepth;
	void* temp;
	lnNode* ptr;
	int result=0;
	int count;
	while(depth <= this->DFSdepth) {
		heap = this->GetHeap(-1);
		temp = heap->getmax(width); // temp -s SearchNode**
		if (width==1) {
			result += this->search_layer_exec(*(SearchNode**)temp);
			if (result)
				break;
			else
				depth = (*(SearchNode**)temp)->depth + 1;
		} else {
			for(count=0;count<=width-1;count++)
				result += this->search_layer_exec((*(SearchNode**)temp+count));
			if (result) {
				int cancontinue=false;
				ptr = this->GetAllNodes();
				while(ptr) {
					cancontinue = cancontinue || (((SearchNode*)(ptr->dataptr))->haschildren());
					ptr = ptr->next;
				}
				if (!cancontinue)
					break;
			depth = (*(SearchNode**)temp)->depth + 1;
			}
		}
	}
	return result;
}

bool SearchTree::search_layer_exec(const lnNode* layerNodes) {
	lnNode* ptr = (lnNode*)layerNodes;
	lnNode* child;
	MaxHeap* heap=NULL;
	int depth;
	bool result=false;
	SearchNode* temp=(SearchNode*)ptr->dataptr;
	if (temp)
		depth = temp->depth + 1;
	else
		return false;
	while(ptr) {
		temp = (SearchNode*)(ptr->dataptr);
		temp->span();
		child = temp->rankchildren();
		while(child) {
			if (heap==NULL)
				heap = this->GetHeap(depth);
			if (heap==NULL)
			{
				lnNode* heapNode=this->HeapsAtDepth;
				lnNode* newNode=(lnNode*)malloc(sizeof(lnNode));
				while(heapNode->next)
					heapNode = heapNode->next;
				heapNode->next = newNode;
				newNode->next=NULL;
				newNode->dataptr = new MaxHeap;
				heap=(MaxHeap*)newNode->dataptr;
			}
			heap->additem((SearchNode*)child->dataptr);
			if (!((SearchNode*)(child->dataptr))->haschildren()) {
				lnNode* linkedlist = (lnNode*)malloc(sizeof(lnNode));
				linkedlist->next = this->EndedNodes;
				linkedlist->dataptr = child->dataptr;
				result = true;
				this->EndedNodes = linkedlist;
			}
			child = child->next;
		}
		ptr = ptr->next;
	}
	return result;
}

bool SearchTree::search_layer_exec(const SearchNode* node) {
	int depth;
	MaxHeap* heap=NULL;
	SearchNode* temp=(SearchNode*)node;
	lnNode* child;
	bool result=false;
	if (temp)
		depth = temp->depth + 1;
	else
		return false;
	temp->span();
	child = temp->rankchildren();
	while(child) {
		if (heap==NULL)
			heap = this->GetHeap(depth);
		if (heap==NULL)
			{
				lnNode* heapNode=this->HeapsAtDepth;
				lnNode* newNode=(lnNode*)malloc(sizeof(lnNode));
				while(heapNode->next)
					heapNode = heapNode->next;
				heapNode->next = newNode;
				newNode->next=NULL;
				newNode->dataptr = new MaxHeap;
				heap=(MaxHeap*)newNode->dataptr;
			}
		heap->additem((SearchNode*)child->dataptr);
		if (!((SearchNode*)(child->dataptr))->haschildren()) {
			lnNode* linkedlist = (lnNode*)malloc(sizeof(lnNode));
			linkedlist->next = this->EndedNodes;
			linkedlist->dataptr = child->dataptr;
			result = true;
			this->EndedNodes = linkedlist;
		}
		child = child->next;
	}
	return result;
}

MaxHeap* SearchTree::extendheap() {
	lnNode* heapNode=this->HeapsAtDepth;
	lnNode* newNode=(lnNode*)malloc(sizeof(lnNode));
	while(heapNode->next)
		heapNode = heapNode->next;
	heapNode->next = newNode;
	newNode->next=NULL;
	newNode->dataptr = new MaxHeap;
	return (MaxHeap*)newNode->dataptr;
}

SearchTree::~SearchTree() {

}








