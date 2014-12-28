#include "death.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

CDeath::CDeath( float x, float y, float z, float r, float g, float b, float size )
{
	mTimer = 0.0f;
	mEmissionRate = 2500.0f;
	mParticlesLeft = 150;
	mX = x;
	mZ = z;
	mY = y;
	this->r = r;
	this->g = g;
	this->b = b;
	this->size = size;
	mTexture = CManagedTexture::Load( &gResourceManager, "particle.bmp" );
}

CDeath::~CDeath()
{
}

void CDeath::CreateParticle()
{
	float x, y, z;
	x = mX + (rand()%1000/1000.0f) * size - size / 2.0f;
	y = mY + (rand()%1000/1000.0f) * size - size / 2.0f;
	z = mZ + (rand()%1000/1000.0f) * size - size / 2.0f;
	CParticle* p = new CParticle( 0.6f, 
		x, y, z, 0.5f, 0.1f );

	p->SetStartColor( r, g, b, 1.0f );
	p->SetEndColor( r, g, b, 0.0f );

	int rot = rand()%15;
	x = (rand()%100)/20.0f - 2.5f;
	y = 1.0f;
	z = (rand()%100)/20.0f - 2.5f;
	p->SetSpeed( x , y, z );
//	p->SetSpeed( 0.0f, 6.0f, 0.0f );
	mParticles.Add( p );
}

void CDeath::EffectParticle( CParticle* p, float dt )
{
	float x, y, z;
	p->GetSpeed( &x, &y, &z );
	x *= (1.0f + 1.2f * dt);
//	y *= (1.0f + 0.2f * dt);
	z *= (1.0f + 1.2f * dt);

	p->SetSpeed( x, y, z );
//	p->Accelerate( 0.0f, 20.0f * dt, 0.0f );
	//p->SetSpeed( cos( rot ) * 3.0f, 5.0f, sin( rot ) * 3.0f );
}

void CDeath::Logic( float dt )
{
}

