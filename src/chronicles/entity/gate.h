#ifndef _GATE_H
#define _GATE_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/entity/physicalObj.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/engine.h"

#define GATE_OBJ_TYPE	13

class CGate: public CEntity, public IPhysicalObj, public IGraphicalObj
{
public:
	CGate();
	~CGate();

	void CreatePhysical( CPhysics* world, SVector3* p1, SVector3* p2 );
	void Update( float dt );
	void RenderAlpha();
	void Open();
	bool IsOpen();

private:
	CTexture* mTexture;
	CTexture* mMask;
	int mOpen;
	bool mOpening;
	CPTriangle*	mTri1;
	CPTriangle*	mTri2;
	SVector3	p1;
	SVector3	p2;
};

#endif
