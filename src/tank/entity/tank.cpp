// tank.cpp for CTank entity

#include "../main.h"
#include "../../engine/common/timer.h"
#include "../../engine/graphics/model.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/sound/wavesound.h"
#include "../game/game.h"
#include "particles/explosion.h"
#include "tank.h"
#include "bullet.h"
#include <string>
#include <sstream>
#include <cmath>


const int TANK_MAX_HEALTH = 8;
const char* TANK_MODEL_FILE		= "tank.ms3d";
const char* TANK_TEXTURE_FILE	= "middle.png";
const char* TANK_SHOOT_SOUND	= "laser.wav";
const char* TANK_HIT_SOUND		= "hit.wav";
const char* TANK_EXPLODE_SOUND	= "explosion.wav";

CTank::CTank()
{
	mHealth = TANK_MAX_HEALTH;
	mModel = CManagedModel::Load( &gResourceManager, TANK_MODEL_FILE );
	mShootSound = CManagedWaveSound::Load(&gResourceManager, TANK_SHOOT_SOUND);
	mHitSound = CManagedWaveSound::Load(&gResourceManager, TANK_HIT_SOUND);
	mExplodeSound = CManagedWaveSound::Load(&gResourceManager, TANK_EXPLODE_SOUND);
	mTexture = CManagedTexture::Load(&gResourceManager, TANK_TEXTURE_FILE);
	mLastShoot.Clear();
	mColor = SVector3(1.0f, 1.0f, 1.0f);
	mCannon = 0;
	mPulse = 0.0f;
}

CTank::~CTank()
{
}

void CTank::Shoot()
{
	if (mLastShoot.Time() > TANK_SHOOT_TIME)
	{
		mCannon = (mCannon+1)%2;
		mLastShoot.Step();

		SVector3 pos(mObject->GetPosition());
		mShootSound->Play(0, pos);

		//SVector3 gravity(0.0f, -15.0f, 0.0f);
		pos.x += cos(mRot+PI) * 1.75f + cos(mRot+HALF_PI)*0.5f*(mCannon*2-1);
		pos.y += 0.5f;
		pos.z += sin(mRot+PI) * 1.75f + sin(mRot+HALF_PI)*0.5f*(mCannon*2-1);

		SVector3 force(cos(mRot+PI) * BULLET_SPEED, 0, sin(mRot+PI) * BULLET_SPEED);

		SSphere bulletsphere;
		//bulletsphere.position = SVector3(0.0f, .25f, 0.0f);
		bulletsphere.position = SVector3(0.0f, 0.0f, 0.0f);
		bulletsphere.radius = .25f;

		SBoundingBox bulletbox;
		/*bulletbox.max = SVector3(-.27f, .04f, .06f);
		bulletbox.min = SVector3(-.25f, -.05f, -.06f);*/
		bulletbox.max = SVector3( .25f, 0.5f, .25f);
		bulletbox.min = SVector3(-.25f, -0.5f, -.25f);

		CBullet* bullet;
		bullet = new CBullet(mColor);
		bullet->CreatePhysical(gWorld);
		bullet->SetRot(mRot);
		CPObject* obj = bullet->GetPhysicalObject();
		obj->SetObjectType(BULLET_OBJECT_TYPE);
		obj->SetPosition(pos);
		obj->SetVelocity(force);
		obj->SetBoundingBox(bulletbox);
		obj->SetSphere(bulletsphere);
		obj->SetCollisionMask(2);
		gEntities.Add(bullet);
		bullet->Explode();
	}
}

void CTank::Render()
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
		glRotatef(mRot * DEGREES, 0.0f, -1.0f, 0.0f );

		glColor3f(mColor.x, mColor.y, mColor.z);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE, GL_ONE);
		//glDisable(GL_CULL_FACE);
		//glDepthMask(GL_FALSE);

		mTexture->Bind();
		mModel->Render();
		mTexture->Unbind();
	}
}

void CTank::AddHealth()
{
	mHealth += 3;
	if (mHealth > TANK_MAX_HEALTH)
		mHealth = TANK_MAX_HEALTH;
}

void CTank::Explode()
{
	mExplodeSound->Play(0, mObject->GetPosition());
	gEntities.Add(new CExplosion(mObject->GetPosition(), mColor.x, mColor.y, mColor.z, 3.0f, 14));
}

void CTank::Update(float dt)
{
	mPulse += dt;

	if (!mObject)
	{
		if (mRespawnTime.Time() > TANK_RESPAWN_TIME)
			Spawn();
	}
	else if (mHealth <= 0)
	{
		Explode();
		SetWorld(NULL);
		delete mObject;
		mObject = NULL;
		mHealth = TANK_MAX_HEALTH;
		mRespawnTime.Step();
	}
}

void CTank::Spawn()
{
	SVector3 gravity(0.0f, -45.0f, 0.0f);
	
	SSphere tanksphere;
	tanksphere.position = SVector3(0.0f, 1.5f, 0.0f);
	tanksphere.radius = 1.5f;

	SBoundingBox tankbox;
	tankbox.max = SVector3(1.5f, 2.0f, 1.5f);
	tankbox.min = SVector3(-1.5f, 0.0f, -1.5f);

	CreatePhysical(gWorld);
	CPObject* obj = GetPhysicalObject();
	//obj->SetPosition(SVector3(50.0f, 0.0f, 50.0f));
	obj->SetPosition(SVector3(rand()%199, 5.0f, rand()%199));
	obj->SetObjectType(TANK_OBJECT_TYPE);
	obj->SetDefaultForce(gravity);
	obj->SetFriction(0.75f);
	obj->SetBoundingBox(tankbox);
	obj->SetSphere(tanksphere);
	obj->SetCollisionMask(1);
}


void CTank::TurnLeft(float dt)
{
	mRot -= TANK_ROT_SPEED * dt;
}

void CTank::TurnRight(float dt)
{
	mRot += TANK_ROT_SPEED * dt;
}

void CTank::MoveForward(float dt)
{
	SVector3 force;
	force.x = -cos(mRot) * TANK_SPEED * dt;
	force.z = -sin(mRot) * TANK_SPEED * dt;
	mObject->ApplyForce(force);
}

void CTank::MoveBackward(float dt)
{
	MoveForward(dt * -0.7f);
}

void CTank::Hit(CBullet* by)
{
	mHealth--;
	if (mHealth > 0)
		mHitSound->Play(0, mObject->GetPosition());
}


