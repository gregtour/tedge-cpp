#include "fire.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

CFire::CFire( float x, float y, float z )
{
	mTimer = 0.0f;
	mEmissionRate = 50.0f;
	mParticlesLeft = -1;
	mGravity = false;
	mX = x;
	mZ = z;
	mY = y;
	mTexture = CManagedTexture::Load( &gResourceManager, "data/particle.bmp" );
}

CFire::~CFire()
{
}

void CFire::SetGravity( bool gravity )
{
	mGravity = gravity;
}

void CFire::CreateParticle()
{
	CParticle* p = new CParticle( 0.7f, 
		mX + (rand()%100)/100.0f - 0.5f, mY, mZ + (rand()%100)/100.0f - 0.5f, (rand()%120)/100.0f + 0.3f, 0.1f );

	p->SetStartColor( 1.0f, 1.0f, 0.7f, 1.0f );
	p->SetEndColor( 1.0f, 0.0f, 0.0f, 0.0f );
	float x, y, z;
	x = (rand()%100)/20.0f - 2.5f;
	if ( mGravity )
		y = 0.0f;
	else
		y = 10.0f;
	z = (rand()%100)/20.0f - 2.5f;
	p->SetSpeed( x , y, z );
//	p->SetSpeed( 0.0f, 6.0f, 0.0f );
	mParticles.Add( p );
}

void CFire::EffectParticle( CParticle* p, float dt )
{
	float x, y, z;
	p->GetSpeed( &x, &y, &z );
	x *= (1.0f - 1.75f * dt);
//	y *= (1.0f + 0.2f * dt);
	z *= (1.0f - 1.75f * dt);

	p->SetSpeed( x, y, z );
//	p->Accelerate( 0.0f, 20.0f * dt, 0.0f );
	//p->SetSpeed( cos( rot ) * 3.0f, 5.0f, sin( rot ) * 3.0f );
}

void CFire::Logic( float dt )
{
}

