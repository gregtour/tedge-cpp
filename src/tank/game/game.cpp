#include "../../engine/common/list.h"
#include "../../engine/engine.h"
#include "../game/game.h"
#include "../entity/tank.h"
#include "../entity/bullet.h"
#include "../entity/pickup.h"
#include "../entity/health.h"
#include "../../engine/physics/collision.h"

void PruneCollisions(CListEntry<CCollision>* next, IPhysicalObj* bad)
{
	while (next)
	{
		CListEntry<CCollision>* evenNext = next->GetNext();
		if (next->data->object1 == bad || next->data->object2 == bad)
		{
			delete next->data;
			gCollisions.RemoveEntry( next );
		}
		next = evenNext;
	}
}

void HandleCollisions()
{

	CListEntry<CCollision>* collisionEntry = gCollisions.GetFirst();
	while ( collisionEntry )
	{
		CCollision* col = collisionEntry->data;
		if (col->Contains(TANK_OBJECT_TYPE, BULLET_OBJECT_TYPE))
		{
			CBullet* bullet = (CBullet*)col->Get(BULLET_OBJECT_TYPE);
			((CTank*)col->Get(TANK_OBJECT_TYPE))->Hit(bullet);
			bullet->Explode();
			bullet->Destroy();
			PruneCollisions(collisionEntry->GetNext(), col->Get(BULLET_OBJECT_TYPE));
		}
		else if (col->Contains(TANK_OBJECT_TYPE, PICKUP_OBJECT_TYPE))
		{
			CPickup* pickup = (CPickup*)col->Get(PICKUP_OBJECT_TYPE);
			pickup->PickedUp((CTank*)col->Get(TANK_OBJECT_TYPE));
			pickup->Destroy();
			PruneCollisions(collisionEntry->GetNext(), col->Get(PICKUP_OBJECT_TYPE));
		}
		else if (col->Contains(BULLET_OBJECT_TYPE))
		{
			CBullet* bullet = (CBullet*)col->Get(BULLET_OBJECT_TYPE);
			bullet->Explode();
			bullet->Destroy();
			PruneCollisions(collisionEntry->GetNext(), col->Get(BULLET_OBJECT_TYPE));
		}

		delete collisionEntry->data;
		gCollisions.RemoveEntry( collisionEntry );
		collisionEntry = gCollisions.GetFirst();
	}

}


