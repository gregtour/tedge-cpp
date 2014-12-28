#ifndef _FOUNTAIN_H
#define _FOUNTAIN_H

#include "../../../engine/graphics/particle.h"

class CFountain: public CParticleEmitter
{
public:
	CFountain( float x, float y, float z );
	~CFountain();

	void CreateParticle();
	void EffectParticle( CParticle* p, float dt );
	void Logic( float dt );

	float iterator;
};


#endif
