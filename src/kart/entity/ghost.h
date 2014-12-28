#ifndef _GHOST_H
#define _GHOST_H

#include "bat.h"

/*
	A "Ghost" Monster
		Ripped off of bat :-P
*/

#include "monster.h"

class CGhost: public CMonster
{
public:
	CGhost( float rotSpeed );
	~CGhost();

	void Update( float dt );
	void Render();
	void RenderAlpha();

	void DeathEffect();
protected:
	void LoadTexture();

	int	mAction;
	float mRotSpeed;

	CWaveSound* mGhostSound;
	CSound* mSound;
};

#endif
