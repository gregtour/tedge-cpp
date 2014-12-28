/********************
 * PLATFORM 
* RAPID HURRY AAA JUNE 28TH 2009
 ********************/

#include "SDL.h"
#include "../engine/engine.h"
#include "../engine/update.h"
#include "../engine/graphics/graphics.h"
#include "entity/player.h"
#include "entity/level.h"
#include "entity/cameraController.h"
#include "game/game.h"
#include "main.h"

int curLevel = 0;

const char* gameTitle = "bloke";

const char* levels[] =
{
	"level0.ms3d",
	"level1.ms3d",
	"level2.ms3d",
	"level3.ms3d"
};


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

void NextLevel()
{
	// Orange Sky of Happy
	glClearColor(1.0f, 0.65f, 0.0f, 1.0f);

	// Clear out anything that exists
	CListEntry<CEntity>* e = gEntities.GetFirst();
	for (; e; e = gEntities.GetFirst())
	{
		delete e->data;
		gEntities.RemoveEntry(e);
	}

	// delete physics & create a new one
	if (gWorld)
		delete gWorld;
	
	gWorld = new CPhysics();
	gWorld->SetCollisionFunction(&GameCollisionFunction);

	CLevel* level = new CLevel();
	gEntities.Add(level);
	IPhysicalObj* player = level->LoadMS3D(levels[curLevel], gWorld);

	CCamera* cam = new CCamera();
	BindCamera(cam);

	CCameraController* camControl = new CCameraController();
	camControl->SetViewAngle(0.85f);
	camControl->SetViewDistance(12.0f);
	camControl->ControlCamera(cam);
	camControl->FocusOnObject(player);
	gEntities.Add(camControl);	

	curLevel = (curLevel + 1) % (sizeof(levels) / sizeof(const char*));
}


void GameStart()
{
	NextLevel();
}


void GameStateChange( int from, int to )
{
	switch ( to )
	{
	case NEXT_LEVEL_STATE:
		NextLevel();
		ChangeGameState( DEFAULT_STATE );
		break;
	default:
		EndGame();
	}

}

