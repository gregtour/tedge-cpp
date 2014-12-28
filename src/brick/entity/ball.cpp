#include <math.h>
#include "ball.h"
#include "../../engine/graphics/model.h"
#include "../game/game.h"
#include "../../engine/physics/svector.h"

const char* BALL_MODEL_FILE	= "ball.ms3d";


CBall::CBall(SVector3 color, SVector3 pos, SVector3 vel)
{
	mModel = CManagedModel::Load( &gResourceManager, BALL_MODEL_FILE );
	mColor = color;


	SSphere ballsphere;
	ballsphere.position = SVector3(0.5f, 0.5f, 0.5f);
	ballsphere.radius = 1.0f;

	SBoundingBox ballbox;
	ballbox.max = SVector3(0.0f, 0.0f, 0.0f);
	ballbox.min = SVector3(1.0f, 1.0f, 1.0f);

	CreatePhysical(gWorld);
	CPObject* obj = GetPhysicalObject();
	obj->SetPosition(pos);
	obj->SetVelocity(vel);
	obj->SetObjectType(BALL_OBJECT_TYPE);
	obj->SetFriction(0.0f);
	obj->SetBoundingBox(ballbox);
	obj->SetSphere(ballsphere);
	obj->SetCollisionMask(1);
}

CBall::~CBall()
{
}

void CBall::Update(float dt)
{
	if (mObject)
	{
		SVector3 pos = mObject->GetPosition();
		SVector3 vel = mObject->GetVelocity();

		if (pos.x < 0.0f || pos.x > 40.0f)
		{
			vel.x = -vel.x;
			mObject->SetVelocity(vel);
		}

		if (pos.y < 0.0f)
		{
			vel.y = -vel.y;
			mObject->SetVelocity(vel);
		}
		else if (pos.y > 30.0f)
		{
			mWorld->Remove(mObject);
			delete mObject;
			mObject = NULL;
		}
	}
}

void CBall::Render()
{
	if (mObject)
	{
		SVector3 pos = mObject->GetPosition();
		glTranslatef( pos.x, pos.y, pos.z );
		glColor3f(mColor.x, mColor.y, mColor.z);
		mModel->Render();
	}
}

