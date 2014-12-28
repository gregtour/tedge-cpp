#ifndef _BRICK_H
#define _BRICK_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/entity/physicalObj.h"

enum BRICKTYPE
{
	NORMAL,
	WIDE_PADDLE,
	MULTI_BALL,
	EXPLOSION
};


class CModel;
class CBrick: public CEntity, public IGraphicalObj, public IPhysicalObj
{
public:
	CBrick(SVector3 pos, SVector3 color, int type = NORMAL);
	~CBrick();

	void RenderOrtho();
	void Update(float dt);

protected:
	SVector3 mColor;
	int	mType;
	CModel* mModel;
};

#endif
