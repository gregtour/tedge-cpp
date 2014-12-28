#ifndef _PICKUP_H
#define _PICKUP_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/physicalObj.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/particle.h"

#define PICKUP_OBJ_TYPE		10

#define KEY_PICKUP_TYPE		1
#define HEALTH_PICKUP_TYPE	2
#define MAGIC_PICKUP_TYPE	3
#define GEM_PICKUP_TYPE		4

class CPickup: public CEntity, public IPhysicalObj, public IGraphicalObj
{
public:
	CPickup( int type );
	virtual ~CPickup();

	virtual void Update( float dt );
	virtual void RenderAlpha();

	int GetType();
	void PickUp();

private:
	CTexture* mTexture;
	CTexture* mMask;
	CParticleEmitter* mEmitter;
	float mRot;
	int mType;
};

#endif
