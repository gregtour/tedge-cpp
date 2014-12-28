// racer.cpp for CRacer entity

#include <string>
#include <sstream>
#include <cmath>


#include "../main.h"
#include "../../engine/common/timer.h"
#include "../../engine/graphics/model.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/lighting.h"
#include "../../engine/sound/wavesound.h"
#include "../../engine/sound/music.h"
#include "../game/game.h"
#include "particles/explosion.h"
#include "racer.h"
#include "projectile.h"
#include "health.h"

CModel* balloons[4] = {NULL, NULL, NULL, NULL};
CModel* morph = NULL;

const char* RACER_MODEL_FILE	= "racer1.ms3d";
const char* RACER_TEXTURE_FILE	= "thick.png";

CRacer::CRacer()
{
    mNumBalloons = 0;
	mModel = CManagedModel::Load( &gResourceManager, RACER_MODEL_FILE );
	mTexture = CManagedTexture::Load(&gResourceManager, RACER_TEXTURE_FILE);
	mColor = SVector3(1.0f, 1.0f, 1.0f);

    if (!balloons[0])
        {
        balloons[0] = new CModel("balloon1.ms3d");
        balloons[1] = new CModel("balloon2.ms3d");
        balloons[2] = balloons[0];
        balloons[3] = new CModel("balloon3.ms3d");
        morph = new CModel("balloon1.ms3d");
        }
}
extern CMusic* gBackgroundMusic2;

void CRacer::Cupcake()
    {
    SVector3 v = mObject->GetVelocity();
    v.y = 30.0f;
    mObject->SetVelocity(v);


    mNumBalloons++;
    if (mNumBalloons == 20)
        {
	    mModel = CManagedModel::Load( &gResourceManager, "win.ms3d" );
        gBackgroundMusic2->Play(0);
        CHealth::mMagic = 1;
        }
    }

CRacer::~CRacer()
{
}

/*
void CRacer::Shoot()
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
		bulletbox.max = SVector3( .25f, 0.5f, .25f);
		bulletbox.min = SVector3(-.25f, -0.5f, -.25f);

		CProjectile* bullet;
		bullet = new CProjectile(mColor);
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
*/

void CRacer::Render()
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

        for (int pass = -1; pass < 3; pass++)
            {
            if (pass >= 0)
                {
                glPopMatrix();
                glPushMatrix();
                }

		    // EnableLighting( &gLights, pos.x, pos.y, pos.z );
            //gLights.GetFirst()->GetNext()->data->Apply(0);
            glDisable(GL_LIGHTING);
            if (pass != -1)
                {
                glDepthMask(GL_TRUE);
                //glDisable(GL_DEPTH_TEST);
                }
            else
                glDepthMask(GL_FALSE);

		    glTranslatef( pos.x, pos.y, pos.z );
		    glRotatef(mRot * DEGREES, 0.0f, -1.0f, 0.0f );
            float size = 0.975f + pass * 0.025f;
            if (pass == -1)
                size = 1.1f;
            glScalef(size, size, size);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            if (pass == -1)
                glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
            else
		        glColor4f(mColor.x, mColor.y, mColor.z, 0.7f - pass * 0.3f);
		    //glEnable(GL_BLEND);
		    //glBlendFunc(GL_ONE, GL_ONE);
		    //glDisable(GL_CULL_FACE);
		    //glDepthMask(GL_FALSE);

		    mTexture->Bind();
		    mModel->Render();
		    mTexture->Unbind();
            }
	}

    Balloons();
}


void CRacer::Update(float dt)
{
    mTime += dt;

	if (!mObject)
	{
		Spawn();
	}
}

void CRacer::Spawn(SVector3 pos)
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
	obj->SetPosition(pos);
	obj->SetObjectType(RACER_OBJECT_TYPE);
	obj->SetDefaultForce(gravity);
	obj->SetFriction(2.5f);
	obj->SetBoundingBox(tankbox);
	obj->SetSphere(tanksphere);
	obj->SetCollisionMask(1);
}


void CRacer::TurnLeft(float dt)
{
	mRot -= RACER_ROT_SPEED * dt;
}

void CRacer::TurnRight(float dt)
{
	mRot += RACER_ROT_SPEED * dt;
}

void CRacer::MoveForward(float dt)
{
    

	SVector3 force;
	force.x = -cos(mRot) * RACER_SPEED * dt;
	force.z = -sin(mRot) * RACER_SPEED * dt;
	mObject->ApplyForce(force);
}

