#ifndef _DEATH_H
#define _DEATH_H

#include "../../../engine/graphics/particle.h"

class CDeath: public CParticleEmitter
{
public:
	CDeath( float x, float y, float z, float r, float g, float b, float size );
	~CDeath();

	void CreateParticle();
	void EffectParticle( CParticle* p, float dt );
	void Logic( float dt );
	float r, g, b, size;
};


#endif
