#include "particle.h"
#include "graphics.h"
#include "../engine.h"
#include "buffer/I3dVertBuf.h"

#include <math.h>

#ifndef NULL
#define NULL	0
#endif

I3DVERTBUF*	CParticleEmitter::gParticleVerts = NULL;


/*	Particle Methods	*/
CParticle::CParticle( float life, float x, float y, float z, float startSize, float endSize )
{
	mLife = life;
	mAge = 0.0f;
	mX = x;
	mY = y;
	mZ = z;
	mStartSize = startSize;
	mEndSize = endSize;
	SetStartColor( 1.0f, 1.0f, 1.0f, 1.0f );
	SetEndColor( 1.0f, 1.0f, 1.0f, 0.0f );
	SetSpeed( 0.0f, 0.0f, 0.0f );
}

CParticle::~CParticle()
{
}

void CParticle::GetSpeed( float* xSpeed, float* ySpeed, float* zSpeed )
{
	*xSpeed = mXSpeed;
	*ySpeed = mYSpeed;
	*zSpeed = mZSpeed;
}

void CParticle::SetStartColor( float startR, float startG, float startB, float startA )
{
	mStartR = startR;
	mStartG = startG;
	mStartB = startB;
	mStartA = startA;
}


void CParticle::SetEndColor( float r, float g, float b, float a )
{
	mEndR = r;
	mEndG = g;
	mEndB = b;
	mEndA = a;
}


void CParticle::SetSpeed( float xSpeed, float ySpeed, float zSpeed )
{
	mXSpeed = xSpeed;
	mYSpeed = ySpeed;
	mZSpeed = zSpeed;
}


void CParticle::Update( float dt )
{
	if ( IsAlive() )
	{
		mAge += dt;
		mX += mXSpeed * dt;
		mY += mYSpeed * dt;
		mZ += mZSpeed * dt;
	}
}


void CParticle::RenderAlpha(float angle)
{
	if ( IsAlive() )
	{

		glPushMatrix();

		glTranslatef( mX, mY, mZ );

		glRotatef( angle, 0.0f, -1.0f, 0.0f );

		float r = (mStartR + (mEndR - mStartR) * (mAge / mLife) );
		float g = (mStartG + (mEndG - mStartG) * (mAge / mLife) );
		float b = (mStartB + (mEndB - mStartB) * (mAge / mLife) );
		float a = (mStartA + (mEndA - mStartA) * (mAge / mLife) );
		float size = (mStartSize + (mEndSize - mStartSize) * (mAge / mLife) );

		glColor4f( r, g, b, a );

		glBegin( GL_TRIANGLES );
			glTexCoord2f( 1.0f, 0.0f );
			glVertex3f( -size, -size, 0.0f );
			glTexCoord2f( 0.01f, 0.0f );
			glVertex3f( -size, size, 0.0f );
			glTexCoord2f( 0.01f, 1.0f );
			glVertex3f( size, size, 0.0f );

			glTexCoord2f( 1.0f, 0.0f );
			glVertex3f( -size, -size, 0.0f );
			glTexCoord2f( 0.01f, 1.0f );
			glVertex3f( size, size, 0.0f );
			glTexCoord2f( 1.0f, 1.0f );
			glVertex3f( size, -size, 0.0f );
		glEnd();

		glPopMatrix();
	}
}

bool CParticle::IsAlive()
{ return ( mAge < mLife ); }


void CParticle::Accelerate( float xSpeed, float ySpeed, float zSpeed )
{
	mXSpeed += xSpeed;
	mYSpeed += ySpeed;
	mZSpeed += zSpeed;
}





/*	Particle Emitter Methods	*/

CParticleEmitter::CParticleEmitter()
{
	if (!gParticleVerts)
		gParticleVerts = new I3DVERTBUF(true, I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_DIFFUSE | I3D_VC_UV1, MAX_PARTICLES*2*3);

	mTimer = 0.0f;
	mEmissionRate = 0.0001f;
	mParticlesLeft = -1;
}

CParticleEmitter::CParticleEmitter( float rate, int particles )
{
	mTimer = 0.0f;
	SetEmissionRate( rate );
	SetParticlesLeft( particles );
}

CParticleEmitter::~CParticleEmitter()
{
	/*	Delete Remaining Particles	*/
	CListEntry<CParticle>* particle = mParticles.GetFirst();
	while ( particle )
	{
		delete particle->data;
		mParticles.RemoveEntry( particle );
		particle = mParticles.GetFirst();
	}
}

