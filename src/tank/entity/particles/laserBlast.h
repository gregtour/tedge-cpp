#ifndef _LASERBLAST_H
#define _LASERBLAST_H

#include "../../../engine/graphics/particle.h"

class CLaserBlast: public CParticleEmitter
{
public:
	CLaserBlast( SVector3 pos, float r, float g, float b, float size, int num );
	~CLaserBlast();

	void CreateParticle();
	void EffectParticle( CParticle* p, float dt );
	void Logic( float dt );
	float r, g, b, size;
};


#endif
