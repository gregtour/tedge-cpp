#include <math.h>
#include "enemy.h"
#include "particles/explosion.h"
#include "../../engine/graphics/texture.h"

const char* ENEMY_TEXTURE_FILE = "enemy.png";

CEnemy::CEnemy()
{
//	mTexture = CManagedTexture::Load(&gResourceManager, ENEMY_TEXTURE_FILE);
	mTarget = NULL;
	//mColor = SVector3(1.0f, 0.0f, 0.0f);
	mColor = HSV((rand()%12)*30, 1.0f, 1.0f);
}

CEnemy::~CEnemy()
{
}

void CEnemy::Update( float dt )
{
	if (mObject)	// if it's not dead:
	{
		// Enemy AI, omg
		CPObject* target;
		if (mTarget)
			target = mTarget->GetPhysicalObject();
		else
			target = NULL;

		SVector3 pos = mObject->GetPosition();

		if (!mTarget || !target)	// if we don't have a target, find the closest thing to kill
		{
			CTank* closest = NULL;
			float closestDistance = 0.0f;
			CListEntry<CEntity>* ent = gEntities.GetFirst();
			while (ent)
			{
				if (ent->data->ClassName().compare("playa") == 0 && ent->data != this)
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

		} else {	// if we do have a target, kill it
			SVector3 targetPos = target->GetPosition();
			SVector3 distance = targetPos - mObject->GetPosition();

			MoveForward(dt);

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
					if (mLastShoot.Time() > TANK_SHOOT_TIME)
					{ Shoot(); }
				}
			} else {
				if (cosAng < 0.0f)
					TurnLeft(dt);
				else if (cosAng > 0.0f)
					TurnRight(dt);
			}
		}
	}

	CTank::Update(dt);
}
