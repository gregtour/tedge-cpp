#ifndef _BAT_H
#define _BAT_H

#define BAT_OBJ_TYPE	7

/*
	A "Bat" Monster
		3D Black Cube, flys around in the air, dives at player then runs away

*/

#include "monster.h"
#include "particles/fire.h"

#define BAT_ALTITUDE	15.0f
#define BAT_SPEED		35.0f
#define BAT_DIVE_SPEED	70.0f
#define BAT_CLIMB_SPEED	20.0f

#define BAT_DIVE_DISTANCE	14.0f
#define BAT_CLIMB_DISTANCE	25.0f

class CBat: public CMonster
{
public:
	CBat( float rotSpeed, float flapHeight, float flapSpeed, float flapIntensity );
	~CBat();

	void Update( float dt );
	void Render();
	void RenderAlpha();

	void DeathEffect();
protected:
	void LoadTexture();

	int	mAction;

	float mRotSpeed;

	float mFlap;
	float mFlapHeight;
	float mFlapSpeed;
	float mFlapIntensity;
	
	float mCruiseHeight;

	CFire* mEffect;

	CWaveSound* mBatSound;
	CSound* mSound;
};

#endif
