#include <math.h>
#include "cameraController.h"
#include "../../engine/graphics/graphics.h"
#include "../../engine/input/inputData.h"
#include "../main.h"

int gCameraMode = 0;

CCameraController::CCameraController()
{
	mUseWaypoints = false;
	mCurrentWaypoint = NULL;
	mVelocity = SVector3(0.0f, 0.0f, 0.0f);

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
	CListEntry<SVector3>* point = mWaypoints.GetFirst();
	while (point)
	{
		delete point->data;
		mWaypoints.RemoveEntry(point);
		point = mWaypoints.GetFirst();
	}
}

void CCameraController::EnableWaypoints()
{
	mUseWaypoints = true;
}

void CCameraController::DisableWaypoints()
{
	mUseWaypoints = false;
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
		float objectAngle = mFocusObject->GetRot() + PI;
		float objx = objPosition.x;
		float objz = objPosition.z;

		if ( mAllowMovement )
		{

			//	Track Camera Movement
			float cx, cy, cz;
			mCamera->GetPosition( &cx, &cy, &cz );
			
			mAngle += mRotSp * dt;
			// cameraAngle = gPlayer->GetMouse().mMouseX / 200.0f;
			// mAngle = gPlayer->GetMouse().mMouseY * 1.0f / 300.0f;
			
			
			// Above view!	//
//				cameraAngle = objectAngle;

			mDistance = VIEWING_ANGLE * CAMERA_DISTANCE;

			// ------------ //

		
			float angleOffset = ( objectAngle - mAngle );

			while ( angleOffset > 2*PI )
				angleOffset -= 2*PI;

			while ( angleOffset < 0.0f )
				angleOffset += (2*PI);

			if ( angleOffset > PI )
				angleOffset = angleOffset - 2*PI;
			
			mRotSp = angleOffset / 0.155f;
		

#ifdef CAMERA_NO_ROTATE
			mAngle = 0.0f;
#endif

			// Set Camera Position
			cx = (cos( mAngle ) * mDistance) + objx;
			cz = (sin( mAngle ) * mDistance) + objz;

			cy = CAMERA_DISTANCE / VIEWING_ANGLE + objPosition.y + 4.0f; // cos( mAngle ) * mDistance;


			mCamera->SetPosition( cx, cy, cz );

			/*
			// Set Speed for Next Iteration
			float targetDistanceMove = 0.0f;

			if ( mDistance > mMaxDistance )
				targetDistanceMove = mMaxDistance - mDistance;
			else if ( mDistance < mMinDistance )
				targetDistanceMove = mMinDistance - mDistance;

			mDistanceSp = targetDistanceMove * 5.0f;
			*/
		}

		objx -= cos( mAngle ) * LOOK_AHEAD_DISTANCE;
		objz -= sin( mAngle ) * LOOK_AHEAD_DISTANCE;
		mCamera->LookAt( objx , objPosition.y + 1.0f, objz );
	} else if (mUseWaypoints && mWaypoints.length > 2) {
		
		if (mCurrentWaypoint == NULL)
		{
			mPos = *mWaypoints.GetFirst()->data;
			mCurrentWaypoint = mWaypoints.GetFirst()->GetNext();
			mTime.Step();
		} else {

			SVector3 last = *mCurrentWaypoint->GetPrev()->data;
			SVector3 next = *mCurrentWaypoint->data;
			SVector3 target = (next - last) * (mTime.Time() / 8.0f) + last;
			mVelocity = (target - mPos);

			mPos = mPos + mVelocity * dt;
			SVector3 inFront = mPos + mVelocity;
		
			if (mTime.Time() > 8.0f)
			{
				mCurrentWaypoint = mCurrentWaypoint->GetNext();
				mTime.Step();
			}
	
			mCamera->SetPosition(target.x, target.y, target.z);
			mCamera->LookAt(inFront.x, inFront.y, inFront.z);
	//		mCamera->SetPosition(mPos.x, mPos.y, mPos.z);
			//mCamera->LookAt(mPos.x+1.0f, mPos.y, mPos.z+1.0f);
		}
	}
}
void CCameraController::AddWaypoint(SVector3	pos)
{
	if (mUseWaypoints)
	{
		SVector3* point = new SVector3(pos.x, pos.y, pos.z);
		mWaypoints.Add(point);
	}
}
