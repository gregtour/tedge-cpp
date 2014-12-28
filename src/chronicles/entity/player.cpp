#include <math.h>
#include "player.h"
#include "menuInput.h"
#include "../main.h"
#include "../../engine/engine.h"
#include "../game/gameInput.h"
#include "../../engine/graphics/graphics.h"
#include "../../engine/common/log.h"
#include "particles/playerDeath.h"
#include "cameraController.h"
#include "pause.h"

#define	FALL_HEIGHT	0.0f

CPlayer::CPlayer()
{
	mSwordSwishSound = CManagedWaveSound::Load( &gResourceManager, "data/swish.wav" );
	gPlayer = this;
	SetHealth( MAX_PLAYER_HEALTH );

	LoadHitSound();

	mJumping = false;

	mSwinging = false;
	mSwingRot = 0.0f;
	lastSwing.Clear();
	mSword = NULL;
	mSwingRight = false;
	mLevitating = false;
	mNumKeys = 0;

#ifdef LOW_RESOLUTION_TEXTURES
	mSwordTexture = CManagedTexture::Load( &gResourceManager, "data/low/sword.bmp" );
	mSwordMask = CManagedTexture::Load( &gResourceManager, "data/low/swordmask.bmp" );
#else
	mSwordTexture = CManagedTexture::Load( &gResourceManager, "data/sword.bmp" );
	mSwordMask = CManagedTexture::Load( &gResourceManager, "data/swordmask.bmp" );
#endif

	//mQuadric = gluNewQuadric();
	//gluQuadricNormals( mQuadric, GLU_SMOOTH );
	//gluQuadricTexture( mQuadric, GL_TRUE );

	CWaveSound* spawnNoise = CManagedWaveSound::Load( &gResourceManager, "data/spawn.wav" );
	spawnNoise->Play( 0 );
}

CPlayer::~CPlayer()
{
	if ( gState != NEXT_LEVEL_STATE )
		gEntities.Add( new CMenuInput() );

	//gluDeleteQuadric( mQuadric );
	CManagedTexture::Unload( &gResourceManager, mSwordTexture );
	CManagedTexture::Unload( &gResourceManager, mSwordMask );
	CManagedWaveSound::Unload( &gResourceManager, mSwordSwishSound );
	gPlayer = NULL;
}

void CPlayer::Levitate()
{
	if (!mLevitating)
	{
		mLevitating = true;
		mObject->SetObjectType( PLAYER_LEVITATE_OBJ );
		mObject->SetCollisionMask(1);
		mLevitateTimer.Step();
		mObject->SetVelocity(SVector3(0.0f, 0.0f, 0.0f));
	}
}

