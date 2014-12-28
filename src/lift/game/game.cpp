#include "../entity/level.h"
#include "../entity/player.h"
#include "../entity/item.h"
#include "../../engine/engine.h"
#include "../main.h"

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
		delete collisionEntry->data;
		gCollisions.RemoveEntry( collisionEntry );
		collisionEntry = gCollisions.GetFirst();
	}

}


