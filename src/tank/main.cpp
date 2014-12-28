/********************
 * TANK 
 *   Or how I learned to stop worrying and love the tedge
 *     June 13th 2009
 *       Greg Tourville
 *         Team Duck
 ********************/

#include "SDL.h"
#include "../engine/engine.h"
#include "../engine/update.h"
#include "../engine/graphics/graphics.h"
#include "entity/player.h"
#include "entity/pickup.h"
#include "entity/health.h"
#include "entity/enemy.h"
#include "entity/level.h"
#include "entity/cameraController.h"
#include "game/game.h"


const char* gameTitle = "TANKS! IN 3D!!";


#ifdef _PSP
extern "C" int SDL_main( int argc, char *argv[] )
#else
int main( int argc, char *argv[] )
#endif
{
	gLog.SetLogging( true );

	Game();

	gLog.SaveLog( "gamelog.log" );
	gLog.ClearLog();

	return 0;
}




int GamePrecache()
{
	return 1;
}



void GameUnload()
{
}




void GameStart()
{

	//glClearColor(1.0f, 0.65f, 0.0f, 1.0f);
	//glClearColor(0.6f, 0.35f, 0.0f, 1.0f);
	//glClearColor(0.6f, 0.0f, 0.0f, 1.0f);
	// Start up the game
	// this will eventually launch a menu
	// for now create the world, throw in the player, and throw in some bots
	gWorld = new CPhysics();
	gWorld->SetCollisionFunction(&GameCollisionFunction);
	SBoundingBox bounds;
	bounds.min = SVector3(0.0f, 0.0f, 0.0f);
	bounds.max = SVector3(1000.0f, 100.0f, 1000.0f);
	gWorld->SetBoundries(bounds);
//	gWorld->SortStaticObjects(100, 1, 100);

	gEntities.Add(new CLevel());


	CTank*	player;
	player = new CPlayer();
	player->Spawn();
	gEntities.Add(player);

	for (int i = 0; i < 15; i++)
	{
		CEnemy*	enemy = new CEnemy();
		enemy->Spawn();
		gEntities.Add(enemy);
	}

	for (int i = 0; i < 10; i++)
	{
		CPickup* health = new CHealth();
		health->Spawn();
		gEntities.Add( health );
	}
	

	CCamera* cam = new CCamera();
	BindCamera(cam);

	CCameraController* camControl = new CCameraController();
	camControl->SetViewAngle(0.35f);
	//camControl->SetViewAngle(1.3f);
	camControl->SetViewDistance(6.0f);
	//camControl->SetViewDistance(39.0f);
	camControl->ControlCamera(cam);
	camControl->FocusOnObject(player);
	gEntities.Add(camControl);
	
}




void GameStateChange( int from, int to )
{
	// If the game had states, they'd probably look something like this:
/*	switch ( to )
	{
	case NEXT_LEVEL_STATE:
		NextLevel();
		ChangeGameState( DEFAULT_STATE );
		break;
	default:
		EndGame();
	}
*/
}

