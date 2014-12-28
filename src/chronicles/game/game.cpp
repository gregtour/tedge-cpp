#include "../entity/level.h"
#include "../entity/gate.h"
#include "../entity/player.h"
#include "../entity/monster.h"
#include "../entity/spike.h"
#include "../entity/item.h"
#include "../entity/bat.h"
#include "../entity/portal.h"
#include "../../engine/engine.h"
#include "../main.h"
#include "../entity/pickup.h"
#include "../../engine/physics/collision.h"

//#define dynamic_cast static_cast

int gPlayerScore;
int gPlayerSwordLevel;

void InitPlayerStats()
{
	gPlayerScore = 0;
	gPlayerSwordLevel = 1;
}

void HandleCollisions()
{
/*	Object Types
	0
	
	2		Item
	3		Monster
	7		Bat Monster

	4		World Walls
	6		Static Physics

	1		Player
	5		Sword
	8		Sword Blocking

	9		End of Level Portal

	10		"Pickup" (Health, Key, etc)

	11		Ghoul/Phantom Enemy 

	12		PLAYER_LEVITATE_OBJ

	13		Gate object

	?		Use Button Sensor
				- A circle slightly larger than the player, slightly in front of player,
				that detects which objects should be triggered when the use button is pressed,
				such as items like Treasure Chests, or Doors.
*/
	CListEntry<CCollision>* collisionEntry = gCollisions.GetFirst();
	while ( collisionEntry )
	{
		IPhysicalObj* object1 = collisionEntry->data->object1;
		IPhysicalObj* object2 = collisionEntry->data->object2;
		int type1 = collisionEntry->data->type1;
		int type2 = collisionEntry->data->type2;
		
		//if ( gWorld->IsObjectInSimulation( object1 ) && gWorld->IsObjectInSimulation( object2 ) )
		{
			if ( type2 == PLAYER_OBJ_TYPE || type2 == PLAYER_LEVITATE_OBJ || type2 == SWORD_OBJ_TYPE  || type1 == SPIKE_OBJ_TYPE)
			{
				IPhysicalObj* temp = object1;
				int	t;
				object1 = object2;
				object2 = temp;
				t = type1;
				type1 = type2;
				type2 = t;
			}

			/*	Player Collision	*/
			if ( type1 == PLAYER_OBJ_TYPE && gPlayer != NULL )
			{
				if ( type2 == MONSTER_OBJ_TYPE )
				{
					CMonster* monster = (CMonster*)object2;
					if (gPlayer->Hit( monster, monster->AttackStrength() ))
					{
						CListEntry<CCollision>* col = collisionEntry->GetNext();
						while (col)
						{
							CListEntry<CCollision>* n = col->GetNext();
							if (col->data->object1 == object1 || col->data->object2 == object1)
							{
								delete col->data;
								gCollisions.RemoveEntry( col );
							}
							col = n;
						}
					}
				}
				else if ( type2 == BAT_OBJ_TYPE )
				{
					CCharacter* bat = (CCharacter*)object2;
//					if ( bat->GetY() < PLAYER_HEIGHT )
					if (gPlayer->Hit( bat, 1 ))
					{
						CListEntry<CCollision>* col = collisionEntry->GetNext();
						while (col)
						{
							CListEntry<CCollision>* n = col->GetNext();
							if (col->data->object1 == object1 || col->data->object2 == object1)
							{
								delete col->data;
								gCollisions.RemoveEntry( col );
							}
							col = n;
						}
					}
				}
				else if ( type2 == PORTAL_OBJECT_TYPE )
				{
					SVector3 v;
					v = object2->GetPhysicalObject()->GetPosition() - object1->GetPhysicalObject()->GetPosition();
					//object2->GetPosition().Difference( &object1->GetPosition(), &v );
					//v.Scale( 6.0f );
					v = v * 6.0f;
					v.y = 0.0f;
					object1->GetPhysicalObject()->SetVelocity( v );
					//object1->GetPhysicalObject()->SetDefaultForce(SVector3(0.0f, 10.0f, 0.0f));

					CPortal* portal = (CPortal*)object2;
					if ( portal )
						portal->Glow();
					gPlayer->Levitate();

					/*	Next Level	*/
					//if ( gPlayer->GetPhysicalObject()->GetPosition().y > (portal->GetPhysicalObject()->GetPosition().y + 6.0f) )
					//	ChangeGameState( NEXT_LEVEL_STATE );
					
				}
				else if ( type2 == PICKUP_OBJ_TYPE )
				{
					CPickup* pickup = (CPickup*)object2;
					
					if ( pickup )
						pickup->PickUp();
				}
				else if ( type2 == GATE_OBJ_TYPE )
				{
					CGate* gate = (CGate*)object2;
					if ( !gate->IsOpen() && gPlayer->RemoveKey() )
					{						
						gate->Open();
					}
				}
				else if ( type2 == SPIKE_OBJ_TYPE )
				{
					CMonster* monster = (CMonster*)object2;
					if ( ((CSpike*)monster)->IsUp() )
					{
						gPlayer->Hit( monster, 2 );
					}
				}
			}
			else if ( type1 == PLAYER_LEVITATE_OBJ && gPlayer != NULL )
			{
				if ( type2 == PORTAL_OBJECT_TYPE )
				{
					SVector3 v;
					v = object2->GetPhysicalObject()->GetPosition() - object1->GetPhysicalObject()->GetPosition();
					//object2->GetPosition().Difference( &object1->GetPosition(), &v );
					//v.Scale( 6.0f );
					v = v * 6.0f;
					v.y = 1.0f;
					object1->GetPhysicalObject()->SetVelocity( v );
					//object1->GetPhysicalObject()->SetDefaultForce(SVector3(0.0f, 10.0f, 0.0f));

					CPortal* portal = (CPortal*)object2;
					if ( portal )
						portal->Glow();
					gPlayer->Levitate();

					/*	Next Level	*/
//					if ( gPlayer->GetPhysicalObject()->GetPosition().y > (portal->GetPhysicalObject()->GetPosition().y + 6.0f) )
//						ChangeGameState( NEXT_LEVEL_STATE );
								
				}
			}
			/*	Sword Collision		*/
			else if ( type1 == SWORD_OBJ_TYPE && gPlayer != NULL )
			{
				if ( type2 == MONSTER_OBJ_TYPE )
				{
					if ( ((CCharacter*)object2)->Hit( gPlayer, SWORD_DAMAGE ))
					{
						CListEntry<CCollision>* col = collisionEntry->GetNext();
						while (col)
						{
							CListEntry<CCollision>* n = col->GetNext();
							if (col->data->object1 == object2 || col->data->object2 == object2)
							{
								delete col->data;
								gCollisions.RemoveEntry( col );
							}
							col = n;
						}
					}
				}
				else if ( type2 == BAT_OBJ_TYPE )
				{
					CCharacter* bat = (CCharacter*)object2;
					//if ( bat->GetY() < PLAYER_HEIGHT + 1.0f )
					if (bat->Hit( gPlayer, SWORD_DAMAGE ))
					{
						CListEntry<CCollision>* col = collisionEntry->GetNext();
						while (col)
						{
							CListEntry<CCollision>* n = col->GetNext();
							if (col->data->object1 == object2 || col->data->object2 == object2)
							{
								delete col->data;
								gCollisions.RemoveEntry( col );
							}
							col = n;
						}
					}
				}
				else if ( type2 == ITEM_OBJ_TYPE )
				{
					//(dynamic_cast<CItem*>((IPhysicalObj*)object2->GetOwner()))->Hit( (IPhysicalObj*)gPlayer, PLAYER_SWORD_FORCE );
				}
				/*else if ( object2->GetObjectType() == WALL_OBJ_TYPE )
				{
					gPlayer->StopSwing();
				}*/
			}

			if ( type2 == SPIKE_OBJ_TYPE )
			{
				if ( type1 == MONSTER_OBJ_TYPE )
				{
					CCharacter* by = (CCharacter*)object2;
					if ( ((CSpike*)by)->IsUp() )
					{
						((CCharacter*)object1)->Hit( by, 2 );
					}
				}
			}
		}
		
		delete collisionEntry->data;
		gCollisions.RemoveEntry( collisionEntry );
		collisionEntry = gCollisions.GetFirst();
	}

}



