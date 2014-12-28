#ifndef _PICKUP_H
#define _PICKUP_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/loggableObj.h"
#include "../../engine/entity/physicalObj.h"
#include "../../engine/entity/graphicalObj.h"

#define PICKUP_XROT_SPEED	1.0f
#define PICKUP_ZROT_SPEED	2.0f

enum
{
	TYPE_HEALTH = 1,
	TYPE_AMMO
};

class CWaveSound;
class CModel;
class CTexture;
class CRacer;
class CPickup: public CEntity, public IGraphicalObj, public IPhysicalObj
{
public:
	virtual std::string	ClassName() {return "pickup";}

	CPickup();
	virtual ~CPickup();

	virtual void Update( float dt );
	virtual void Render();

	void Spawn();
	void PickedUp(CRacer* by);

protected:
	int mType;
	CWaveSound* mPickupSound;
	CModel*		mModel;
	CTexture*	mTexture;
};

#endif
