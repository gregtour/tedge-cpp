#include "smoke.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

CSmoke::CSmoke( float x, float y, float z )
{
	mTimer = 0.0f;
	mEmissionRate = 1000.0f;
	mParticlesLeft = -1;
	mX = x;
	mZ = z;
	mY = y;
	mTexture = CManagedTexture::Load( &gResourceManager, "data/particle.bmp" );
}

CSmoke::~CSmoke()
{
}

void CSmoke::CreateParticle()
{
	CParticle* p = new CParticle( 0.9f, 
		mX + (rand()%1000)/40.0f - 12.5f, (rand()%100)/50.0f + mY, mZ + (rand()%1000)/40.0f - 12.5f, 2.0f, 0.8f );

	p->SetStartColor( 0.0f, 0.0f, 0.0f, 1.0f );
	p->SetEndColor( 0.5f, 0.5f, 0.8f, 0.0f );
	float x, y, z;
	x = (rand()%100)/20.0f - 2.5f;
	y = 0.0f;
	z = (rand()%100)/20.0f - 2.5f;
	p->SetSpeed( x , y, z );
//	p->SetSpeed( 0.0f, 6.0f, 0.0f );
	mParticles.Add( p );
}

void CSmoke::EffectParticle( CParticle* p, float dt )
{
	/*
	float x, y, z;
	p->GetSpeed( &x, &y, &z );
	x *= (1.0f - 1.75f * dt);
//	y *= (1.0f + 0.2f * dt);
	z *= (1.0f - 1.75f * dt);
*/
//	p->SetSpeed( x, y, z );
//	p->Accelerate( 0.0f, 20.0f * dt, 0.0f );
	//p->SetSpeed( cos( rot ) * 3.0f, 5.0f, sin( rot ) * 3.0f );
}

void CSmoke::Logic( float dt )
{
}

/*
void CSmoke::RenderAlpha()
{
	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_CULL_FACE );
//	glDisable( GL_DEPTH_TEST );
//	glBlendEquation( GL_FUNC_SUBTRACT );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glDepthMask( GL_FALSE );

	CListEntry<CParticle>* particle = mParticles.GetFirst();
	while ( particle )
	{
		particle->data->RenderAlpha();
		particle = particle->GetNext();
	}

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glEnable( GL_LIGHTING );
	glEnable( GL_CULL_FACE );
	glDepthMask( GL_TRUE );
//	glBlendEquation( GL_FUNC_ADD );
//	glEnable( GL_DEPTH_TEST );
}
*/