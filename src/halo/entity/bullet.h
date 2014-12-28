#ifndef _BULLET_H
#define _BULLET_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/entity/physicalObj.h"

#define BULLET_SPEED		30.0f
//#define BULLET_SPEED		5000.0f
#define BULLET_LIFE			0.66f

class CModel;
class CTexture;
class CBullet: public CEntity, public IGraphicalObj, public IPhysicalObj
{
public:
	CBullet(SVector3 color);
	~CBullet();

	void Render();
	void Update(float dt);
	void Explode();

protected:
	SVector3 mColor;
	CModel* mModel;
    CTexture* mTexture;
	float mLife;
};

#endif