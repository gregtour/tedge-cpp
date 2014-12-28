#include "platform.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/model.h"
#include "../game/game.h"
#include <cmath>

CPlatform::CPlatform()
{
	mTexture = CManagedTexture::Load(&gResourceManager, "brick.bmp");
	mColor = HSV((rand()%12)*30, 1.0f, 1.0f);
}

CPlatform::~CPlatform()
{

}

void CPlatform::Create(SVector3 center, float radius)
{
	mCenter = center;
	mRadius = radius;

	if (mRadius == 0.0f)
		mModel = CManagedModel::Load(&gResourceManager, "spin.ms3d");
	else
		mModel = CManagedModel::Load(&gResourceManager, "platform.ms3d");


	CreatePhysical(gWorld, mModel);

	mObject->SetPosition( center );
	mObject->SetObjectType(PLATFORM_OBJ_TYPE);
	mObject->SetCollisionMask( 1 );
	mObject->SetFriction( 0.0f );
}

void CPlatform::Update( float dt )
{
	if (mRadius == 0.0f)
	{
		mTheta += 0.3f * dt;
		((CPMesh*)mObject)->mRot = mTheta;
	}
	else
	{
		mTheta += 0.5f * dt;
		SVector3 pos = mCenter + SVector3(sin(mTheta) * mRadius, cos(mTheta) * mRadius, 0.0f);
		mObject->ApplyForce((pos - mObject->GetPosition()) * 10.0f - mObject->GetVelocity() );
	}
	
}

void CPlatform::Render()
{
	if (mObject)
	{
		SVector3 pos = mObject->GetPosition();
		{
			float x, y, z;
			gCamera->GetPosition( &x, &y, &z );
			if ( (x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y) + (z-pos.z)*(z-pos.z) > VIEW_DISTANCE_SQUARED )
				return;
		}

		glTranslatef( pos.x, pos.y, pos.z );
		if (mRadius == 0.0f)
			glRotatef(mTheta * DEGREES, 0.0f, 0.0f, 1.0f );

		glColor3f(mColor.x, mColor.y, mColor.z);
		mTexture->Bind();
		mModel->Render();
		mTexture->Unbind();
		glColor3f(1.0f, 1.0f, 1.0f);
	}
}
