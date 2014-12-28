#include "../../engine/engine.h"
#include "ghost.h"
#include "player.h"
#include "particles/death.h"
#include <math.h>

#define	SOUND_CUTOFF_DISTANCE	1500.0f
#define SOUND_REACH				10

CGhost::CGhost( float rotSpeed )
{
	mRotSpeed = rotSpeed;
	mAction = 0;
	
	mSound = NULL;
	mGhostSound = CManagedWaveSound::Load( &gResourceManager, "data/ghost.wav" );

	LoadTexture();
}

CGhost::~CGhost()
{
	CManagedWaveSound::Unload( &gResourceManager, mGhostSound );
	if ( mSound )
	{
		if ( mSound->IsPlaying() )
			mSound->Stop();
		delete mSound;
	}
}

void CGhost::LoadTexture()
{
	mTexture = CManagedTexture::Load( &gResourceManager, "data/ghost.bmp" );
}


void CGhost::Update( float dt )
{

	SVector3 pos = mObject->GetPosition();
	// Object Sound
	{
		float x, y, z;
		gCamera->GetPosition( &x, &y, &z );
		float distance = (x-pos.x)*(x-pos.x) + (z-pos.z)*(z-pos.z) + (y-pos.y)*(y-pos.y);

		if ( distance < SOUND_CUTOFF_DISTANCE )
		{
			distance = sqrt( distance );

			if ( !mSound )
			{
				mSound = new CSound( mGhostSound->Play( -1 ) );
			}

			int intDistance = (int)distance;
			if ( intDistance <= 0 )
				intDistance = 1;

			int volume = 128 / intDistance * SOUND_REACH;
			if ( volume <= 0 )
				volume = 1;
			if ( volume > 60 )
				volume = 60;
			
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

	if ( gPlayer )
	{


		/*	Manage Altitude	*/
		if ( mAction == 1 )
		{
			if ( pos.y > gPlayer->GetPosition().y + 10.0f - 2.0f )
			{
				pos.y -= ( gPlayer->GetPosition().y + 10.0f - 2.0f - pos.y ) * dt / 2.0f;
				mObject->SetPosition(pos);
			}
		}

		/*	Fly Around Player	*/
		SVector3 distance;
		distance = gPlayer->GetPosition() - GetPosition();
		float distanceToPlayer = distance.LengthSquared();

		/*	Distance	*/
		if ( mAction == 0 && distanceToPlayer < 16.0f * 16.0f )
			mAction = 1;
		if ( mAction == 1 && distanceToPlayer < 1.5f*1.5f && pos.y <= gPlayer->GetPosition().y + 10.0f )
			mAction = 2;
		if ( mAction == 2 && pos.y >= gPlayer->GetPosition().y + 6.0f )
			mAction = 0;
		
		/*	Rotation	*/
		if ( mAction == 0 || mAction == 2 )	// Circle
		{
		//	mRot = atan( &distance );
			mRot += mRotSpeed * RADS * dt;
		}
		else if ( mAction == 1 )	// Charge at it
		{
			mRot = atan2( distance.z, distance.x );
		} 
//		else						// Run Away
//		{
//			distance.Rotate( 180.0f );
//			mRot = atan( &distance );
//		}

		/*	Apply Movement	*/
		SVector3 force;
		force.x = cos( mRot ) * dt;
		force.z = sin( mRot ) * dt;
		force.y = 0.0f;

		if ( mAction == 0 )
			force = force * BAT_SPEED;
		else if ( mAction == 1 )
		{
			force = distance;
			force.Normalize();			
			force = force * 2.0f;
//			force = force * BAT_DIVE_SPEED / 3.0f;
		}
		else
			force = force * BAT_CLIMB_SPEED;

		if ( mAction == 2 && pos.y > gPlayer->GetPosition().y + 10.0f )
			force = force * 2.0f;

		mObject->ApplyForce( force );
	} else {
		mRot += mRotSpeed * RADS * dt;
		SVector3 force;
		force.x = cos( mRot ) * dt;
		force.z = sin( mRot ) * dt;
		force.y = 0.0f;
		force = force * BAT_SPEED;
		mObject->ApplyForce( force );
	}
}

void CGhost::RenderAlpha()
{
	SVector3 position = mObject->GetPosition();
	glTranslatef( position.x, position.y, position.z );

	SVector3 cameraPos;
	gCamera->GetPosition( &cameraPos.x, &cameraPos.y, &cameraPos.z );

	SVector3 billboardAngle;
	//position.Difference( &cameraPos, &billboardAngle );
	billboardAngle = position - cameraPos;
	glRotatef( atan2( billboardAngle.z, billboardAngle.x ) * DEGREES + 90.0f, 0.0f, -1.0f, 0.0f );

	glDisable( GL_CULL_FACE );
	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	//glEnable( GL_TEXTURE_2D );
	glDisable( GL_FOG );
	glDepthMask( GL_FALSE );

	glBlendFunc( GL_ONE, GL_ONE );	
	mTexture->Bind();


	glBegin( GL_QUADS );
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -1.0f, 2.0f, 0.0f );

		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( -1.0f, 0.0f, 0.0f );

		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( 1.0f, 0.0f, 0.0f );

		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( 1.0f, 2.0f, 0.0f );
	glEnd();

	mTexture->Unbind();
	glDisable( GL_BLEND );
	glEnable( GL_LIGHTING );
	glEnable( GL_CULL_FACE );
	glDepthMask( GL_TRUE );
	glEnable( GL_FOG );

}

void CGhost::Render()
{	
}

void CGhost::DeathEffect()
{
	SVector3 pos = GetPosition();
	gEntities.Add( new CDeath( pos.x, pos.y, pos.z, 1.0f, 1.0f, 1.0f, 1.0f ) );
}
