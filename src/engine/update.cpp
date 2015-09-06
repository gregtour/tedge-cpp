#include "engine.h"
#include "update.h"
#include "entity/entity.h"
#include "physics/collision.h"
//#include "common/profile.h"

int GameUpdate( float dt )
{
//    ProfileStartClock(P_UPDATE);

	/*	Update Entities	*/
	CListEntry<CEntity>* entity = gEntities.GetFirst();
	while ( entity )
	{
		if ( entity->data )
		{
			CListEntry<CEntity>* next = entity->GetNext();
			entity->data->Update( dt );
			entity = next;
		} 
		else 
		{	/* Remove NULL entries	*/
			CListEntry<CEntity>* nextEntity = entity->GetNext();
			gEntities.RemoveEntry( entity );
			entity = nextEntity;
		}
	}

//    ProfileStopClock(P_UPDATE);
	return 1;
}

#ifdef THREADED_PHYSICS
pthread_mutex_t collision_mutex = PTHREAD_MUTEX_INITIALIZER;

void GameCollisionFunction(CCollision col)
{
    pthread_mutex_lock(&collision_mutex);
	CCollision* c = new CCollision();
	*c = col;
	gCollisions.Add( c );
    pthread_mutex_unlock(&collision_mutex);
}
#else
void GameCollisionFunction(CCollision col)
{
	CCollision* c = new CCollision();
	*c = col;
	gCollisions.Add( c );
}
#endif

