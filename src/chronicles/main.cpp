/****************************
 * lift: outrunner
 *	by team duck
 *	Feb 21 09
**/   

#include <vector>


#include "SDL.h"
#include "../engine/engine.h"
#include "../engine/sound/music.h"
#include "game/splash.h"

#include "game/game.h"
#include "entity/level.h"
#include "entity/player.h"
#include "entity/hud.h"
#include "entity/menuInput.h"
#include "entity/pause.h"
#include <fstream>

#ifdef _PSP
#include <psppower.h>
#include <pspkernel.h>

PSP_HEAP_SIZE_KB(-128);

#endif

#define NEXT_LEVEL_STATE	1
#define MAIN_MENU_STATE		2
#define PAUSE_STATE			3

CPlayer* gPlayer = NULL;
CLevel* gLevel = NULL;
CMusic* gBackgroundMusic = NULL;
CLinkedList<SDungeonEntry>	gDungeonList;
SDungeonEntry*				gCurrentDungeon = NULL;
int gLevelNumber = 0;

const char* gameTitle = "Chronicles";

void NextLevel();
void LoadDungeonList()
{
	std::ifstream input("levels.txt", std::ifstream::in);
	std::string name;
	input >> name;
	while (!input.eof())
	{
//		char tmp[256];
		SDungeonEntry* d = new SDungeonEntry();
//		input.getline(tmp, 255);
//		d->name = tmp;
//		input.getline(tmp, 255);
//		d->path = tmp;
//		input >> d->name;
		d->name = name;
		input >> d->path;
		input >> d->numLevels;
		input >> d->curLevel;
		std::string dum;
		gDungeonList.Add(d);
		input >> name;
	}
	input.close();
}

void SaveDungeonList()
{
	std::ofstream output("levels.txt");
	CListEntry<SDungeonEntry>* d = gDungeonList.GetFirst();
	while (d)
	{
		output << d->data->name << '\n';
		output << d->data->path << '\n';
		output << d->data->numLevels << '\n';
		output << d->data->curLevel;
		delete d->data;
		gDungeonList.RemoveEntry(d);
		d = gDungeonList.GetFirst();
		if (d)
			output  << "\n";
	}
	output.close();
}

#ifdef _PSP
/* Example custom exception handler */
/*
void ExceptionHandler(PspDebugRegBlock *regs)
{
	pspDebugScreenInit();

	pspDebugScreenSetBackColor(0x00FF0000);
	pspDebugScreenSetTextColor(0xFFFFFFFF);
	pspDebugScreenClear();

	pspDebugScreenPrintf("I regret to inform you your psp has just crashed\n");
	pspDebugScreenPrintf("Please contact Sony technical support for further information\n\n");
	pspDebugScreenPrintf("Exception Details:\n");
	pspDebugDumpException(regs);
	pspDebugScreenPrintf("\nBlame the 3rd party software, it cannot possibly be our fault!\n");
	gLog.SaveLog( "crash.log" );
}*/
#endif


#ifdef _PSP
extern "C" int SDL_main( int argc, char *argv[] )
#else
int main( int argc, char *argv[] )
#endif
{
	#ifdef _PSP
	scePowerSetClockFrequency(333, 333, 166);
	#endif
	gLog.SetLogging( true );

	LoadDungeonList();

	Game();
	SaveDungeonList();

	gLog.SaveLog( "gamelog.log" );
	gLog.ClearLog();

	return 0;
}




int GamePrecache()
{
	//gBackgroundMusic = new CMusic( "data/music.mp3" );
	CManagedTexture::Load(&gResourceManager, "stonefloor.bmp");
	CManagedTexture::Load(&gResourceManager, "stonewall.bmp");
	CManagedTexture::Load(&gResourceManager, "marble.bmp");
	CManagedTexture::Load(&gResourceManager, "data/bar.bmp");
	CManagedTexture::Load(&gResourceManager, "data/bat.bmp");
	CManagedTexture::Load(&gResourceManager, "data/chronicles.png", true);
	CManagedTexture::Load(&gResourceManager, "data/font.bmp");
	CManagedTexture::Load(&gResourceManager, "data/fontmask.bmp");
	CManagedTexture::Load(&gResourceManager, "data/gate.bmp");
	CManagedTexture::Load(&gResourceManager, "data/gateMask.bmp");
	CManagedTexture::Load(&gResourceManager, "data/ghost.bmp");
	CManagedTexture::Load(&gResourceManager, "data/gremling.bmp");
	CManagedTexture::Load(&gResourceManager, "data/gremling2.bmp");
	CManagedTexture::Load(&gResourceManager, "data/health.bmp");
	CManagedTexture::Load(&gResourceManager, "data/keyMask2.bmp");
	CManagedTexture::Load(&gResourceManager, "data/keySprite2.bmp");
	CManagedTexture::Load(&gResourceManager, "data/particle.bmp");
	CManagedTexture::Load(&gResourceManager, "data/portal.bmp");
	CManagedTexture::Load(&gResourceManager, "data/shine.bmp");
	CManagedTexture::Load(&gResourceManager, "data/sword.bmp");
	CManagedTexture::Load(&gResourceManager, "data/swordmask.bmp");
	CManagedTexture::Load(&gResourceManager, "data/sword.png", true);
	CManagedWaveSound::Load( &gResourceManager, "data/portalActive.wav" );
	CManagedWaveSound::Load( &gResourceManager, "data/warp.wav" );
	CManagedWaveSound::Load( &gResourceManager, "data/portal.wav" );
	return 1;
}



void GameUnload()
{
	gLevel->Shutdown();
}


void LaunchMenu()
{
	if ( gLevel )
	{
		gLevel->Shutdown();
	}

	gLevelNumber = 4;
	gLevel = new CLevel();
	gLevel->LoadGameMenu();
	gLevel->Init();
	gLevelNumber = 0;

	gEntities.Add( gLevel );
	gEntities.Add( new CMenuInput() );
}



void GameStart()
{
#ifndef _DEBUG
	
	SplashScreen();
#endif

	GamePrecache();

	InitPlayerStats();
//	gBackgroundMusic->Play(0);
	
	LaunchMenu();

	//InitPlayerStats();
	//NextLevel();
	//gCurrentDungeon = gDungeonList.GetFirst()->data;
	//NextLevel();
}




void GameStateChange( int from, int to )
{
	switch ( to )
	{
	case NEXT_LEVEL_STATE:
		NextLevel();
		ChangeGameState( DEFAULT_STATE );
		break;
	case MAIN_MENU_STATE:
		LaunchMenu();
		ChangeGameState( DEFAULT_STATE );
		break;
	case PAUSE_STATE:
		gEntities.Add(new CPause());
		ChangeGameState( DEFAULT_STATE );
		break;
	}
}




void NextLevel()
{
	if ( gLevel )
	{
		gLevel->Shutdown();
		//delete gLevel;
	}

	gLevelNumber++;
	if (gLevelNumber > gCurrentDungeon->numLevels)
	{
		LaunchMenu();
	} else {
		gLevel = new CLevel();
		if (gLevelNumber > gCurrentDungeon->curLevel)
			gCurrentDungeon->curLevel = gLevelNumber;

		gLevel->LoadLevel(gCurrentDungeon->path, gLevelNumber);
		//gLevel->LoadLevel(gCurrentDungeon->path, gLevelNumber);
		gLevel->Init();

		gEntities.Add( gLevel );
		gEntities.Add( new CHud() );
	}
}

