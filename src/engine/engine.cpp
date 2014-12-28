/*
 * TeDGE:
 * Team Duck Game Engine
 * 
 * Copyright 2010 Team Duck
 */


/*	 Library Includes	*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
	#include <windows.h>
#endif

#include "SDL.h"
#include "SDL_opengl.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL_mixer.h"

/*	Accessory Classes	*/
#include "common/list.h"
#include "common/timer.h"
#include "common/resource.h"
#include "common/log.h"
#include "common/profile.h"

/*	Game Includes		*/
#include "entity/entity.h"
#include "entity/camera.h"

#include "engine.h"
#include "update.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "sound/sound.h"
#include "physics/physics.h"
#ifdef _NETWORKING
#include "network/network.h"
#endif 

/*	Global Variables	*/
bool gRunning;
bool gActive;
bool gPaused;

CLinkedList<CEntity>	gEntities;
CPhysics*				gWorld = NULL;
CLog					gLog;
CResourceManager		gResourceManager;
CLinkedList<CCollision> gCollisions;
float gFPS;
int gState;


/*	Code			*/

int Startup() 
{
	gState = DEFAULT_STATE;
	gRunning = true;
	gActive = true;
	gPaused = false;

#ifdef _DEBUG
	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER  | SDL_INIT_NOPARACHUTE | SDL_INIT_JOYSTICK ) != 0 )
		return 0;	
#else
	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK ) != 0)
		return 0;
#endif

	SDL_JoystickOpen(0);
	SDL_JoystickEventState(SDL_ENABLE);

	gLog.ResetTimeStamp();
    ProfileInit();

	if (
		SoundStartup() &&
		GraphicsStartup() &&
#ifdef _NETWORKING
		NetworkStartup() &&
#endif
		SetupInput() &&
		GamePrecache()
	)
		return 1;
	
	return 0;
}


void Shutdown() 
{ 
	GameUnload();
	SoundShutdown();
	GraphicsShutdown();
#ifdef _NETWORKING
	NetworkShutdown();
#endif
	SDL_Quit();

    ProfileResults();

	gResourceManager.UnloadAll();
}



void EndGame()
{
	gRunning = false;
}

void ChangeGameState( int newState )
{
	gState = newState;
}


void SdlEvents()
{
	SDL_Event event;

	while ( SDL_PollEvent( &event ) )
	{
		switch( event.type )
		{
		/*	Window Focus	*/
		case SDL_ACTIVEEVENT:
			if ( event.active.gain == 0 )
				gActive = true;
			else
				gActive = true;
			break;	

		/* Any of the Input Event Types */
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_JOYAXISMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			InputEvent( &event );
			break;
		
		case SDL_QUIT:
			EndGame();
			break;
		}
	}
}



extern int frame_count;
void Game()
{
	gLog.LogItem( new CLogMessage("Starting Up") );
	if ( Startup() == 0 )
	{
		gLog.LogItem( new CLogMessage("Error Starting Up") );
		gLog.LogItem( new CLogMessage("Shutting Down") );
		Shutdown();
		return;
	}

	gLog.LogItem( new CLogMessage("Starting Game") );
	GameStart();

	CGameTimer timer;
	timer.Clear();

	int ticks  = SDL_GetTicks();
	int lastTicks;

	int oldState = gState;
	int fpsTick = 0;
	gFPS = 10.0f;

    float total_time = 0.0f;
    int total_frames = 0;

	gLog.LogItem( new CLogMessage("Entering Main Loop") );
	while ( gRunning ) 
	{
		if ( oldState != gState )
			GameStateChange( oldState, gState );
		oldState = gState;

		gInputState->Update();
		SdlEvents();

		lastTicks = ticks;
		ticks = SDL_GetTicks();

		if (gPaused)
			CTimer::gPausedTicks += (ticks - lastTicks);
		timer.Step();
		
        total_time += (ticks - lastTicks) * 0.001f;
        total_frames++;

		fpsTick = (fpsTick+1) % (2);
		if ( fpsTick == 0 ) {
			gFPS = 1 / timer.DT();
            if (total_time > 1.0f) {
                printf("%.2f FPS\n", total_frames/total_time);
                total_time = 0.0f;
                total_frames = 0;
            }
        }

		if ( gActive )
		{
			if ( !gPaused )
			{
				gWorld->Update( timer.DT() );
				HandleCollisions();
				GameUpdate( timer.DT() );				
			} else {
				GameUpdate(0.0f);
			}

            ProfileIncrementFrame();
            if (frame_count % 2 == 0)
    			GameRender();
		}

	}

	gLog.LogItem( new CLogMessage("Shutting Down") );
    gLog.LogItem( new CLogMessage("<3 HAL") );
	Shutdown();
}


CEntity* FindFirst(std::string type)
    {
	CListEntry<CEntity>* ent = gEntities.GetFirst();
	while (ent)
        {
        if (ent->data->ClassName().compare(type) == 0)
            {
            return ent->data;
            }
        ent = ent->GetNext();
        }

    return NULL;
    }
