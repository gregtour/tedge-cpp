
#include "../main.h"
#include "../../engine/common/timer.h"
#include "../../engine/graphics/model.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/sound/wavesound.h"
#include "../game/game.h"
#include "pickup.h"
#include "particles/spin.h"
#include <string>
#include <sstream>
#include <cmath>


const char* PICKUP_SOUND		= "pop.wav";

CPickup::CPickup()
{
	//mModel = CManagedModel::Load( &gResourceManager, PICKUP_MODEL_FILE );
	mModel = NULL;
	mTexture = NULL;
	//mPickupSound = CManagedWaveSound::Load(&gResourceManager, PICKUP_SOUND);
	mPickupSound = NULL;
}

CPickup::~CPickup()
{
}


void CPickup::Render()
{
	if (mObject && mModel)
	{
		SVector3 pos = mObject->GetPosition();
		{
			float x, y, z;
			gCamera->GetPosition( &x, &y, &z );
			if ( (x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y) + (z-pos.z)*(z-pos.z) > VIEW_DISTANCE_SQUARED )
				return;
		}
		//float r = mObject->GetSphere().radius;

		glTranslatef( pos.x, pos.y, pos.z );
		glRotatef(mRot * PICKUP_XROT_SPEED * DEGREES, 0.0f, 1.0f, 0.0f );
		glColor3f(1.0f, 1.0f, 1.0f);

		if (mTexture)
			mTexture->Bind();

		mModel->Render();
		
		if (mTexture)
			mTexture->Unbind();
	}
}

void CPickup::Update(float dt)
{
	if (mRot < 2.0f*PI)
		mRot += 1.0f * dt;
	else
		mRot = 0.0f;
}

void CPickup::PickedUp(CRacer* by)
{
	if (mPickupSound)
		mPickupSound->Play(0);//, mObject->GetPosition());
    SVector3 pos = mObject->GetPosition();
    gEntities.Add(new CSpin(pos.x, pos.y, pos.z));
    by->Cupcake();
    //new CDeath();
}

void CPickup::Spawn()
{
	SVector3 gravity(0.0f, 0.0f, 0.0f);
	
	SSphere sphere;
	sphere.position = SVector3(0.0f, 1.5f, 0.0f);
	sphere.radius = 4.0f;

	SBoundingBox box;
	box.max = SVector3(4.0f, 4.0f, 4.0f);
	box.min = SVector3(-4.0f, -4.0f, -4.0f);

	CreatePhysical( gWorld );
	CPObject* obj = GetPhysicalObject();
	obj->SetPosition( SVector3( rand()%500, rand()%50+20.5f, rand()%500 ) );
	obj->SetObjectType( ITEM_OBJECT_TYPE );
	obj->SetDefaultForce( gravity );
	obj->SetFriction( 5.0f );
	obj->SetBoundingBox( box );
	obj->SetSphere( sphere );
	obj->SetCollisionMask( 1 );
}


