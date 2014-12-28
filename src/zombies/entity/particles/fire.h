#ifndef _FIRE_H
#define _FIRE_H

#include "../../../engine/graphics/particle.h"

class CFire: public CParticleEmitter
{
public:
	CFire( float x, float y, float z );
	~CFire();

	void CreateParticle();
	void EffectParticle( CParticle* p, float dt );
	void Logic( float dt );

	void SetGravity( bool gravity );

private:
	bool mGravity;
};


#endif
