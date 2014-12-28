#include "SDL.h"
#include "timer.h"

#ifdef TIMER_LOGGING
	#include "../engine.h"
	#include "log.h"
#endif

int CTimer::gPausedTicks = 0;

#define		SDL_GetTicks()		(SDL_GetTicks() - gPausedTicks)

CTimer::CTimer()
{
	Clear();
}

CTimer::~CTimer()
{
}
	
void	CTimer::Clear()
{
	lastTick = SDL_GetTicks();
	curTick = lastTick;
	deltaTime = 0.0f;
}

void	CTimer::Step()
{
	lastTick = curTick;
	curTick = SDL_GetTicks();
#ifdef TIMER_LOGGING
	lastDeltaTime = deltaTime;
#endif
	deltaTime = (curTick - lastTick)/1000.0f;
}

float	CTimer::DT()
{
	if ( deltaTime <= MIN_DELTA_TIME )
		deltaTime = MIN_DELTA_TIME;

	return deltaTime;
}


float CTimer::Time()
{
	float dt = (SDL_GetTicks() - curTick)/1000.0f;
	if ( dt <= MIN_DELTA_TIME )
		dt = MIN_DELTA_TIME;

	return dt;
}

CGameTimer::~CGameTimer()
{
}

float CGameTimer::DT()
{
	if ( deltaTime <= MIN_DELTA_TIME )
		deltaTime = MIN_DELTA_TIME;

	if ( deltaTime >= MAX_GAME_DELTA_TIME )
	{
		deltaTime = MAX_GAME_DELTA_TIME;
#ifdef TIMER_LOGGING
		if ( lastDeltaTime < MAX_GAME_DELTA_TIME )
		{
			gLog.LogItem( new CLogMessage("Warning: Frame Rate Dropped Below Minimum Playable FPS") );
		}
#endif
	}

	return deltaTime;
}
