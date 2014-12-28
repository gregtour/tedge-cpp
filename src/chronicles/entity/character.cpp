#include "../game/game.h"
#include "../main.h"
#include "character.h"
#include <string>
#include <sstream>

CCharacter::CCharacter()
{
	LoadHitSound();
	mRot = 0.0f;
}

CCharacter::~CCharacter()
{
}

void CCharacter::Update( float dt )
{
};

void CCharacter::LoadHitSound()
{
	mHitSound = CManagedWaveSound::Load( &gResourceManager, "data/grunt.wav" );
}


float CCharacter::GetUpRot()
{
	return mUpRot;
//	return 0.0f;
}

void CCharacter::Render()
{
	if ( mObject )
	{
		SBoundingBox b = mObject->GetBoundingBox();
		SVector3 translate = ( b.min + b.max ) / 2.0f;
		glTranslatef( translate.x, translate.y, translate.z );
		SVector3 scale = b.max - b.min;
		glScalef( scale.x, scale.y, scale.z );
		glRotatef( mRot * DEGREES, 0.0f, -1.0f, 0.0f );

		{	// Draw Box
			float r = 0.5f;
			glColor3f( 1.0f, 1.0f, 1.0f );
			glBegin(GL_QUADS);		// Draw The Cube Using quads
				glNormal3f( 0.0f, 1.0f, 0.0f );
				glVertex3f( r, r,-r);	// Top Right Of The Quad (Top)
				glVertex3f(-r, r,-r);	// Top Left Of The Quad (Top)
				glVertex3f(-r, r, r);	// Bottom Left Of The Quad (Top)
				glVertex3f( r, r, r);	// Bottom Right Of The Quad (Top)
				glNormal3f( 0.0f, -1.0f, 0.0f );
				glVertex3f( r,-r, r);	// Top Right Of The Quad (Bottom)
				glVertex3f(-r,-r, r);	// Top Left Of The Quad (Bottom)
				glVertex3f(-r,-r,-r);	// Bottom Left Of The Quad (Bottom)
				glVertex3f( r,-r,-r);	// Bottom Right Of The Quad (Bottom)
				glNormal3f( 0.0f, 0.0f, 1.0f );
				glVertex3f( r, r, r);	// Top Right Of The Quad (Front)
				glVertex3f(-r, r, r);	// Top Left Of The Quad (Front)
				glVertex3f(-r,-r, r);	// Bottom Left Of The Quad (Front)
				glVertex3f( r,-r, r);	// Bottom Right Of The Quad (Front)
				glNormal3f( 0.0f, 0.0f, -1.0f );
				glVertex3f( r,-r,-r);	// Top Right Of The Quad (Back)
				glVertex3f(-r,-r,-r);	// Top Left Of The Quad (Back)
				glVertex3f(-r, r,-r);	// Bottom Left Of The Quad (Back)
				glVertex3f( r, r,-r);	// Bottom Right Of The Quad (Back)
				glNormal3f( -1.0f, 0.0f, 0.0f );
				glVertex3f(-r, r, r);	// Top Right Of The Quad (Left)
				glVertex3f(-r, r,-r);	// Top Left Of The Quad (Left)
				glVertex3f(-r,-r,-r);	// Bottom Left Of The Quad (Left)
				glVertex3f(-r,-r, r);	// Bottom Right Of The Quad (Left)
				glNormal3f( 1.0f, 0.0f, 0.0f );
				glVertex3f( r, r,-r);	// Top Right Of The Quad (Right)
				glVertex3f( r, r, r);	// Top Left Of The Quad (Right)
				glVertex3f( r,-r, r);	// Bottom Left Of The Quad (Right)
				glVertex3f( r,-r,-r);	// Bottom Right Of The Quad (Right)
			  glEnd();			// End Drawing The Cube
		}
	}
};

SVector3 CCharacter::GetPosition()
{
	return mObject->GetPosition();
}

int CCharacter::GetHealth()
{
	return mHealth;
}

void CCharacter::SetHealth( int h )
{
	mHealth = h;
}

bool CCharacter::Hit( CCharacter* byWhom, int damage )
{
	if ( mLastHit.Time() > MIN_HIT_TIME )
	{
//		if ( byWhom == gPlayer )
//			gPlayerScore += HIT_ENEMY_POINTS;
		mHitSound->Play( 0 );

		mLastHit.Step();

		if (byWhom)
		{
			SVector3 v = mObject->GetPosition() - byWhom->GetPosition();
			v.Normalize();
			v = v * (damage * 20.0f );
		//	mObject->Translate( v.x, v.y );
			mObject->SetVelocity( v );
		}

		mHealth -= damage;
		if ( mHealth <= 0 )
		{
//			if ( byWhom == gPlayer )
//				gPlayerScore += KILLED_ENEMY_POINTS;
			Die();
			return true;
		}
	}
	return false;
}

void CCharacter::Die()
{
	DeathEffect();

	/*	Remove it from the game	*/
	gEntities.RemoveEntry( gEntities.FindEntry( this ) );
	/*	Delete it	*/
	delete this;
}

void CCharacter::DeathEffect()
{

}


