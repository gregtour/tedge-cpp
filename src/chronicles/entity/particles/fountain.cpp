#include "fountain.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

CFountain::CFountain( float x, float y, float z )
{
	mTimer = 0.0f;
	mEmissionRate = 100.0f;
	mParticlesLeft = -1;
	mX = x;
	mZ = z;
	mY = y;
	iterator = 0.0f;
	mTexture = CManagedTexture::Load( &gResourceManager, "data/particle.bmp" );
}

CFountain::~CFountain()
{
}

void CFountain::CreateParticle()
{
	CParticle* p = new CParticle( 2.2f, 
		mX, mY, mZ, 0.15f, 1.5f );

	p->SetStartColor( 0.7f, 0.7f, 1.0f, 1.0f );
	p->SetEndColor( 0.9f, 0.9f, 1.0f, 0.0f );
	int rot = rand()%15;
	if ( rot < 12 )
	{
		p->SetSpeed( sin((float)rot)*2.5f, 15.0f + sin(iterator)*3.0f, cos((float)rot)*2.5f );
	} else {
		p->SetSpeed( (rand()%100/200.0f)-0.25f, 17.0f + (rand()%100/50.0f)-1.0f + cos(iterator)*4.5f, (rand()%100/200.0f)-0.25f );
	}
//	p->SetSpeed( 0.0f, 6.0f, 0.0f );
	mParticles.Add( p );
}

void CFountain::EffectParticle( CParticle* p, float dt )
{
	p->Accelerate( 0.0f, -20.0f * dt, 0.0f );
	//p->SetSpeed( cos( rot ) * 3.0f, 5.0f, sin( rot ) * 3.0f );
}

void CFountain::Logic( float dt )
{
	iterator += dt / 3.0f;
}

