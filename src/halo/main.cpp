/********************
 * Karts 
 *   Or how I learned to stop worrying and love the tedge
 *     June 13th 2009
 *       Greg Tourville
 *         Team Duck
 ********************/

#include "SDL.h"
#include "../engine/engine.h"
#include "../engine/update.h"
#include "../engine/graphics/graphics.h"
#include "../engine/graphics/lighting.h"
#include "entity/player.h"
#include "entity/posse.h"
#include "entity/gun.h"
#include "entity/pickup.h"
#include "entity/health.h"
#include "entity/enemy.h"
//#include "entity/ghost.h"
#include "entity/track.h"
#include "entity/plat.h"
#include "entity/cameraController.h"
#include "game/game.h"
#include "../engine/sound/music.h"

CPlayer* gPlayer;
const char* gameTitle = "FIESTALAND Metropolitan Area!";


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



CMusic* gBackgroundMusic1;
CMusic* gBackgroundMusic2;

void GameStart()
{
    gLog.LogItem( new CLogMessage("happy 20th sam") );
    gLog.LogItem( new CLogMessage("FATAL ERROR: ENTERING FAILSAFE MODE") );

    gBackgroundMusic1 = new CMusic("01_-_opening.mp3");
    gBackgroundMusic2 = new CMusic("outcome.mp3");
    gBackgroundMusic1->Play(0);

	// Start up the game
	// this will eventually launch a menu
	// for now create the world, throw in the player, and throw in some bots
	gWorld = new CPhysics();
	gWorld->SetCollisionFunction(&GameCollisionFunction);
	SBoundingBox bounds;
	bounds.min = SVector3(0.0f, -50.0f, 0.0f);
	bounds.max = SVector3(500.0f, 100.0f, 500.0f);
	gWorld->SetBoundries(bounds);
//	gWorld->SortStaticObjects(100, 1, 100);

	//gEntities.Add(new CTrack());

	gPlayer = new CPlayer();
	gPlayer->Spawn();
	gEntities.Add(gPlayer);

    
    for (int buds = 0; buds < 3; buds++)
        {
        CPosse* guy = new CPosse(gPlayer);
        guy->Spawn(SVector3(80.0f+buds*2.0f, 255.0f, 70.0f+(buds-1)*5.0f));
        gEntities.Add(guy);
        }
/*
    for (int guns = 0; guns < 5; guns++)
        {
	    CGun* gun = new CGun();
	    gun->SetPosition(SVector3(40.0f+guns*2.0f, 230.0f, 60.0f+(guns-1)*5.0f));
	    gun->mTime = (guns%3)*0.33f;
	    gEntities.Add(gun);
        }*/


    CPlat* platt = new CPlat(gWorld);
	gEntities.Add(platt);
	platt->LoadMS3D("map1.ms3d", gWorld);

    /*
    CPlat* platt2 = new CPlat(gWorld);
	gEntities.Add(platt2);
	platt->LoadMS3D("trench.ms3d", gWorld);*/

	CLight* light = new CLight();
	float position[] = {40.0f, 50.0f, 60.0f, 1.0f};
	float ambient[] = {0.8f, 0.8f, 0.8f, 1.0f}; 
	float diffusive[] = {0.7f, 0.6f, 0.4f, 1.0f};
	float attenuation[] = {0.2f, 0.004f, 0.00027f};
	light->SetPosition( position );
	light->SetAmbient( ambient );
	light->SetDiffusive( diffusive );
	light->SetAttenuation( attenuation );
	gLights.Add(light);

	light = new CLight();
	float position2[] = {1.0f, -1.0f, 1.0f, 0.0f};
	float ambient2[] = {0.8f, 0.8f, 0.8f, 1.0f}; 
	float diffusive2[] = {0.6f, 0.6f, 0.5f, 1.0f};
	float attenuation2[] = {0.2f, 0.004f, 0.00017f};
	light->SetPosition( position2 );
	light->SetAmbient( ambient2 );
	light->SetDiffusive( diffusive2 );
	light->SetAttenuation( attenuation2 );
    //light->Spotlight(
	gLights.Add(light);

	GLfloat globalAmbient[] = { 0.9f, 0.9f, 0.9f, 0.0f };
	AmbientLighting( globalAmbient );



	CCamera* cam = new CCamera();
	BindCamera(cam);

	CCameraController* camControl = new CCameraController();
	camControl->SetViewAngle(0.35f);
	camControl->SetViewDistance(14.0f);
	camControl->ControlCamera(cam);
	camControl->FocusOnObject(gPlayer);
	gEntities.Add(camControl);
	

    gWorld->SortStaticObjects(50, 10, 50);
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

