#ifndef _BOSS_H
#define _BOSS_H

#include "monster.h"

#define		BOSS_SCUTTLE_INTENSITY	0.5f
#define		BOSS_SCUTTLE_SPEED		2.0f

extern int gNumBosses;

class CBoss: public CMonster
{
public:
	CBoss( float min, float prefered );
	~CBoss();
	
	int AttackStrength();

	void LoadHitSound();
	void DeathEffect();

	void Update( float dt );
	void Render();

protected:
	void LoadTexture();

	CTexture* mFace;
	CTimer mFaceTimer;
	//	If distance < minDistance, set running to true
	//  if running
	//		ScuttleAway!
	//		if distance > prefered, set running to false
	//	else
	//		run in circles
	//
	bool mRunning;
	float mMinDistanceToPlayer;
	float mPreferedDistance;
	float mScuttle;

	CWaveSound* mGremSound;
	CSound* mSound;
};

#endif
