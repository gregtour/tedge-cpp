/********************
 * PLATFORM 
* RAPID HURRY AAA JUNE 28TH 2009
 ********************/

#include "SDL.h"
#include <cmath>
#include "../engine/engine.h"
#include "../engine/update.h"
#include "../engine/graphics/graphics.h"
#include "entity/player.h"
#include "entity/vehicle.h"
#include "entity/track.h"
#include "entity/zombie.h"
#include "entity/cameraController.h"
#include "game/game.h"
#include "main.h"

int curLevel = 0;

const char* gameTitle = "ZOMBIES STOLE MY TRUCK";

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
	// Screen clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

	//CLevel* level = new CLevel();
	//gEntities.Add(level);
    CTrack* track = new CTrack();
    gEntities.Add(track);

	CPlayer* player;// = level->LoadMS3D(levels[curLevel], gWorld);
    {
		player = new CPlayer();
		gEntities.Add(player);
        player->CreatePhysical(gWorld);
        CPObject* obj = player->GetPhysicalObject();

		SBoundingBox b;
		b.min = SVector3( -1.0f, 0.0f, -1.0f );
		b.max = SVector3( 1.0f, 1.0f, 1.0f );
		
		SSphere s;
		s.position = SVector3( 0.0f, 1.0f, 0.0f );
		s.radius = 1.0f;

		obj->SetPosition( SVector3(400, 50, 400) );
		obj->SetSphere( s );
		obj->SetBoundingBox( b );
		obj->SetObjectType(PLAYER_OBJ_TYPE);
		obj->SetCollisionMask( 1 | 2 );
		obj->SetDefaultForce( SVector3( 0.0f, -145.0f, 0.0f ) );
		obj->SetFriction( 16.0f );

		player->SetRot( 3.01f );
		gEntities.Add(player);
	}

	CVehicle* vehicle;// = level->LoadMS3D(levels[curLevel], gWorld);
    {
		vehicle = new CVehicle();
		gEntities.Add(vehicle);
	}

    /* ADD ZOMBIES */
    for (int z = 0; z < 1500; z++) {
        CZombie* zombie;
        zombie = new CZombie();
        zombie->CreatePhysical(gWorld);
        CPObject* obj = zombie->GetPhysicalObject();

		SBoundingBox b;
		b.min = SVector3( -1.0f, 0.0f, -1.0f );
		b.max = SVector3( 1.0f, 1.0f, 1.0f );
		
		SSphere s;
		s.position = SVector3( 0.0f, 1.0f, 0.0f );
		s.radius = 1.0f;

		obj->SetPosition( SVector3(rand()%1000, 20, rand()%1000 ));
		obj->SetSphere( s );
		obj->SetBoundingBox( b );
		obj->SetObjectType(5);
		obj->SetCollisionMask( 1 | 2 );
		obj->SetDefaultForce( SVector3( 0.0f, -145.0f, 0.0f ) );
		obj->SetFriction( 16.0f );

		zombie->SetRot( sinf(z)*3 );
		gEntities.Add(zombie);
    }


	CCamera* cam = new CCamera();
	BindCamera(cam);

	CCameraController* camControl = new CCameraController();
	camControl->SetViewAngle(0.45f);
	camControl->SetViewDistance(12.0f);
	camControl->ControlCamera(cam);
	camControl->FocusOnObject(vehicle);
	gEntities.Add(camControl);	

    // lighting stuff
    GLfloat ambient_light[] = {0.4985f, 0.4985f, 0.4985f, 1.0f};
    GLfloat diffuse_light[] = {0.420f, 0.420f, 0.475f, 1.0f};
    GLfloat light_pos[] = {0.5f, -0.7746f, -0.387f, 0.0f};
    GLfloat diffuse_light_b[] = {0.36f, 0.46f, 0.46f, 1.0f};
    GLfloat light_pos_b[] = {-3.87f, 0.5f, -0.7746f, 0.0f};
    GLfloat ambient_light_c[] = {0.3f,0.3f,0.3f,1.0f};
    GLfloat ambient_dir[] = {0,0,0,0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light_b);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos_b);
    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient_light_c);
    glLightfv(GL_LIGHT2, GL_POSITION, ambient_dir);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    //glEnable(GL_AUTO_NORMAL);
    
    glShadeModel(GL_SMOOTH);
//    for (int k = 0; k < 100; k++)
//        gWorld->Update(1.0f);

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

