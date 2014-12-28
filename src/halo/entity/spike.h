#ifndef _SPIKE_H
#define _SPIKE_H

#include "monster.h"

#define SPIKE_OBJ_TYPE			14

class CSpike: public CMonster
{
public:
	CSpike( float frequency );
	~CSpike();

	void CreatePhysical( CPhysics* world, SVector3* p1, SVector3* p2 );
	void Update( float dt );
	void Render();
	bool IsUp();

	SVector3 GetPosition();

	void LoadTexture();

private:
	CWaveSound* mSound;
	float mFrequency;
	CTimer mTime;
	bool mUp;
	float mY;

	CPTriangle*	mTri1;
	CPTriangle*	mTri2;	
	SVector3	p1;
	SVector3	p2;

	GLUquadricObj* mQuadric;
};

#endif