void CPlayer::Update( float dt )
{

	if (mLevitating && mLevitateTimer.Time() > 2.0f) {
		ChangeGameState( NEXT_LEVEL_STATE );
	}

	if ( mObject->GetVelocity().y == 0.0f)
	{
		mLastSafe = GetPosition();
		if (mJumping)
			mJumping = false;
	}

	if (mInputState->IsKeyHit(K_JUMP) && !mJumping)
	{
		//Jump();
		SVector3 v = mObject->GetVelocity();
		v.y = 20.0f;
		mObject->SetVelocity(v);
		mJumping = true;
	}

	if ( mInputState->IsKeyPressed(K_UP))
	{
		SVector3 force;
		force.x = cos( mRot ) * PLAYER_SPEED * dt;
		force.z = sin( mRot ) * PLAYER_SPEED * dt;
		mObject->ApplyForce( force );
	}

	if (mInputState->yaxis > 0.5f)
	{
		SVector3 force;
		force.x = cos( mRot ) * PLAYER_SPEED * mInputState->yaxis * dt;
		force.z = sin( mRot ) * PLAYER_SPEED * mInputState->yaxis * dt;
		mObject->ApplyForce( force );
	}

	if ( mInputState->IsKeyPressed(K_DOWN))
	{
		SVector3 force;
		force.x = -cos( mRot ) * PLAYER_SPEED * 0.7f * dt;
		force.z = -sin( mRot ) * PLAYER_SPEED * 0.7f * dt;
		mObject->ApplyForce( force );
	}

	if (mInputState->yaxis < -0.5f)
	{
		SVector3 force;
		force.x = cos( mRot ) * PLAYER_SPEED * 0.7f * mInputState->yaxis * dt;
		force.z = sin( mRot ) * PLAYER_SPEED * 0.7f * mInputState->yaxis * dt;
		mObject->ApplyForce( force );
	}
	
	if ( mInputState->IsKeyPressed(K_RIGHT) )
	{
		mRot += PLAYER_ROT_SPEED * dt;
	}

	if (mInputState->xaxis > 0.5f)
	{
		mRot += PLAYER_ROT_SPEED * mInputState->xaxis * dt;
	}

	if ( mInputState->IsKeyPressed(K_LEFT) )
	{
		mRot -= PLAYER_ROT_SPEED * dt;
	}

	if (mInputState->xaxis < -0.5f)
	{
		mRot += PLAYER_ROT_SPEED * mInputState->xaxis * dt;
	}


	PhysicalSword();

	if ( mSwinging )
	{
		if ( mSwingRight )
		{
			mSwingRot += SWING_SPEED * dt;

			if ( mSwingRot > SWING_ROT / 2.0f )
			{
				mSwinging = false;

				if ( mSword != NULL )
				{
					mWorld->Remove( mSword );
					delete mSword;
					mSword = NULL;
				}
			}
		} else {
			mSwingRot -= SWING_SPEED * dt;

			if ( mSwingRot < -SWING_ROT / 2.0f )
			{
				mSwinging = false;

				if ( mSword != NULL )
				{
					mWorld->Remove( mSword );
					delete mSword;
					mSword = NULL;
				}
			}
		}
	}
/*
	if ( mInputState->IsKeyPressed( K_CAMERA ) )
	{
		CListEntry<CKeyState>* key = mInputState->keys.GetFirst();
		while ( key )
		{
			if ( key->data->key.keyNumber == K_CAMERA )
			{
				CListEntry<CKeyStatus>* kStatus = key->data->status.GetFirst();
				int camCount = 0
				while ( kStatus )
				{
					if ( 
					kStatus = kStatus->GetNext();
				}
				mCamCount
			}
			key = key->GetNext();
		}

		if ( key && key->data->key.keyNumber == K_SWING )
		{
	}
*/


	if ( mSwinging == false && lastSwing.Time() > SWING_DELAY )
	{
//		CListEntry<CKeyState>* key = mInputState->keys.GetFirst();
//		while ( key && key->data->key.keyNumber != K_SWING )
//			key = key->GetNext();

//		if ( key && key->data->key.keyNumber == K_SWING )
		if ( mInputState->IsKeyPressed(K_SWING) )
		{
//			float duration = 0.0f;
//			bool pressed = false;
			
//			if ( key->data->status.GetLast() )
/*			{
				duration += key->data->status.GetLast()->data->duration.Time();
				if ( key->data->status.GetLast()->data->status == false )
				{
					if ( key->data->status.GetLast()->GetPrev() )
					{
						if ( key->data->status.GetLast()->GetPrev()->data->status == true )
						{
							pressed = true;
							duration += key->data->status.GetLast()->GetPrev()->data->duration.DT();
						}
					}
				} else {
					pressed = true;
				}
			}

//			if ( pressed && lastSwing.Time() - duration > SWING_DELAY )
			{ */
				Swing();
//			}
		}	
	}

	if ( mLevitating )
	{
		//SVector3 up = SVector3(0.0f, 5.0f, 0.0f);
		//mObject->ApplyForce(up);
		SVector3 v = mObject->GetVelocity();
		v.y = 5.0f;
		mObject->SetVelocity(v);
/*		SVector3 p = mObject->GetPosition();
		p.y += dt * 5.0f;
		mObject->SetPosition(p);*/
	}
	//mRot += GetMouse().mMouseXSp * dt / 400.0f;

	if (GetPosition().y < FALL_HEIGHT)
	{
		mObject->SetPosition(mLastSafe);
		SVector3 v = mObject->GetVelocity();
		v.y = 5.0f;
		mObject->SetVelocity(v);
		Hit(NULL, 3);
	}
	if (mInputState->IsKeyHit(K_START))
	{
		ChangeGameState(PAUSE_STATE);
	}
}


void CPlayer::Swing()
{
	if ( mSwingRight && lastSwing.Time() < 0.6f )
		mSwingRight = false;
	else
		mSwingRight = true;
	mSwordSwishSound->Play( 0 );
	mSwinging = true;
	lastSwing.Step();
	if ( mSwingRight )
		mSwingRot = -SWING_ROT / 2.0f;
	else
		mSwingRot = SWING_ROT / 2.0f;
	PhysicalSword();
}

