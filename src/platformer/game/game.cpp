#include "../../engine/common/list.h"
#include "../../engine/engine.h"
#include "../../engine/physics/collision.h"
#include "game.h"
#include "../main.h"
#include "../../engine/engine.h"
#include "../entity/player.h"
#include <cmath>

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
		if (col->Contains(PLAYER_OBJ_TYPE))
		{
			CPlayer* player = (CPlayer*)col->Get(PLAYER_OBJ_TYPE);
            player->SetGrounded(true);
            
			if (fabs(col->contactPoint.y - player->GetPhysicalObject()->GetPosition().y) < TRIANGLE_THICKNESS * 2.0f && fabs(col->normal.y) > 0.3f)
			{
				player->SetGrounded(true);
			}

			if (fabs(col->contactPoint.y - player->GetPhysicalObject()->GetPosition().y) < TRIANGLE_THICKNESS * 1.1f && fabs(col->normal.y) == 1.0f)
			{
				if (col->Contains(GROUND_OBJ_TYPE))
					player->SetSafe();
				if (col->Contains(END_OBJ_TYPE))
					gState = NEXT_LEVEL_STATE;
			}
		}

		delete collisionEntry->data;
		gCollisions.RemoveEntry( collisionEntry );
		collisionEntry = gCollisions.GetFirst();
	}

}


