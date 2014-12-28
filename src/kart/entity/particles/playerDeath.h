#ifndef _DEATH_H
#define _DEATH_H

#include "../../../engine/graphics/particle.h"

class CPlayerDeath: public CParticleEmitter
{
public:
	CPlayerDeath( float x, float y, float z );
	~CPlayerDeath();

	void CreateParticle();
	void EffectParticle( CParticle* p, float dt );
	void Logic( float dt );
};


#endif