void CPlayer::PhysicalSword()
{
	if ( mSword != NULL )
	{
		mWorld->Remove( mSword );
		delete mSword;
		mSword = NULL;
	}

	if ( mSwinging )
	{
		SVector3 pos = mObject->GetPosition();
		float x1 = (float)(cos( mRot + mSwingRot * RADS / 2 ) * mObject->GetSphere().radius + pos.x);
		float z1 = (float)(sin( mRot + mSwingRot * RADS / 2 ) * mObject->GetSphere().radius + pos.z);
		float x2 = (float)(cos( mRot + mSwingRot * RADS / 2 ) * 4.4f + pos.x);
		float z2 = (float)(sin( mRot + mSwingRot * RADS / 2 ) * 4.4f + pos.z);

		mSword = new CPTriangle(true);		
		mSword->SetPoint(SVector3(x1, -3.0f + pos.y, z1), 0);
		mSword->SetPoint(SVector3(x2, -3.0f + pos.y, z2), 1);
		mSword->SetPoint(SVector3(x2, 6.5f + pos.y, z2), 2);
		mSword->SetCollisionMask(1);
		mSword->SetObjectType(SWORD_OBJ_TYPE);

		//mSword = new CPLine( new SPoint(x1,z1), new SPoint(x2,z2), new SPoint(), new SVector(), false, SWORD_OBJ_TYPE );
		//mSword->SetVelocity( &mObject->GetVelocity() );
		mWorld->Add( mSword );
	}
}


