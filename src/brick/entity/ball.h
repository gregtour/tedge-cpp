#ifndef _BALL_H
#define _BALL_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/entity/physicalObj.h"

#define BALL_SPEED		10.0f

class CModel;
class CBall: public CEntity, public IGraphicalObj, public IPhysicalObj
{
public:
	CBall(SVector3 color, SVector3 pos, SVector3 vel);
	~CBall();

	void Render();
	void Update(float dt);

protected:
	SVector3 mColor;
	CModel* mModel;
};

#endif
