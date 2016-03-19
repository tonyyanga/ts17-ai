#include "../headers/common.h"
#include "../headers/teamstyle17.h"

const Map *map=GetMap();
const Status *Self=GetStatus();


int count_type(ObjectType A)
{
	int count,i;
	for(i=0;i<map->objects_number;i++)
	{
		if (map->objects[i].type==A)
			count++;
	}
	return(count);
}

Position* pos(ObjectType A)
{
	int i;
	Position* Pos;
	Pos=new Position[count_type(A)];	//给Pos分配A数量的内存
	for(i=0;i<map->objects_number;i++)
	{
		if (map->objects[i].type==A)
			Pos[i]=map->objects[i].pos;
	}
	return(Pos);
}

Position cloest(ObjectType A)
{
	int cloest=0,i;
	Position* posA=pos(A);
	Position p0=Self->objects[0].pos;
	for(i=1;i<count_type(A);i++)
	{
		if (Distance(p0,posA[i])<Distance(p0,posA[cloest]))
			cloest=i;
	}
	return(posA[cloest]);
}

Position* dev_inway(Speed A)
{
	double distance;	//刺球到speed线段的距离
	int i;
	double r=Self->objects[0].radius;
	Position* posd=pos(DEVOUR);
	Position linepoint=Displacement(Self->objects[0].pos,A);
	for(i=0;i<count_type(DEVOUR);i++)
	{
		distance=PointLineDistance(posd[i],Self->objects[0].pos,linepoint);
		if (distance<r) return(&posd[i]);
	}
	return(0);
}

		