void CRacer::MoveBackward(float dt)
{
	MoveForward(dt * -0.7f);
}

void CRacer::Hit(CProjectile* by)
{
}

void CRacer::Follow(CRacer* behind, float dt)
{
    CPObject* target = behind->GetPhysicalObject();

	if (mObject)	// if it's not dead:
	{
		// Enemy AI, omg
		SVector3 pos = mObject->GetPosition();

		if (!target)	// if we don't have a target, find the closest thing to kill
		{
/*
			CTank* closest = NULL;
			float closestDistance = 0.0f;
			CListEntry<CEntity>* ent = gEntities.GetFirst();
			while (ent)
			{
				if (ent->data->ClassName().compare("tank") == 0 && ent->data != this)
				{
					CTank* possible = (CTank*)ent->data;
					if (possible->GetPhysicalObject())
					{
						SVector3 distanceVect = possible->GetPhysicalObject()->GetPosition() - pos;
						float distance = distanceVect.LengthSquared();
						if (!closest || distance < closestDistance)
						{
							closest = possible;
							closestDistance = distance;
						}
					}
				}
				ent = ent->GetNext();
			}
			
			mTarget = closest;
*/
		} else {	// if we do have a target, kill it
			SVector3 targetPos = target->GetPosition();
			SVector3 distance = targetPos - mObject->GetPosition();

            float speed = (distance.Length() > 25.0f ? 1.0f : distance.Length() / 25.0f);
            speed *= speed;
            if (speed > 0.5f)
			    MoveForward(speed * dt);

			distance.Normalize();
			SVector3 velocity = mObject->GetVelocity();
			velocity.y = 0.0f;
			velocity.Normalize();

			float cosAng = distance.CrossProduct(velocity).y;
			float sinAng = distance.DotProduct(velocity);
			if (sinAng > 0.0f)
			{
				if (cosAng < -0.2f)
					TurnLeft(dt);
				else if (cosAng > 0.2f)
					TurnRight(dt);
				else {
					// Fire when ready captain
					//if (mLastShoot.Time() > TANK_SHOOT_TIME)
					//{ Shoot(); }
				}
			} else {
				if (cosAng < 0.0f)
					TurnLeft(dt);
				else if (cosAng > 0.0f)
					TurnRight(dt);
			}
		}
	}
}



void CRacer::Balloons()
    {
    //CRacer::Render();

	if (mObject)
        for (int b = 0; b < mNumBalloons; b++)
	    {
        //if (b)
            {
            glPopMatrix();
            glPushMatrix();
            }

		SVector3 pos = mObject->GetPosition();
		glClearColor(1.0f, 0.65f+(pos.y/300.0f), (pos.x/500.0f), 1.0f);

		// EnableLighting( &gLights, pos.x, pos.y, pos.z );
        glDisable(GL_LIGHTING);

		glTranslatef( pos.x, pos.y, pos.z );
		glRotatef(mRot * DEGREES + 30.0f, 0.0f, -1.0f, 0.0f );
        glTranslatef(-2.5f, 0.7f, -1.5f);
        glRotatef(30.0f*(b/3), 0.0f, 1.0f, 0.0f);
        glRotatef(30.0f*(b-1), 1.0f, 0.0f, 0.0f);

        glScalef(0.05f, 0.05f, 0.05f);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float colors[5][3] = {{1.0f, 0.1f, 0.2f}, {0.2f, 0.1f, 1.0f}, {0.2f, 0.8f, 0.1f}, {0.8f, 0.2f, 0.6f},  {0.2f,0.8f, 0.6f}};
        glColor4f(colors[b%5][0], colors[b%5][1], colors[b%5][2], 1.0f);
/*
        if (b == 0)
		    glColor4f(1.0f, 0.1f, 0.2f, 1.0f);
        if (b == 1)
		    glColor4f(0.2f, 0.1f, 1.0f, 1.0f);
        if (b == 2)
		    glColor4f(0.2f, 0.8f, 0.1f, 1.0f);*/
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE, GL_ONE);
		//glDisable(GL_CULL_FACE);
		//glDepthMask(GL_FALSE);

		//mTexture->Bind();
        balloons[((int)(mTime+b*0.3f))%4]->Render();
		//mModel->Render();
		//mTexture->Unbind();

	}
}

