#ifndef _COLLISION_H
#define _COLLISION_H

#ifndef NULL
#define	NULL	0L
#endif

#include "svector.h"

class IPhysicalObj;
struct CCollision
{
	IPhysicalObj* object1;
	IPhysicalObj* object2;
	int		type1;
	int		type2;
	SVector3 contactPoint;
	SVector3 normal;

	CCollision()
	{
		object1 = NULL;
		object2 = NULL;
		type1 = 0;
		type2 = 0;
		normal = SVector3(0.0f, 0.0f, 0.0f);
	};

	CCollision(IPhysicalObj* o1, IPhysicalObj* o2, int t1, int t2, SVector3 contact, SVector3 n)
	{
		object1 = o1;
		object2 = o2;
		type1 = t1;
		type2 = t2;
		contactPoint = contact;
		normal = n;
	}

	bool Contains(int t1, int t2)
	{
		return ((t1 == type1 && t2 == type2) || (t1 == type2 && t2 == type1));
	}

	bool Contains(int t1)
	{
		return (t1 == type1 || t1 == type2);
	}
	
	IPhysicalObj* Get(int type)
	{
		if (type == type1)
			return object1;
		else if (type == type2)
			return object2;
		else
			return NULL;
	}
};

#endif
