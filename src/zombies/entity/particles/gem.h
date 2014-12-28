#ifndef _GEM_H
#define _GEM_H

#include "../../../engine/graphics/particle.h"

class CGem: public CParticleEmitter
{
public:
	CGem( float x, float y, float z );
	~CGem();

	void CreateParticle();
	void EffectParticle( CParticle* p, float dt );
	void Logic( float dt );
};


#endif
