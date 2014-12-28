#ifndef _BULLET_H
#define _BULLET_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/entity/physicalObj.h"

#define PROJECTILE_SPEED		100.0f
#define PROJECTILE_LIFE			0.66f

class CModel;
class CProjectile: public CEntity, public IGraphicalObj, public IPhysicalObj
{
public:
	CProjectile(SVector3 color);
	~CProjectile();

	void Render();
	void Update(float dt);
	void Explode();

protected:
	SVector3 mColor;
	CModel* mModel;
	float mLife;
};

#endif