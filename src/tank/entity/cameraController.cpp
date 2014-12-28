#include <math.h>
#include "cameraController.h"
#include "../../engine/graphics/graphics.h"
#include "../../engine/common/timer.h"
//#include "../../engine/input/inputData.h"
//#include "../main.h"

CCameraController::CCameraController()
{
	mCamera = NULL;
	mFocusObject = NULL;
	mTargetDistance = 1.0f;
	mTargetAngle = 0.0f;
	mCameraPosition = SVector3(0.0f, 0.0f, 0.0f);
	mCameraVelocity = SVector3(0.0f, 0.0f, 0.0f);

	CreatePhysical(gWorld);
	SSphere sphere;
	sphere.position = SVector3(0.0f, 0.0f, 0.0f);
	sphere.radius = 1.0f;

	SBoundingBox box;
	box.max = SVector3(1.0f, 1.0f, 1.0f);
	box.min = SVector3(-1.0f, -1.0f, -1.0f);

	CPObject* obj = GetPhysicalObject();
	//obj->SetPosition(SVector3(50.0f, 0.0f, 50.0f));
	obj->SetPosition(mCameraPosition);
	obj->SetFriction(15.0f);
	obj->SetObjectType(0);
	obj->SetBoundingBox(box);
	obj->SetSphere(sphere);
	obj->SetCollisionMask(2);
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
	if (mFocusObject)
	{
		mCameraPosition = o->GetPhysicalObject()->GetPosition();
		float angle = o->GetRot();
		mCameraPosition.x += mTargetDistance * cos(angle) * cos(mTargetAngle);
		mCameraPosition.y += mTargetDistance * sin(mTargetAngle);
		mCameraPosition.z += mTargetDistance * sin(angle) * cos(mTargetAngle);
	}
}

void CCameraController::SetViewDistance(float dist)
{
	mTargetDistance = dist;
}

void CCameraController::SetViewAngle(float ang)
{
	mTargetAngle = ang;
}


void CCameraController::Update( float dt )
{
	if (mCamera)
	{
		if ( mFocusObject && mFocusObject->GetPhysicalObject() )
		{
			mCameraPosition = mObject->GetPosition();

			SVector3 position = mFocusObject->GetPhysicalObject()->GetPosition();
			SVector3 target = position;
			float angle = mFocusObject->GetRot();
			target.x += mTargetDistance * cos(angle) * cos(mTargetAngle);
			target.y += mTargetDistance * sin(mTargetAngle);
			target.z += mTargetDistance * sin(angle) * cos(mTargetAngle);

			position.x += 5.0f * -cos(angle);
			position.z += 5.0f * -sin(angle);

			SVector3 dist = target - mCameraPosition;
			//mCameraVelocity = mCameraVelocity*(1.0f-dt*5.0f) + dist * dt * 50.25f;
			mCameraVelocity = dist * 10.0f;
			//mCameraVelocity = dist * 1.0f;
			mObject->SetVelocity(mCameraVelocity);
			//mObject->ApplyForce(mCameraVelocity);

			if (dist.LengthSquared() > (mTargetDistance*3.0f*mTargetDistance))
			{
				mCameraPosition = target;
				mObject->SetPosition(mCameraPosition);
			}
			//mCameraPosition = mCameraPosition + mCameraVelocity * dt;

			mCamera->SetPosition(mCameraPosition.x, mCameraPosition.y, mCameraPosition.z);
			mCamera->LookAt(position.x, position.y, position.z);
			mDeathTimer.Step();
		} else {
			if (mDeathTimer.Time() > 5.0f)	// allows you to see yourself explode before going overview
			{
				mCamera->SetPosition(40.0f, 30.0f, 43.0f);
				mCamera->LookAt(50.0f, 0.0f, 50.0f);
			}
		}
	}
}
