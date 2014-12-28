#include "pause.h"
#include "../game/gameInput.h"
#include "../main.h"
#include "../game/game.h"
#include "../../engine/engine.h"
#include "../../engine/graphics/font.h"
#include "../../engine/graphics/texture.h"
#include <sstream>


CPause::CPause()
{
	mFont = new CFontRenderer("data/font.bmp", "data/fontmask.bmp", 16, " !\"   &'() +,~./0123456789:;   ? ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]   abcdefghijklmnopqrstuvwxyz                                   ");
	mTitle = CManagedTexture::Load(&gResourceManager, "data/chronicles.png", true);
	mSelect = CManagedTexture::Load(&gResourceManager, "data/sword.png", true);
	
	mSelected = 0;

	//ChangeGameState(PAUSED_STATE);
	gPaused = true;

	t.Clear();
	t.Step();
}

CPause::~CPause()
{
	gPaused = false;
}

void CPause::Update( float dt )
{

	if (mInputState->IsKeyHit(K_UP))
	{
		if (mSelected > 0)
			mSelected--;
	} 

	if (mInputState->IsKeyHit(K_DOWN))
	{
		if (mSelected < 2)
			mSelected++;
	}


	if (mInputState->IsKeyHit(K_SWING) || mInputState->IsKeyHit(K_START))
	{
		if (mSelected == 0)
		{
			// resume game
			CListEntry<CEntity>* e = gEntities.FindEntry(this);
			e->data = NULL;
			delete this;
			return;
		} else if (mSelected == 1) {
			// restart level
			gLevelNumber--;
			ChangeGameState(NEXT_LEVEL_STATE);
		} else if (mSelected == 2) {
			// quit
			ChangeGameState(MAIN_MENU_STATE);
		}
	}

	/*
	if ( t.Time() > 2.5f )
	{
		if ( mInputState->IsKeyPressed(K_SWING) )
		{
			gLevelNumber = 0;
			gPlayerScore = 0;
			ChangeGameState( NEXT_LEVEL_STATE );
		}
		else if ( mInputState->IsKeyPressed(K_CONTINUE) && gPlayerScore >= POINTS_TO_CONTINUE )
		{
			gLevelNumber--;
			gPlayerScore -= POINTS_TO_CONTINUE;
			ChangeGameState( NEXT_LEVEL_STATE );
		}
	}*/
}


void CPause::RenderOrtho()
{
	glPushMatrix();
	mSelect->Bind();
		glDisable( GL_LIGHTING );
		glDisable( GL_FOG );
		glEnable( GL_BLEND );
		glDisable( GL_DEPTH_TEST );
		glDisable( GL_CULL_FACE );
		//glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(-0.3f, 0.12f+0.07f*(mSelected+1), 0.0f);
		glScalef(0.8f, 0.3f, 1.0f);
		glBegin( GL_TRIANGLES );
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			
			glTexCoord2f( 1.0f, 1.0f );
			glVertex2f( 0.2f, 0.2f*SCR_HEIGHT/SCR_WIDTH );

			glTexCoord2f( 1.0f, 0.0f );
			glVertex2f( 0.8f, 0.2f*SCR_HEIGHT/SCR_WIDTH );

			glTexCoord2f( 0.0f, 0.0f );
			glVertex2f( 0.8f, 0.8f*SCR_HEIGHT/SCR_WIDTH );


			glTexCoord2f( 1.0f, 1.0f );
			glVertex2f( 0.2f, 0.2f*SCR_HEIGHT/SCR_WIDTH );

			glTexCoord2f( 0.0f, 0.0f );
			glVertex2f( 0.8f, 0.8f*SCR_HEIGHT/SCR_WIDTH );

			glTexCoord2f( 0.0f, 1.0f );
			glVertex2f( 0.2f, 0.8f*SCR_HEIGHT/SCR_WIDTH );
		glEnd();
	glPopMatrix();


	mFont->RenderString("PAUSED", 0.22f, 0.1f, 0.11f, 0.11f, 0.1f);
	mFont->RenderString("Resume", 0.3f, 0.24f, 0.08f, 0.08f, 0.035f);
	mFont->RenderString("Start Over", 0.3f, 0.31f, 0.08f, 0.08f, 0.035f);
	mFont->RenderString("Quit", 0.3f, 0.38f, 0.08f, 0.08f, 0.035f);
/*
	mTitle->Bind();
		glDisable( GL_LIGHTING );
		glDisable( GL_FOG );
		glEnable( GL_BLEND );
		glDisable( GL_DEPTH_TEST );
		glDisable( GL_CULL_FACE );
		glScalef(1.0f, 0.6f, 1.0f);
		glTranslatef(0.0f, -0.05f, 0.0f);
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
	mTitle->Unbind();
	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );*/
	//glEnable( GL_FOG );
	gLevel->Fog();

	/*
	std::stringstream stream;
	stream << gPlayerScore;
	std::string score = stream.str();
	mScoreFont->RenderString( score, 0.96f - (0.035f*score.length()), 0.02f, 0.06f, 0.06f, 0.035f );

#ifdef SHOW_FPS
	std::stringstream fpsStream;
	fpsStream << (int)gFPS;
	std::string fps = fpsStream.str();
//	mScoreFont->RenderString( fps, 0.01f, 0.94f / SCR_WIDTH * SCR_HEIGHT, 0.05f, 0.05f, 0.035f );
#endif

	if ( !gPlayer )
	{
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
		gLevel->Fog();
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
	}*/
}
