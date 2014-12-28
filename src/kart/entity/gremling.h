#ifndef _GREMLING_H
#define _GREMLING_H

#include "monster.h"
#include "../../engine/graphics/model.h"
#define		GREMLING_SCUTTLE_INTENSITY	1.0f
#define		GREMLING_SCUTTLE_SPEED		5.0f

class CGremling: public CMonster
{
public:
	CGremling( float min, float prefered );
	~CGremling();

	void LoadHitSound();
	void DeathEffect();

	void Update( float dt );
	void Render();

protected:
	void LoadTexture();
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
	CModel* mModel;
};

#endif
