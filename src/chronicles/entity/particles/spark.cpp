#include "spark.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

CSpark::CSpark( float x, float y, float z, float r, float g, float b, float size, int num, float time, float gravity, std::string texture )
{
	mTimer = 0.0f;
	mEmissionRate = 5000.0f;
	mParticlesLeft = num;
	mX = x;
	mZ = z;
	mY = y;
	this->r = r;
	this->g = g;
	this->b = b;
	this->size = size;
	this->texture = texture;
	this->num = num;
	this->time = time;
	this->gravity = gravity;
}

CSpark::~CSpark()
{
}

void CSpark::CreateParticle()
{
	CParticle* p = new CParticle( texture, time, 
		mX, mY, mZ, size/10.0f, size/4.0f );

	p->SetStartColor( r, g, b, 1.0f );
	p->SetEndColor( r, g, b, 0.0f );

	float rot = rand()%1000/200.0f;
	float roty = rand()%1000/200.0f;
	float x, y, z;
	x = sin( rot ) * size / time;
	y = sin( roty ) * size / time;
	z = cos( rot ) * size / time;
	p->SetSpeed( x , y, z );
//	p->SetSpeed( 0.0f, 6.0f, 0.0f );
	mParticles.Add( p );
}

void CSpark::EffectParticle( CParticle* p, float dt )
{
//	float x, y, z;
//	p->GetSpeed( &x, &y, &z );
//	x *= (1.0f + 1.2f * dt);
//	y *= (1.0f + 0.2f * dt);
//	z *= (1.0f + 1.2f * dt);

//	p->SetSpeed( x, y, z );

	p->Accelerate( 0.0f, -gravity * dt, 0.0f );
	//p->SetSpeed( cos( rot ) * 3.0f, 5.0f, sin( rot ) * 3.0f );
}

void CSpark::Logic( float dt )
{
}

