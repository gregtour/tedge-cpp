#ifndef _SMOKE_H
#define _SMOKE_H

#include "../../../engine/graphics/particle.h"

class CSmoke: public CParticleEmitter
{
public:
	CSmoke( float x, float y, float z );
	~CSmoke();

//	void RenderAlpha();
	void CreateParticle();
	void EffectParticle( CParticle* p, float dt );
	void Logic( float dt );
};


#endif
