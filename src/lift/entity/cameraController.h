#ifndef _CAMERACONTROLLER_H
#define _CAMERACONTROLLER_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/camera.h"
#include "../../engine/entity/physicalObj.h"

#ifndef NULL
#	define	NULL 0L
#endif

extern int gCameraMode;

#define VIEWING_ANGLE	 (16.0f/15.0f)
#define CAMERA_DISTANCE	 (3.0f)
#define VIEWING_ANGLE2	 (16.0f/15.0f)
#define CAMERA_DISTANCE2 (5.0f)
#define LOOK_AHEAD_DISTANCE	(6.0f)
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

private:
	CCamera* mCamera;
	IPhysicalObj* mFocusObject;
	bool mAllowMovement;
	float mMinDistance, mMaxDistance;
	float mAngle, mRotSp;
	float mDistance, mDistanceSp;
};



#endif
