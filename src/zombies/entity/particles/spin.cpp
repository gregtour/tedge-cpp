#include "spin.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

CSpin::CSpin( float x, float y, float z )
{
	mTimer = 0.0f;
	mEmissionRate = 30.0f;
	mParticlesLeft = 45;
	mX = x;
	mZ = z;
	mY = y;
	rot = 0.0f;
	mTexture = CManagedTexture::Load( &gResourceManager, "data/particle.bmp" );
}

CSpin::~CSpin()
{
}

void CSpin::CreateParticle()
{
	CParticle* p = new CParticle( 2.2f, 
		cos( rot ) * 2.0f + mX, mY, sin( rot ) * 2.0f + mZ, 1.5f, 0.5f );

	p->SetStartColor( 0.0f, 0.0f, 1.0f, 1.0f );
	p->SetEndColor( 1.0f, 0.0f, 0.0f, 0.0f );
//	p->SetSpeed( (rand()%100)/10.0f - 5.0f, 20.0f, (rand()%100)/10.0f - 5.0f );
	p->SetSpeed( 0.0f, 6.0f, 0.0f );
	mParticles.Add( p );
}

void CSpin::EffectParticle( CParticle* p, float dt )
{
//	p->Accelerate( 0.0f, -20.0f * dt, 0.0f );
	//p->SetSpeed( cos( rot ) * 3.0f, 5.0f, sin( rot ) * 3.0f );
}

void CSpin::Logic( float dt )
{
	rot += dt * 8.0f;
}