void CPlayer::Render()
{
/*	gLevel->Light( mObject->GetPosition() );

	glTranslatef( mObject->GetPosition().x,  mObject->GetPosition().y, mObject->GetPosition().z );
	glRotatef( (GLfloat)(mRot * DEGREES), 0.0f, -1.0f, 0.0f );
	glPushMatrix();

    glColor3f( 0.3f, 0.7f, 0.3f);
	if ( mLastHit.Time() < MIN_HIT_TIME )
		glColor3f( (MIN_HIT_TIME - mLastHit.Time())/1.5f + 0.3f, 0.7f, 0.3f );

	glRotatef( 90.0f, -1.0f, 0.0f, 0.0f );
	#ifdef LOW_QUALITY_GEOMETRY
	gluCylinder( mQuadric, 1.2f, 0.4f, PLAYER_HEIGHT, 10, 4 );
	#else
	gluCylinder( mQuadric, 1.2f, 0.4f, PLAYER_HEIGHT, 18, 6 );
	#endif

	glColor3f( 0.3f, 0.3f, 0.7f );
	glPopMatrix();
	glTranslatef( 0.0f, PLAYER_HEIGHT, 0.0f );

	#ifdef LOW_QUALITY_GEOMETRY
	gluSphere( mQuadric, 0.9f, 8, 8 );
	#else
	gluSphere( mQuadric, 0.9f, 16, 16 );
	#endif
*/
	/*	Temporary Code, Draw A Cube as a Placeholder	*/
	/*
  glBegin(GL_TRIANGLES);		// Draw The Cube Using quads

    glColor3f(0.8f,0.0f,0.0f);	// Color Red	
	if ( mLastHit.Time() < 1.0f )
		glColor3f( (1.0f - mLastHit.Time())/2 + 0.5f, 0.0f, 0.0f );
	glNormal3f( 0.0f, 0.0f, 1.0f );
    glVertex3f( 1.0f, PLAYER_HEIGHT, 1.0f);	// Top Right Of The Quad (Front)
    glVertex3f(-1.0f, PLAYER_HEIGHT, 1.0f);	// Top Left Of The Quad (Front)
    glVertex3f(-1.0f, 0.0f, 1.0f);	// Bottom Left Of The Quad (Front)

    glVertex3f( 1.0f, PLAYER_HEIGHT, 1.0f);	// Top Right Of The Quad (Front)
    glVertex3f(-1.0f, 0.0f, 1.0f);	// Bottom Left Of The Quad (Front)
    glVertex3f( 1.0f, 0.0f, 1.0f);	// Bottom Right Of The Quad (Front)

	if ( mLastHit.Time() > 1.0f )
		glColor3f(0.0f,0.8f,0.0f);
	glNormal3f( 0.0f, 1.0f, 0.0f );
    glVertex3f( 1.0f, PLAYER_HEIGHT,-1.0f);	// Top Right Of The Quad (Top)
    glVertex3f(-1.0f, PLAYER_HEIGHT,-1.0f);	// Top Left Of The Quad (Top)
    glVertex3f(-1.0f, PLAYER_HEIGHT, 1.0f);	// Bottom Left Of The Quad (Top)

    glVertex3f( 1.0f, PLAYER_HEIGHT,-1.0f);	// Top Right Of The Quad (Top)
    glVertex3f(-1.0f, PLAYER_HEIGHT, 1.0f);	// Bottom Left Of The Quad (Top)
    glVertex3f( 1.0f, PLAYER_HEIGHT, 1.0f);	// Bottom Right Of The Quad (Top)

    if ( mLastHit.Time() > 1.0f  )
		glColor3f(0.0f,0.0f,0.8f);
	glNormal3f( 0.0f, -1.0f, 0.0f );
    glVertex3f( 1.0f, 0.0f, 1.0f);	// Top Right Of The Quad (Bottom)
    glVertex3f(-1.0f, 0.0f, 1.0f);	// Top Left Of The Quad (Bottom)
    glVertex3f(-1.0f, 0.0f,-1.0f);	// Bottom Left Of The Quad (Bottom)

    glVertex3f( 1.0f, 0.0f, 1.0f);	// Top Right Of The Quad (Bottom)
    glVertex3f(-1.0f, 0.0f,-1.0f);	// Bottom Left Of The Quad (Bottom)
    glVertex3f( 1.0f, 0.0f,-1.0f);	// Bottom Right Of The Quad (Bottom)

    if ( mLastHit.Time() > 1.0f  )
		glColor3f(0.8f,0.8f,0.0f);
	glNormal3f( 0.0f, 0.0f, -1.0f );
    glVertex3f( 1.0f, 0.0f,-1.0f);	// Top Right Of The Quad (Back)
    glVertex3f(-1.0f, 0.0f,-1.0f);	// Top Left Of The Quad (Back)
    glVertex3f(-1.0f, PLAYER_HEIGHT,-1.0f);	// Bottom Left Of The Quad (Back)

    glVertex3f( 1.0f, 0.0f,-1.0f);	// Top Right Of The Quad (Back)
    glVertex3f(-1.0f, PLAYER_HEIGHT,-1.0f);	// Bottom Left Of The Quad (Back)
    glVertex3f( 1.0f, PLAYER_HEIGHT,-1.0f);	// Bottom Right Of The Quad (Back)

    if ( mLastHit.Time() > 1.0f  )
		glColor3f(0.0f,0.8f,0.8f);
	glNormal3f( -1.0f, 0.0f, 0.0f );
    glVertex3f(-1.0f, PLAYER_HEIGHT, 1.0f);	// Top Right Of The Quad (Left)
    glVertex3f(-1.0f, PLAYER_HEIGHT,-1.0f);	// Top Left Of The Quad (Left)
    glVertex3f(-1.0f, 0.0f,-1.0f);	// Bottom Left Of The Quad (Left)

    glVertex3f(-1.0f, PLAYER_HEIGHT, 1.0f);	// Top Right Of The Quad (Left)
    glVertex3f(-1.0f, 0.0f,-1.0f);	// Bottom Left Of The Quad (Left)
    glVertex3f(-1.0f, 0.0f, 1.0f);	// Bottom Right Of The Quad (Left)

    if ( mLastHit.Time() > 1.0f  )
		glColor3f(0.8f,0.0f,0.8f);
	glNormal3f( 1.0f, 0.0f, 0.0f );
    glVertex3f( 1.0f, PLAYER_HEIGHT,-1.0f);	// Top Right Of The Quad (Right)
    glVertex3f( 1.0f, PLAYER_HEIGHT, 1.0f);	// Top Left Of The Quad (Right)
    glVertex3f( 1.0f, 0.0f, 1.0f);	// Bottom Left Of The Quad (Right)

    glVertex3f( 1.0f, PLAYER_HEIGHT,-1.0f);	// Top Right Of The Quad (Right)
    glVertex3f( 1.0f, 0.0f, 1.0f);	// Bottom Left Of The Quad (Right)
    glVertex3f( 1.0f, 0.0f,-1.0f);	// Bottom Right Of The Quad (Right)
  glEnd();			// End Drawing The Cube
  */
}