void CParticleEmitter::SetEmissionRate( float rate )
{ mEmissionRate = rate; }


void CParticleEmitter::SetParticlesLeft( int particles )
{ mParticlesLeft = particles; }

int CParticleEmitter::GetParticlesLeft()
{ return mParticlesLeft; }



void CParticleEmitter::SetPosition( float x, float y, float z )
{
	mX = x;
	mY = y;
	mZ = z;
}


void CParticleEmitter::Update( float dt )
{
	{
		float x, y, z;
		gCamera->GetPosition( &x, &y, &z );
		if (((x-mX)*(x-mX) + (z-mZ)*(z-mZ) + (y-mY)*(y-mY)) > VIEW_DISTANCE_SQUARED)
			return;
	}

	/*	Create New Particles	*/
	mTimer += dt;
	while ( mTimer > ( 1.0f / mEmissionRate ) && mParticlesLeft != 0 && mParticles.length < MAX_PARTICLES)
	{
		CreateParticle();
		mTimer -= ( 1.0f / mEmissionRate );
		if ( mParticlesLeft > 0 )
			mParticlesLeft--;
	}

	/*	Update Old Particles	*/
	CListEntry<CParticle>* particle = mParticles.GetFirst();
	while ( particle )
	{
		particle->data->Update( dt );
		EffectParticle( particle->data, dt );	//	Effects like gravity

		/*	Delete Dead Particles	*/
		if ( !particle->data->IsAlive() )
		{
			delete particle->data;
			CListEntry<CParticle>* nextParticle = particle->GetNext();
			mParticles.RemoveEntry( particle );
			particle = nextParticle;
		}
		else
			particle = particle->GetNext();
	}

	Logic( dt );

	/*	If there are no particles, and no particles left, delete this emitter	*/
	if ( mParticles.length == 0 && mParticlesLeft == 0 )
	{
		/*	Set the ListEntry for it to NULL so it will be removed automatically	*/
		//gEntities.FindEntry( this )->data = NULL;
		//delete this;
		Destroy();
	}
}

void CParticleEmitter::Logic( float dt )
{
	return;
}

