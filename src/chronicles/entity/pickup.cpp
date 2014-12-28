#include "pickup.h"
#include "particles/gem.h"
#include "../main.h"
#include "../../engine/engine.h"
#include "../../engine/sound/wavesound.h"

CPickup::CPickup( int type )
{
	mType = type;
	mRot = 0.0f;
	mEmitter = NULL;

	switch ( mType )
	{
	case KEY_PICKUP_TYPE:
		mTexture = CManagedTexture::Load( &gResourceManager, "data/keySprite2.bmp" );
//		mTexture = CManagedTexture::Load( &gResourceManager, "data/keySprite2.png", true );
		mMask = CManagedTexture::Load( &gResourceManager, "data/keyMask2.bmp" );
		break;
	case HEALTH_PICKUP_TYPE:
		mMask = NULL;
		mTexture = CManagedTexture::Load( &gResourceManager, "data/heart.bmp" );
		break;
	case MAGIC_PICKUP_TYPE:
		mMask = NULL;
		mTexture = CManagedTexture::Load( &gResourceManager, "data/mana.bmp" );
		break;
	}
}

CPickup::~CPickup()
{
	CManagedTexture::Unload( &gResourceManager, mTexture );
	if ( mMask )
	{
		CManagedTexture::Unload( &gResourceManager, mMask );
	}
	if ( mEmitter && gEntities.FindEntry( mEmitter ) )
	{
		mEmitter->SetParticlesLeft( 0 );
	}
}

void CPickup::Update( float dt )
{
	mRot += dt * 120.0f;

	if ( mEmitter == NULL )
	{
		switch ( mType )
		{
		case KEY_PICKUP_TYPE:
			break;
		case HEALTH_PICKUP_TYPE:
			break;
		case MAGIC_PICKUP_TYPE:
			break;
		case GEM_PICKUP_TYPE:
			mEmitter = new CGem( mObject->GetPosition().x, mObject->GetPosition().y, mObject->GetPosition().z );
			gEntities.Add( mEmitter );
			break;
		}
	}
}

void CPickup::RenderAlpha()
{
	if ( mType == KEY_PICKUP_TYPE )
	{
		glDisable( GL_CULL_FACE );
		glEnable( GL_BLEND );
		glDisable( GL_LIGHTING );
		glDisable( GL_FOG );

		glTranslatef( mObject->GetPosition().x, mObject->GetPosition().y, mObject->GetPosition().z );
		glRotatef( mRot, 0.0f, 1.0f, 0.0f );

		for ( int i = 0; i < 2; i++ )
		{
			if ( i == 0 )
			{
				glBlendFunc( GL_DST_COLOR, GL_ZERO );
				mMask->Bind();
				glDepthMask(GL_FALSE);
			}
			else if ( i == 1 )
			{
				glBlendFunc( GL_ONE, GL_ONE );
				mTexture->Bind();
				glDepthMask(GL_TRUE);
			}

//			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//			mTexture->Bind();

			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			glBegin( GL_QUADS );
				glTexCoord2f( 0.0f, 0.0f );
				glVertex3f( 0.0f, 2.0f, -2.0f );
				glTexCoord2f( 0.0f, 1.0f );
				glVertex3f( 0.0f, -2.0f, -2.0f );
				glTexCoord2f( 1.0f, 1.0f );
				glVertex3f( 0.0f, -2.0f, 2.0f );
				glTexCoord2f( 1.0f, 0.0f );
				glVertex3f( 0.0f, 2.0f, 2.0f );
			glEnd();
		}

		glEnable( GL_CULL_FACE );
		glDisable( GL_BLEND );
		mTexture->Unbind();
		glEnable( GL_LIGHTING );
		//glEnable( GL_FOG );
		gLevel->Fog();
	}
}

int CPickup::GetType()
{
	return mType;
}


void CPickup::PickUp()
{
	CWaveSound* s = NULL;

	switch ( mType )
	{
	case GEM_PICKUP_TYPE:
		gLevel->CreatePortal();

		//	This sound is not unloaded X_X, but it will be automatically [with warnings! oh noes]
		s = CManagedWaveSound::Load( &gResourceManager, "data/portalActive.wav" );
		s->Play( 0 );

		gEntities.RemoveEntry( gEntities.FindEntry( this ) );
		delete this;
		break;
	case KEY_PICKUP_TYPE:

		s = CManagedWaveSound::Load( &gResourceManager, "data/portalActive.wav" );
		s->Play( 0 );

		if ( gPlayer )
		{
			gPlayer->AddKey();
		}

		gEntities.RemoveEntry( gEntities.FindEntry( this ) );
		delete this;
		break;
	}
}
