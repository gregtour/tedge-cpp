#include "hud.h"
#include <sstream>
#include "../main.h"
#include "../game/game.h"

CHud::CHud()
{
	mCircleTexture = CManagedTexture::Load( &gResourceManager, "data/circle.bmp" );
#ifdef LOW_RESOLUTION_TEXTURES
	mScoreFont = new CFontRenderer( "data/low/digits.bmp", "data/low/digitsmask.bmp", 10, "0123456789" );
	mGameOverTexture = CManagedTexture::Load( &gResourceManager, "data/low/gameover.bmp" );
	mHealthTexture = CManagedTexture::Load( &gResourceManager, "data/low/health.bmp" );
	mBarTexture = CManagedTexture::Load( &gResourceManager, "data/low/bar.bmp" );
	mMagicTexture = CManagedTexture::Load( &gResourceManager, "data/low/magic.bmp" );
#else
	//mScoreFont = new CFontRenderer( "data/digits.bmp", "data/digitsmask.bmp", 10, "0123456789" );
	mScoreFont = new CFontRenderer("data/font.bmp", "data/fontmask.bmp", 16, " !\"   &'() +,~./0123456789:;   ? ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]   abcdefghijklmnopqrstuvwxyz                                   ");
	mGameOverTexture = CManagedTexture::Load( &gResourceManager, "data/gameover.bmp" );
	mHealthTexture = CManagedTexture::Load( &gResourceManager, "data/health.bmp" );
	mBarTexture = CManagedTexture::Load( &gResourceManager, "data/bar.bmp" );
	mMagicTexture = CManagedTexture::Load( &gResourceManager, "data/magic.bmp" );
#endif

	mGameOverTexture2 = CManagedTexture::Load( &gResourceManager, "data/gameover2.bmp" );
}

CHud::~CHud()
{
	CManagedTexture::Unload( &gResourceManager, mHealthTexture );
	CManagedTexture::Unload( &gResourceManager, mBarTexture );
	CManagedTexture::Unload( &gResourceManager, mMagicTexture );
	CManagedTexture::Unload( &gResourceManager, mGameOverTexture );
	CManagedTexture::Unload( &gResourceManager, mCircleTexture );
	delete mScoreFont;
}

void CHud::RenderOrtho()
{
	std::stringstream stream;
	stream << gPlayerScore;
	std::string score = stream.str();
//	mScoreFont->RenderString( score, 0.96f - (0.035f*score.length()), 0.02f, 0.06f, 0.06f, 0.035f );

//#ifdef SHOW_FPS
	std::stringstream fpsStream;
	fpsStream << (int)gFPS;
	std::string fps = fpsStream.str();
//	mScoreFont->RenderString( fps, 0.01f, 0.94f / SCR_WIDTH * SCR_HEIGHT, 0.05f, 0.05f, 0.035f );
//#endif

	if ( !gPlayer )
	{/*
		glDisable( GL_LIGHTING );
		glDisable( GL_FOG );
		glEnable( GL_BLEND );
		glDisable( GL_DEPTH_TEST );
		glDisable( GL_CULL_FACE );

		glBlendFunc( GL_ONE, GL_ONE );	

		if ( gPlayerScore >= POINTS_TO_CONTINUE )
			mGameOverTexture->Bind();
		else
			mGameOverTexture2->Bind();

		glBegin( GL_TRIANGLES );
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			
			glTexCoord2f( 0.0f, 0.0f );
			glVertex2f( 0.2f, 0.2f*SCR_HEIGHT/SCR_WIDTH );

			glTexCoord2f( 1.0f, 0.0f );
			glVertex2f( 0.8f, 0.2f*SCR_HEIGHT/SCR_WIDTH );

			glTexCoord2f( 1.0f, 1.0f );
			glVertex2f( 0.8f, 0.8f*SCR_HEIGHT/SCR_WIDTH );


			glTexCoord2f( 0.0f, 0.0f );
			glVertex2f( 0.2f, 0.2f*SCR_HEIGHT/SCR_WIDTH );

			glTexCoord2f( 1.0f, 1.0f );
			glVertex2f( 0.8f, 0.8f*SCR_HEIGHT/SCR_WIDTH );

			glTexCoord2f( 0.0f, 1.0f );
			glVertex2f( 0.2f, 0.8f*SCR_HEIGHT/SCR_WIDTH );
		glEnd();

		glEnable( GL_CULL_FACE );
		mGameOverTexture->Unbind();
		glDisable( GL_BLEND );
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_LIGHTING );
		//glEnable( GL_FOG );
		gLevel->Fog();*/
	}
	else
	{
		glDisable( GL_LIGHTING );
		glDisable( GL_FOG );
		glDisable( GL_DEPTH_TEST );
		glDisable( GL_CULL_FACE );

		glPushMatrix();
		glTranslatef( 0.05f, 0.9f * SCR_HEIGHT / SCR_WIDTH, 0.0f );

		mBarTexture->Bind();
		glBegin( GL_TRIANGLES );
			glColor3f( 1.0f, 1.0f, 1.0f );

			glTexCoord2f( 0.0f, 0.0f );
			glVertex2f( 0.0f, 0.0f );

			glTexCoord2f( 5.0f, 0.0f );
			glVertex2f( 0.4f, 0.0f );

			glTexCoord2f( 5.0f, 1.0f );
			glVertex2f( 0.4f, 0.03f );


			glTexCoord2f( 0.0f, 0.0f );
			glVertex2f( 0.0f, 0.0f );

			glTexCoord2f( 5.0f, 1.0f );
			glVertex2f( 0.4f, 0.03f );

			glTexCoord2f( 0.0f, 1.0f );
			glVertex2f( 0.0f, 0.03f );
		glEnd();

		float health = (float)gPlayer->GetHealth() / MAX_PLAYER_HEALTH;

		mHealthTexture->Bind();
		glBegin( GL_TRIANGLES );
			glColor3f( 1.0f, 1.0f, 1.0f );

			glTexCoord2f( 0.0f, 0.0f );
			glVertex2f( 0.0f, 0.0f );

			glTexCoord2f( 5.0f * health, 0.0f );
			glVertex2f( 0.4f * health, 0.0f );

			glTexCoord2f( 5.0f * health, 1.0f );
			glVertex2f( 0.4f * health, 0.03f );

			glTexCoord2f( 0.0f, 0.0f );
			glVertex2f( 0.0f, 0.0f );

			glTexCoord2f( 5.0f * health, 1.0f );
			glVertex2f( 0.4f * health, 0.03f );

			glTexCoord2f( 0.0f, 1.0f );
			glVertex2f( 0.0f, 0.03f );
		glEnd();
		
		glPopMatrix();

		glEnable( GL_CULL_FACE );
		mHealthTexture->Unbind();
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_LIGHTING );
		//glEnable( GL_FOG );
		gLevel->Fog();
	}
}
