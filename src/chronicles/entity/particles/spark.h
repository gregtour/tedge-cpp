#ifndef _SPARK_H
#define _SPARK_H

#include "../../../engine/graphics/particle.h"

class CSpark: public CParticleEmitter
{
public:
	CSpark( float x, float y, float z, float r, float g, float b, float size, int num, float time, float gravity, std::string texture );
	~CSpark();

	void CreateParticle();
	void EffectParticle( CParticle* p, float dt );
	void Logic( float dt );

	float r, g, b, size, time, gravity;
	int num;
	std::string texture;
};


#endif