void CParticleEmitter::RenderAlpha()
{
	if (mParticles.length == 0)
		return;

	SVector3 cPos;
	gCamera->GetPosition( &cPos.x, &cPos.y, &cPos.z );
	if ( ((cPos.x-mX)*(cPos.x-mX) + (cPos.z-mZ)*(cPos.z-mZ) + (cPos.y - mY)*(cPos.y-mY)) > VIEW_DISTANCE_SQUARED )
		return;
	
	SVector3 normal;
	SVector3 position( mX, mY, mZ );
	normal = position - cPos;
	normal.y = 0.0f;
	//float ang = atan2(billboardAngle.z, billboardAngle.x) * DEGREES + 90.0f;
	normal.Normalize();

	SVector3 up(0.0f, 1.0f, 0.0f);
	SVector3 right = up.CrossProduct(normal);

	//right = right * 0.5f;
	//up = up * 0.5f;

	// Lock all
	I3DVERTBUF* vertBuf = gParticleVerts;
	vertBuf->Lock(MAX_PARTICLES*6);
	//vertBuf->Lock(mParticles.length*6);

	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	//glEnable( GL_TEXTURE_2D );
	glDisable( GL_CULL_FACE );
//	glDisable( GL_DEPTH_TEST );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//glBlendFunc( GL_ONE, GL_ZERO );
	glDepthMask( GL_FALSE );

	mTexture->Bind();

	CListEntry<CParticle>* particle = mParticles.GetFirst();
	while ( particle )
	{
		CParticle* p = particle->data;
		float r = (p->mStartR + (p->mEndR - p->mStartR) * (p->mAge / p->mLife) );
		float g = (p->mStartG + (p->mEndG - p->mStartG) * (p->mAge / p->mLife) );
		float b = (p->mStartB + (p->mEndB - p->mStartB) * (p->mAge / p->mLife) );
		float a = (p->mStartA + (p->mEndA - p->mStartA) * (p->mAge / p->mLife) );
		float size = (p->mStartSize + (p->mEndSize - p->mStartSize) * (p->mAge / p->mLife) ) * 2.0f;

		SVector3 p1 = SVector3(p->mX, p->mY, p->mZ) + right *  0.5f * size + up *  0.5f * size;
		SVector3 p2 = SVector3(p->mX, p->mY, p->mZ) + right * -0.5f * size + up *  0.5f * size;
		SVector3 p3 = SVector3(p->mX, p->mY, p->mZ) + right *  0.5f * size + up * -0.5f * size;
		SVector3 p4 = SVector3(p->mX, p->mY, p->mZ) + right * -0.5f * size + up * -0.5f * size;
/*
#define vertBuf->SetColor	glColor4f
#define vertBuf->SetNormal	glNormal3f
#define vertBuf->SetUV0		glTexCoord2f
#define vertBuf->SetPos((r))	glVertex3f((r)->x, (r)->y, (r)->z)
#define vertBuf->NextVert();*/
//glBegin(GL_TRIANGLES);
/*		glColor4f(r, g, b, a);
		glNormal3f(normal.x, normal.y, normal.z);
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3fv((float*)&p4);

		glColor4f(r, g, b, a);
		glNormal3f(normal.x, normal.y, normal.z);
		glTexCoord2f( 0.01f, 0.0f );
		glVertex3fv((float*)&p2);

		glColor4f(r, g, b, a);
		glNormal3f(normal.x, normal.y, normal.z);
		glTexCoord2f( 0.01f, 1.0f );
		glVertex3fv((float*)&p1);

		glColor4f(r, g, b, a);
		glNormal3f(normal.x, normal.y, normal.z);
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3fv((float*)&p4);

		glColor4f(r, g, b, a);
		glNormal3f(normal.x, normal.y, normal.z);
		glTexCoord2f( 0.01f, 1.0f );
		glVertex3fv((float*)&p1);
	
		glColor4f(r, g, b, a);
		glNormal3f(normal.x, normal.y, normal.z);
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3fv((float*)&p3);
*/


		vertBuf->SetColor(a, r, g, b);
		vertBuf->SetNormal(normal.x, normal.y, normal.z);
		vertBuf->SetUV0( 1.0f, 0.0f );
		vertBuf->SetPos((VECTOR3*)&p4);
		vertBuf->NextVert();

		vertBuf->SetColor(a, r, g, b);
		vertBuf->SetNormal(normal.x, normal.y, normal.z);
		vertBuf->SetUV0( 0.01f, 0.0f );
		vertBuf->SetPos((VECTOR3*)&p2);
		vertBuf->NextVert();

		vertBuf->SetColor(a, r, g, b);
		vertBuf->SetNormal(normal.x, normal.y, normal.z);
		vertBuf->SetUV0( 0.01f, 1.0f );
		vertBuf->SetPos((VECTOR3*)&p1);
		vertBuf->NextVert();

		vertBuf->SetColor(a, r, g, b);
		vertBuf->SetNormal(normal.x, normal.y, normal.z);
		vertBuf->SetUV0( 1.0f, 0.0f );
		vertBuf->SetPos((VECTOR3*)&p4);
		vertBuf->NextVert();

		vertBuf->SetColor(a, r, g, b);
		vertBuf->SetNormal(normal.x, normal.y, normal.z);
		vertBuf->SetUV0( 0.01f, 1.0f );
		vertBuf->SetPos((VECTOR3*)&p1);
		vertBuf->NextVert();
	
		vertBuf->SetColor(a, r, g, b);
		vertBuf->SetNormal(normal.x, normal.y, normal.z);
		vertBuf->SetUV0( 1.0f, 1.0f );
		vertBuf->SetPos((VECTOR3*)&p3);
		vertBuf->NextVert();
//glEnd();

		//vertBuf->SetUV0((p.x + p.z)/10.0f, (p.y + p.z - p.x)/10.0f);
		particle = particle->GetNext();
	}
	vertBuf->Unlock();

	


	/*
	CListEntry<CParticle>* particle = mParticles.GetFirst();
	while ( particle )
	{
		particle->data->RenderAlpha(ang);
		particle = particle->GetNext();
	}*/
	I3dSelectBuffers(gParticleVerts, NULL);
	I3dRenderPrim(1, mParticles.length * 2, mParticles.length * 6);

	mTexture->Unbind();

	//glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	//glEnable( GL_LIGHTING );
	glEnable( GL_CULL_FACE );
	glDepthMask( GL_TRUE );
//	glEnable( GL_DEPTH_TEST );
}

