#ifndef _PLAYER_H
#define _PLAYER_H

#include "../../engine/common/timer.h"
#include "../../engine/entity/entity.h"
#include "../../engine/entity/loggableObj.h"
#include "../../engine/entity/physicalObj.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/entity/inputtingObj.h"

class CModel;
class CPlayer: public CEntity, public IPhysicalObj, public IGraphicalObj, public IInputtingObj
{
public:
	CPlayer();
	~CPlayer();

	void Update( float );
	void Render();

	void TurnRight(float dt);
	void TurnLeft(float dt);
	void MoveForward(float dt);
	void MoveBackward(float dt);
	void Jump();

	void SetSafe();
	void SetGrounded(bool grounded);
	void ResetPosition();

protected:
	bool		mHasJumped;
	bool		mGrounded;
	SVector3	mSafePos;
	CModel*		mModel;
};

#endif
