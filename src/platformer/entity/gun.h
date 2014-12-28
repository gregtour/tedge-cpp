#ifndef _GUN_H
#define _GUN_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/physics/physics.h"
#include "../../engine/physics/common.h"
#include "../../engine/common/list.h"

class CModel;
class CGun: public CEntity, public IGraphicalObj
{
public:
	CGun();

	void SetPosition(SVector3 pos);

	void Update( float );
	void Render();

	float					mTime;
protected:
	CModel*					mModel;
	SVector3				mPos;
	CLinkedList<CPObject>	mShot;
};

#endif
