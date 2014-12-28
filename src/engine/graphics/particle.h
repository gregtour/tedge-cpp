#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "../common/list.h"
#include "../entity/entity.h"
#include "../entity/graphicalObj.h"
#include "texture.h"
#define MAX_PARTICLES		300

//class CParticle: public CEntity, public IGraphicalObj
class CParticle
{
public:
	CParticle( float life, float x, float y, float z, float startSize, float endSize );
	~CParticle();

	void SetStartColor( float startR, float startG, float startB, float startA );
	void SetEndColor( float r, float g, float b, float a );
	void SetSpeed( float xSpeed, float ySpeed, float zSpeed );
	void GetSpeed( float* xSpeed, float* ySpeed, float* zSpeed );

	void Update( float dt );
	void RenderAlpha(float angle);
	bool IsAlive();
	void Accelerate( float xSpeed, float ySpeed, float zSpeed );

//private:
	float mLife;
	float mAge;
	float mStartR, mStartG, mStartB, mStartA, mStartSize;
	float mEndR, mEndG, mEndB, mEndA, mEndSize;
	float mX, mY, mZ;
	float mXSpeed, mYSpeed, mZSpeed;
};


class I3DVERTBUF;

/* Virtual Class CParticleEmitter	*/
class CParticleEmitter: public CEntity, public IGraphicalObj
{
public:
	CParticleEmitter();
	CParticleEmitter( float rate, int particles );
	virtual ~CParticleEmitter();

	void SetEmissionRate( float rate );
	void SetParticlesLeft( int particles );
	void SetPosition( float x, float y, float z );
    void SetPosition( SVector3 p ) { SetPosition(p.x, p.y, p.z); }

	int GetParticlesLeft();

	virtual void CreateParticle()=0;
	virtual void EffectParticle( CParticle* p, float dt )=0;
	virtual void Logic( float dt );
	virtual void Update( float dt );
	void RenderAlpha();

	static I3DVERTBUF*	gParticleVerts;

protected:
	int mParticlesLeft;		// Total # of particles to emmit over time, < 0 means infinite
	float mEmissionRate;	// Particles per Second
	float mTimer;
	float mX, mY, mZ;
	CLinkedList<CParticle> mParticles;
	CTexture*	mTexture;
};


#endif
