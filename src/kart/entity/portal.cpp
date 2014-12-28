#include "portal.h"
#include "../main.h"
#include <math.h>

#define	SOUND_CUTOFF_DISTANCE	2500.0f
#define SOUND_REACH				16

CPortal::CPortal()
{
#ifdef LOW_RESOLUTION_TEXTURES
	mPortalTexture = CManagedTexture::Load( &gResourceManager, "data/low/portal.bmp" );
	mShineTexture = CManagedTexture::Load( &gResourceManager, "data/low/shine.bmp" );
#else
	mPortalTexture = CManagedTexture::Load( &gResourceManager, "data/portal.bmp" );
	mShineTexture = CManagedTexture::Load( &gResourceManager, "data/shine.bmp" );
#endif
	mRotation = 0.0f;
	mShine = 0.0f;
	mGlowing = false;
	mGlow = 0.0f;
	mSound = NULL;

	mWarpSound = CManagedWaveSound::Load( &gResourceManager, "data/warp.wav" );
	mPortalSound = CManagedWaveSound::Load( &gResourceManager, "data/portal.wav" );
}

CPortal::~CPortal()
{
	if ( mSound )
	{
		if ( mSound->IsPlaying() )
			mSound->Stop();
		delete mSound;
	}
	CManagedWaveSound::Unload( &gResourceManager, mWarpSound );
	CManagedWaveSound::Unload( &gResourceManager, mPortalSound );
	CManagedTexture::Unload( &gResourceManager, mPortalTexture );
	CManagedTexture::Unload( &gResourceManager, mShineTexture );
}

void CPortal::Glow()
{
	if ( !mGlowing )
	{
		mWarpSound->Play( 0 );
		mGlowing = true;
	}
}

void CPortal::Update( float dt )
{
	mRotation += dt / 2;
	mShine += dt;
	if ( mGlowing )
		mGlow += dt * 15.0f;

	// Object Sound
	{
		float mX = mObject->GetPosition().x;
		float mZ = mObject->GetPosition().y;
		float x, y, z;
		gCamera->GetPosition( &x, &y, &z );
		float distance = (x-mX)*(x-mX) + (z-mZ)*(z-mZ);

		if ( distance < SOUND_CUTOFF_DISTANCE )
		{
			distance = sqrt( distance );

			if ( !mSound )
			{
				mSound = new CSound( mPortalSound->Play( -1 ) );
			}

			int intDistance = (int)distance;
			if ( intDistance <= 0 )
				intDistance = 1;

			int volume = 128 / intDistance * SOUND_REACH;
			if ( volume <= 0 )
				volume = 1;
			if ( volume > 128 )
				volume = 128;
			
			mSound->SetVolume( volume );

		}
		else	// Out of range
		{
			if ( mSound )
			{
				if ( mSound->IsPlaying() )
					mSound->Stop();

				delete mSound;
				mSound = NULL;
			}
		}
	}

}


void CPortal::RenderAlpha()
{

	SVector3 position = mObject->GetPosition();
	float size = mObject->GetSphere().radius * 2.0f;
	//float size = ((CPCircle*)mObject)->GetRadius() * 2.0f;
	
	glTranslatef( position.x, position.y, position.z );

	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	//glEnable( GL_TEXTURE_2D );
	glDisable( GL_CULL_FACE );
	glDisable( GL_FOG );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE );

	glPushMatrix();

	glRotatef( mRotation * DEGREES, 0.0f, 1.0f, 0.0f );
	mPortalTexture->Bind();

	glTranslatef(0.0f, 0.25f, 0.0f);

	glBegin( GL_TRIANGLES );
		glColor4f( 1.0f, 1.0f, 1.0f, 0.7f );

		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -size, 0.0f, -size );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( -size, 0.0f, size );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( size, 0.0f, size );


		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -size, 0.0f, -size );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( size, 0.0f, size );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( size, 0.0f, -size );
	glEnd();

	size /= 1.5f;

	glBegin( GL_TRIANGLES );
		glColor4f( 1.0f, 1.0f, 1.0f, 0.5f );

		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -size, 0.01f, -size );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( -size, 0.01f, size );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( size, 0.01f, size );

		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -size, 0.01f, -size );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( size, 0.01f, size );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( size, 0.01f, -size );
	glEnd();

	glPopMatrix();

	SVector3 cameraPos;
	gCamera->GetPosition( &cameraPos.x, &cameraPos.y, &cameraPos.z );

	SVector3 billboardAngle = position - cameraPos;
	//position.Difference( &cameraPos, &billboardAngle );
	glRotatef( atan2( billboardAngle.z, billboardAngle.x ) * DEGREES + 90.0f, 0.0f, -1.0f, 0.0f );

	mShineTexture->Bind();

	float height = 3.0f * sin( mShine ) + 6.0f ;

	glBegin( GL_TRIANGLES );
		glColor4f( 1.0f, 1.0f, 1.0f, 0.8f );

		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( -size, 0.0f, 0.0f );
		glTexCoord2f( 0.01f, 0.0f );
		glVertex3f( -size, height, 0.0f );
		glTexCoord2f( 0.01f, 1.0f );
		glVertex3f( size, height, 0.0f );


		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( -size, 0.0f, 0.0f );
		glTexCoord2f( 0.01f, 1.0f );
		glVertex3f( size, height, 0.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( size, 0.0f, 0.0f );
	glEnd();

	if ( mGlowing )
	{
		glDisable( GL_DEPTH_TEST );
		glBegin( GL_TRIANGLES );
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

			glTexCoord2f( 1.0f, 0.0f );
			glVertex3f( -1.0f, 0.0f, 0.0f );

			glTexCoord2f( 0.01f, 0.0f );
			glVertex3f( -1.0f, mGlow, 0.0f );

			glTexCoord2f( 0.01f, 1.0f );
			glVertex3f( 1.0f, mGlow, 0.0f );


			glTexCoord2f( 1.0f, 0.0f );
			glVertex3f( -1.0f, 0.0f, 0.0f );

			glTexCoord2f( 0.01f, 1.0f );
			glVertex3f( 1.0f, mGlow, 0.0f );

			glTexCoord2f( 1.0f, 1.0f );
			glVertex3f( 1.0f, 0.0f, 0.0f );
		glEnd();
		glEnable( GL_DEPTH_TEST );
	}


	//glEnable( GL_FOG );
//	gLevel->Fog();
	glEnable( GL_CULL_FACE );
	glEnable( GL_LIGHTING );
	glDisable( GL_BLEND );
	mShineTexture->Unbind();
	//glDisable( GL_TEXTURE_2D );
}

void CPortal::OnCreatePhysical()
{
}


