/****************************
 * lift: outrunner
 *	by team duck
 *	Feb 21 09
**/   

#include "SDL.h"
#include "../engine/engine.h"
#include "../engine/sound/music.h"

#include "game/game.h"
#include "entity/level.h"
#include "entity/player.h"

CPlayer* gPlayer = NULL;
CLevel* gLevel = NULL;

const char* gameTitle = "Lift: outrunner";

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
	gLevel->Shutdown();
}




void GameStart()
{
	NextLevel();
}




void GameStateChange( int from, int to )
{
	/*switch ( to )
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



void NextLevel()
{

	if ( gLevel )
		gLevel->Shutdown();

	gLevel = new CLevel();

	gLevel->FakeLoad();
	gLevel->Init();

	gEntities.Add( gLevel );
}

