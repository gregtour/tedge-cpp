#ifndef _SPIN_H
#define _SPIN_H

#include "../../../engine/graphics/particle.h"

class CSpin: public CParticleEmitter
{
public:
	CSpin( float x, float y, float z );
	~CSpin();

	void CreateParticle();
	void EffectParticle( CParticle* p, float dt );
	void Logic( float dt );
	float rot;
};


#endif
