#include "gun.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/model.h"
#include "../../engine/graphics/buffer/I3dVertBuf.h"
#include "../game/game.h"
#include <cmath>

CGun::CGun()
{
	mModel = CManagedModel::Load(&gResourceManager, "ball.ms3d");
	mTime = 0.0f;
}

void CGun::SetPosition(SVector3 pos)
{
	mPos = pos;
}

void CGun::Update( float dt)
{
	mTime += dt;
	if (mTime > 2.0f)
	{
		mTime -= 2.0f;

		CPObject* mObject = new CPObject( true );
		gWorld->Add( mObject );

		SBoundingBox b;
		b.min = SVector3( -1.0f, -1.0f, -1.0f );
		b.max = SVector3( 1.0f, 1.0f, 1.0f );

		SSphere s;
		s.position = SVector3( 0.0f, 0.0f, 0.0f );
		s.radius = 1.0f;

		mObject->SetSphere( s );
		mObject->SetBoundingBox( b );
		mObject->SetPosition( mPos );
		mObject->SetObjectType(CANNON_OBJ_TYPE);
		mObject->SetCollisionMask( 1 );
		mObject->SetFriction( 0.0f );
		mObject->SetVelocity(SVector3(0.0f, 0.0f, -30.0f));

		mShot.Add(mObject);
	}	

	CListEntry<CPObject>* o = mShot.GetFirst();
	while (o)
	{
		CListEntry<CPObject>* next = o->GetNext();
		SVector3 pos = o->data->GetPosition();
		if ( (pos - mPos).LengthSquared() > 50.0f*50.0f )
		{
			gWorld->Remove(o->data);
			delete o->data;
			mShot.RemoveEntry(o);
		}
		o = next;
	}
}

void CGun::Render()
{
	CListEntry<CPObject>* o = mShot.GetFirst();
	while (o)
	{
		glPushMatrix();

		SVector3 pos = o->data->GetPosition();

		glTranslatef( pos.x, pos.y, pos.z );

		glColor3f(0.0f, 0.0f, 0.0f);

		//mTexture->Bind();

		mModel->Render();

		//mTexture->Unbind();

		glPopMatrix();
		o = o->GetNext();
	}
}