void CPlayer::RenderAlpha()
{/*
	if (mSword)
	{
		glBegin(GL_TRIANGLES);
		for(int j = 0; j < 3; j++)
		{
			SVector3 p;
			p = mSword->GetPoint(j);
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
	}*/

	SVector3 pos = mObject->GetPosition();
	if (mSwinging)
		glTranslatef( pos.x, pos.y-0.9f, pos.z );
	else
		glTranslatef( pos.x, pos.y-0.6f, pos.z );
	glRotatef( (GLfloat)(mRot * DEGREES), 0.0f, -1.0f, 0.0f );
	glTranslatef(0.0f, 0.0f, 0.7f);
	if (mSwinging)
	{
		if ( mSwingRight )
			glRotatef( mSwingRot, 0.0f, 0.0f, -0.5f );
		else
			glRotatef( mSwingRot, 0.0f, 0.0f, 0.5f );
		glRotatef( mSwingRot, 0.0f, -1.0f, 0.0f );
	}

	glDisable( GL_CULL_FACE );
	glEnable( GL_BLEND );
	//glEnable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glDisable( GL_FOG );
	glDisable(GL_DEPTH_TEST);

	for ( int i = 0; i < 2; i++ )
	{

		if ( i == 0 )
		{
			glPushMatrix();
			glBlendFunc( GL_DST_COLOR, GL_ZERO );
			mSwordMask->Bind();
		}
		else if ( i == 1 )
		{
			glPopMatrix();
			glBlendFunc( GL_ONE, GL_ONE );
			mSwordTexture->Bind();
		}

		/*			Draw Sword			*/
		/*if ( mSwinging )
		{
			glTranslatef( 0.0f, 2.5f, 0.0f );
			if ( mSwingRight )
				glRotatef( mSwingRot, 0.0f, 0.0f, -0.5f );
			else
				glRotatef( mSwingRot, 0.0f, 0.0f, 0.5f );
			glRotatef( mSwingRot, 0.0f, -1.0f, 0.0f );

			glBegin( GL_TRIANGLES );
				glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

				glTexCoord2f( 0.0f, 1.0f );
				glVertex3f( 0.75f, -0.25f, 0.0f );

				glTexCoord2f( 0.0f, 0.0f );
				glVertex3f( 4.0f, -0.25f, 0.0f );

				glTexCoord2f( 1.0f, 0.0f );
				glVertex3f( 4.0f, 0.25f, 0.0f );

				glTexCoord2f( 0.0f, 1.0f );
				glVertex3f( 0.75f, -0.25f, 0.0f );

				glTexCoord2f( 1.0f, 0.0f );
				glVertex3f( 4.0f, 0.25f, 0.0f );

				glTexCoord2f( 1.0f, 1.0f );
				glVertex3f( 0.75f, 0.25f, 0.0f );
			glEnd();
		} 
		else 
		{*/
			glBegin( GL_TRIANGLES );
				glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

				glTexCoord2f( 0.0f, 1.0f );
				glVertex3f( 1.0f, 2.25f, -0.25f );

				glTexCoord2f( 0.0f, 0.0f );
				glVertex3f( 2.0f, 4.75f, -0.25f );

				glTexCoord2f( 1.0f, 0.0f );
				glVertex3f( 2.0f, 4.75f, 0.25f );


				glTexCoord2f( 0.0f, 1.0f );
				glVertex3f( 1.0f, 2.25f, -0.25f );

				glTexCoord2f( 1.0f, 0.0f );
				glVertex3f( 2.0f, 4.75f, 0.25f );

				glTexCoord2f( 1.0f, 1.0f );
				glVertex3f( 1.0f, 2.25f, 0.25f );
			glEnd();
		//}
	}

	glEnable(GL_DEPTH_TEST);
	mSwordTexture->Unbind();
	glEnable( GL_CULL_FACE );
	glDisable( GL_BLEND );
	glEnable( GL_LIGHTING );
	//glEnable( GL_FOG );
	gLevel->Fog();
}

CMousePosition CPlayer::GetMouse()
{
	return mInputState->mousePosition;
}

void CPlayer::StopSwing()
{
	mSwinging = false;
}


void CPlayer::DeathEffect()
{
	CParticleEmitter* p = new CPlayerDeath( mObject->GetPosition().x, 0.0f, mObject->GetPosition().y );
	gEntities.Add( p );
}


int CPlayer::NumKeys()
{
	return mNumKeys;
}

void CPlayer::AddKey()
{
	mNumKeys++;
}

bool CPlayer::RemoveKey()
{
	if ( mNumKeys > 0 )
	{
		mNumKeys--;
		return true;
	}
	else
		return false;
}

