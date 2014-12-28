#ifndef _CAMERACONTROLLER_H
#define _CAMERACONTROLLER_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/camera.h"
#include "../../engine/entity/physicalObj.h"
#include "../../engine/common/timer.h"

#ifndef NULL
#	define	NULL 0L
#endif

class CCameraController: public CEntity, public IPhysicalObj
{
public:
	CCameraController();
	~CCameraController();

	void ControlCamera( CCamera* c );
	void FocusOnObject( IPhysicalObj* o );
	void SetViewDistance( float dist);
	void SetViewAngle( float ang );
	void Update( float dt );

private:
	CCamera*		mCamera;
	IPhysicalObj*	mFocusObject;
	
	float mTargetDistance, mTargetAngle;
	
	CTimer mDeathTimer;
	SVector3 mCameraPosition;
	SVector3 mCameraVelocity;
};



#endif
