#include <math.h>
#include "tour.h"


CTourCam::CTourCam(CCameraController* c, IPhysicalObj* n)
{
	cam = c;
	next = n;
}

CTourCam::~CTourCam()
{
}

void CTourCam::Spawn()
{
	SVector3 gravity(0.0f, 0.0f, 0.0f);
	
	SSphere tanksphere;
	tanksphere.position = SVector3(0.0f, 1.5f, 0.0f);
	tanksphere.radius = 12.0f;

	SBoundingBox tankbox;
	tankbox.max = SVector3(12.2f, 12.2f, 12.2f);
	tankbox.min = SVector3(-12.2f, -12.2f, 12.2f);

	CreatePhysical(gWorld);
	CPObject* obj = GetPhysicalObject();
	//obj->SetPosition(SVector3(50.0f, 0.0f, 50.0f));
	obj->SetPosition(SVector3(250.0f, 100.0f, 250.0f));
	obj->SetObjectType(0);
	obj->SetDefaultForce(gravity);
	obj->SetFriction(0.75f);
	obj->SetBoundingBox(tankbox);
	obj->SetSphere(tanksphere);
	obj->SetCollisionMask(2);
}

void CTourCam::Update( float dt )
{
	if (mPulse < 30.0f)
	{
		mPulse += dt;
		if (mPulse > 30.0f)
		{
			SetWorld( NULL );
			delete mObject;
			mObject = NULL;
			cam->FocusOnObject(next);
		}
		else
		{
			mRot += (cos(mPulse*0.1f)*0.5f - 0.05f)*dt;
			SVector3 force;
			force.x = -cos(mRot) * 10.0f * dt * (sin(mPulse) + 1.0f);
			force.y = sin(mPulse*0.25f) * 2.0f * dt;
			force.z = -sin(mRot) * 10.0f * dt * (sin(mPulse) + 1.0f);
			mObject->ApplyForce(force);
		}

	}
}
