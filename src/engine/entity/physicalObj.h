#ifndef _PHYSICALOBJ_H
#define _PHYSICALOBJ_H

#include "../physics/physics.h"

#ifndef NULL
#	define NULL 0L
#endif

class CModel;
class IPhysicalObj
{
public:
	IPhysicalObj();
	virtual ~IPhysicalObj();

	void CreatePhysical( CPhysics* world );
	void SetWorld( CPhysics* world );

	void CreatePhysical(CPhysics* world, CModel* mesh);
	
	CPObject* GetPhysicalObject();
	float GetRot();
	void SetRot( float r );

protected:
	CPhysics*	mWorld;
	CPObject*	mObject;
	float mRot;
};

#endif
