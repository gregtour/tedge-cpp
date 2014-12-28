#ifndef _CAMERACONTROLLER_H
#define _CAMERACONTROLLER_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/camera.h"
#include "../../engine/entity/physicalObj.h"
#include "../../engine/common/timer.h"

#ifndef NULL
#	define	NULL 0L
#endif

extern int gCameraMode;

/*
#define VIEWING_ANGLE	 (12.0f/15.0f)
#define CAMERA_DISTANCE	 (15.0f)
#define VIEWING_ANGLE2	 (15.0f/15.0f)
#define CAMERA_DISTANCE2 (10.0f)
#define LOOK_AHEAD_DISTANCE	(6.0f)*/
#define VIEWING_ANGLE	 (15.0f/5.0f)
#define CAMERA_DISTANCE	 (0.5f)

#define LOOK_AHEAD_DISTANCE	(11.0f)
//#define CAMERA_NO_ROTATE

class CCameraController: public CEntity
{
public:
	CCameraController();
	~CCameraController();

	void ControlCamera( CCamera* c );
	void FocusOnObject( IPhysicalObj* o );
	void SetViewDistance( float min, float max );
	void SetViewAngle( float a );
	void SetAnchored( bool a );

	void Update( float dt );

	void EnableWaypoints();
	void DisableWaypoints();
	void AddWaypoint(SVector3	pos);

private:
	CCamera* mCamera;
	IPhysicalObj* mFocusObject;
	bool mAllowMovement;
	float mMinDistance, mMaxDistance;
	float mAngle, mRotSp;
	float mDistance, mDistanceSp;
	CTimer					mTime;
	SVector3				mPos;
	SVector3				mVelocity;
	bool					mUseWaypoints;
	CListEntry<SVector3>*	mCurrentWaypoint;
	CLinkedList<SVector3>	mWaypoints;
};



#endif
