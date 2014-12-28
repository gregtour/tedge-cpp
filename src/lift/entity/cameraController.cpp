#include <math.h>
#include "cameraController.h"
#include "../../engine/graphics/graphics.h"
#include "../../engine/input/inputData.h"
#include "../main.h"

#define DIST	8.0f

int gCameraMode = 0;

CCameraController::CCameraController()
{
	mCamera = NULL;
	mFocusObject = NULL;
	mDistanceSp = 0.0f;
	mRotSp = 0.0f;
	mAngle = 0.0f;
	SetViewDistance( 0.0f, 0.0f );
	SetViewAngle( 0.0f );
	SetAnchored( false );
}

CCameraController::~CCameraController()
{

}

void CCameraController::ControlCamera( CCamera* c )
{
	mCamera = c;
}

void CCameraController::FocusOnObject( IPhysicalObj* o )
{
	mFocusObject = o;
}

void CCameraController::SetViewDistance( float min, float max )
{
	mMinDistance = min;
	mMaxDistance = max;
}

void CCameraController::SetViewAngle( float a )
{
	mAngle = a;
}

void CCameraController::SetAnchored( bool a )
{
	mAllowMovement = !a;
}

void CCameraController::Update( float dt )
{
	
	if ( mFocusObject && (gPlayer) )
	{

		CPObject* obj = mFocusObject->GetPhysicalObject();
		SVector3 objPosition = obj->GetPosition();
		float rot = mFocusObject->GetRot();
		float xrot = gPlayer->GetUpRot();

		if ( mAllowMovement )
		{
			
			mCamera->LookAt( objPosition.x, objPosition.y, objPosition.z );
			mCamera->SetPosition( objPosition.x + cos( rot )*DIST,
				objPosition.y + sin( rot )*sin( xrot)*DIST, 
				objPosition.z + sin( rot ) * cos(xrot)*DIST );

			SVector3 up = objPosition;
			up = up * -1.0f;
			up.Normalize();
			mCamera->SetUp(up.x, up.y, up.z);
		}

/*

		objx -= cos( mAngle ) * LOOK_AHEAD_DISTANCE;
		objz -= sin( mAngle ) * LOOK_AHEAD_DISTANCE;
		mCamera->LookAt( objx , objPosition.y, objz );
*/
	}
}
