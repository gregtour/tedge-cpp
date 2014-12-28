#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/entity/physicalObj.h"

class CModel;
class CTexture;
class CPlatform: public CEntity, public IGraphicalObj, public IPhysicalObj
{
public:
	CPlatform();
	~CPlatform();

	//void Create(const char* model, CTexture* t, SVector3 center, float radius);
	void Create(SVector3 center, float radius);

	float mTheta;

	void Update( float );
	void Render();

protected:
	SVector3 mCenter;
	float mRadius;
	CTexture* mTexture;
	CModel*	mModel;
	SVector3 mColor;
};

#endif
