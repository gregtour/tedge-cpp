#ifndef _ENGINE_H
#define _ENGINE_H

#include "common/timer.h"
#include "common/list.h"
#include "common/log.h"
#include "common/resource.h"
#include "entity/entity.h"
#include "physics/physics.h"
#include "entity/camera.h"

#define ESCAPE_QUIT

#define DEFAULT_STATE	0


/*	Global Variables	*/
extern bool gRunning;
extern bool gActive;
extern bool gPaused;

extern int					gState;
extern CTimer				gTimer;
extern CPhysics*			gWorld;
extern CLog					gLog;
extern CLinkedList<CEntity>	gEntities;
extern CCamera*				gCamera;
extern CResourceManager		gResourceManager;
extern CLinkedList<CCollision> gCollisions;
extern float				gFPS;

/*		Defined, Implemented, and Created in Game Specific Code		*/
class COptions;
extern COptions gOptions;

extern const char* gameTitle;

/*	Functions		*/
void ChangeGameState( int newState );
void EndGame();
void Game();
void HandleCollisions();
int  SetupInput();
CEntity*    FindFirst(std::string type);
extern int  GamePrecache();
extern void GameUnload();
extern void GameStart();
extern void GameStateChange( int from, int to );

#endif
