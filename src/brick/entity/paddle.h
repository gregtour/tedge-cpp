#ifndef _PADDLE_H
#define _PADDLE_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/entity/physicalObj.h"
#include "../../engine/entity/inputtingObj.h"

class CTexture;
class CBall;
class CPaddle: public CEntity, public IGraphicalObj, public IPhysicalObj, public IInputtingObj
{
public:
	CPaddle(int player);
	~CPaddle();

	void Update( float );
	void RenderOrtho();

protected:
	int mPlayer;
	CTexture* mTexture;
	CModel* mModel;
	CLinkedList<CBall> mBalls;
};

#endif
