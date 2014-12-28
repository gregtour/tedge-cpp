#ifndef _TIMER_H
#define _TIMER_H

#define MINIMUM_PLAYABLE_FPS	20
#define MAX_GAME_DELTA_TIME		(1.0f/MINIMUM_PLAYABLE_FPS)
#define MIN_DELTA_TIME			0.000001f

//	Timer Logging Logs a Warning if the Framerate Drops Below MINIMUM_PLAYABLE_FPS
#define TIMER_LOGGING

class CTimer
{
public:
	CTimer();
	virtual ~CTimer();
	
	void	Clear();	// Clear the timer
	void	Step();		// Steps the timer and refreshes Delta Time
	virtual float	DT();		// Returns time between last Step() and the step before
	float	Time();		// Returns time since last Step() without stepping

	static int gPausedTicks;

protected:
	long lastTick;
	long curTick;
	float deltaTime;

#ifdef TIMER_LOGGING
	float lastDeltaTime;
#endif
};

class CGameTimer: public CTimer
{
public:
	virtual ~CGameTimer();
	float DT();
};

#endif
