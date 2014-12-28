#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include "../../../engine/graphics/particle.h"

class CExplosion: public CParticleEmitter
{
public:
	CExplosion( SVector3 pos, float r, float g, float b, float size, int num );
	~CExplosion();

	void CreateParticle();
	void EffectParticle( CParticle* p, float dt );
	void Logic( float dt );
	float r, g, b, size;
};


#endif
