#include "explosion.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

CExplosion::CExplosion( SVector3 pos, float r, float g, float b, float size, int num )
{
	mTimer = 0.0f;
	mEmissionRate = 2500.0f;
	mParticlesLeft = num;	// 20
	mX = pos.x;
	mZ = pos.z;
	mY = pos.y;
	this->r = r;
	this->g = g;
	this->b = b;
	this->size = size;
	//mTexture = CManagedTexture::Load( &gResourceManager, "data/particle.bmp" );
	mTexture = CManagedTexture::Load( &gResourceManager, "thick.png" );
	//mTexture = CManagedTexture::Load( &gResourceManager, "plus.png" );
}

CExplosion::~CExplosion()
{
}

void CExplosion::CreateParticle()
{
	float x, y, z;
	x = mX + (rand()%1000/1000.0f) * size - size / 2.0f;
	y = mY + (rand()%1000/1000.0f) * size / 3.0f - size / 6.0f;
	z = mZ + (rand()%1000/1000.0f) * size - size / 2.0f;
	CParticle* p = new CParticle( 0.9f, 
		x, y, z, 0.5f, 0.1f );

	p->SetStartColor( r, g, b, 1.0f );
	p->SetEndColor( r, g, b, 0.0f );

	int rot = rand()%15;
	//x = (rand()%1000/1000.0f) * size - size / 2.0f;
	y = (rand()%1000/1000.0f) * size / 2.0f - size / 4.0f;
	//z = (rand()%1000/1000.0f) * size - size / 2.0f;
	x = 0.0f;
	z = 0.0f;
	p->SetSpeed( x , y, z );
//	p->SetSpeed( 0.0f, 6.0f, 0.0f );
	mParticles.Add( p );
}

void CExplosion::EffectParticle( CParticle* p, float dt )
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

void CExplosion::Logic( float dt )
{
}

