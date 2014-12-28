#include "gem.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

CGem::CGem( float x, float y, float z )
{
	mTimer = 0.0f;
	mEmissionRate = 80.0f;
	mParticlesLeft = -1;
	mX = x;
	mZ = z;
	mY = y;
	mTexture = CManagedTexture::Load( &gResourceManager, "data/sparkle.bmp" );
}

CGem::~CGem()
{
}

void CGem::CreateParticle()
{
	float x, y, z;
	float rot = (rand()%1000)/100.0f;
	float dis = (rand()%1200)/1000.0f;
	y = (rand()%2000/1000.0f) - 1.0f;
	dis *=  (1.0f - fabs( y / 2.5f )) * 0.5f;
	x = mX + sin(rot) * dis;
	y += mY;
	z = mZ + cos(rot) * dis;
	CParticle* p = new CParticle( 1.25f, 
		x, y, z, 0.1f, 0.8f );

	p->SetEndColor( 0.0f, 0.0f, 1.0f, 0.0f );

/*	if ( y < 3.0f )
	{
		p->SetStartColor( 0.3f, 0.7f, 0.3f, 1.0f );
	} else {
		p->SetStartColor( 0.3f, 0.3f, 0.7f, 1.0f );
	}*/
	p->SetEndColor( 0.0f, 0.0f, 1.0f, 0.0f );
/*
	int rot = rand()%15;
	x = (rand()%100)/20.0f - 2.5f;
	y = 1.0f;
	z = (rand()%100)/20.0f - 2.5f;*/
	p->SetSpeed( 0.0f, 1.5f - (rand()%900/300.0f), 0.0f );
//	p->SetSpeed( 0.0f, 6.0f, 0.0f );
	mParticles.Add( p );
}

void CGem::EffectParticle( CParticle* p, float dt )
{/*
	float x, y, z;
	p->GetSpeed( &x, &y, &z );
	x *= (1.0f + 1.2f * dt);
//	y *= (1.0f + 0.2f * dt);
	z *= (1.0f + 1.2f * dt);

	p->SetSpeed( x, y, z );
//	p->Accelerate( 0.0f, 20.0f * dt, 0.0f );
	//p->SetSpeed( cos( rot ) * 3.0f, 5.0f, sin( rot ) * 3.0f );*/
}

void CGem::Logic( float dt )
{
}

