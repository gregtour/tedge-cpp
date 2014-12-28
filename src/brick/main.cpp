/********************
 * BRICK the BREAKER 
 *   For Caleb + Not Doing Homework
 *     <3
 *       April 18th 2010
 *         Greg Tourville
 *           Team Duck
 ********************/

#include "SDL.h"
#include "../engine/engine.h"
#include "../engine/update.h"
#include "../engine/graphics/graphics.h"
#include "entity/paddle.h"
#include "entity/level.h"
#include "game/game.h"
#include "main.h"

const char* gameTitle = "BRICK the BREAKER";

CLevel* currentLevel = NULL;

void NextLevel();


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
	// Start up the game
	// this will eventually launch a menu
	// for now create the world, throw in the player, and throw in some bots
	gWorld = new CPhysics();
	gWorld->SetCollisionFunction(&GameCollisionFunction);
	SBoundingBox bounds;
	bounds.min = SVector3(0.0f, 0.0f, -1.0f);
	bounds.max = SVector3(40.0f, 30.0f, 2.0f);
	//gWorld->SetBoundries(bounds);

	CPaddle* player = new CPaddle(1);
	gEntities.Add(player);

	CCamera* cam = new CCamera();
	BindCamera(cam);

	NextLevel();

	cam->SetPosition(20.0f, 15.0f, 10.0f);
	cam->LookAt(20.0f, 15.0f, 0.0f);
}

void NextLevel()
{
	if (currentLevel != NULL)
	{
		gEntities.RemoveEntry(gEntities.FindEntry(currentLevel));
	}

	const char* layout[10] = 
	{
		"**********",
		"**********",
		"**********",
		"**********",
		"**********",
		"          ",
		"          ",
		"          ",
		"          ",
		"          "
	};

	currentLevel = new CLevel(layout); 
	gEntities.Add(currentLevel);
}


void GameStateChange( int from, int to )
{
	switch (to)
	{
	case NEXT_LEVEL:
		NextLevel();
		ChangeGameState(PLAYING);
		break;
	case PLAYING:
		break;
	case GAME_OVER:
		break;
	default:
		EndGame();
	}
}

