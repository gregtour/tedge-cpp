#include "menuInput.h"
#include "../game/gameInput.h"
#include "../main.h"
#include "../game/game.h"
#include "../../engine/engine.h"
#include "../../engine/graphics/font.h"
#include "../../engine/graphics/texture.h"
#include <sstream>


CMenuInput::CMenuInput()
{
	if (gDungeonList.GetLength() < 1)
	{
		gLog.LogItem(new CLogMessage("Error: No dungeons found in levels.txt file."));
		EndGame();
	}

	mFont = new CFontRenderer("data/font.bmp", "data/fontmask.bmp", 16, " !\"   &'() +,~./0123456789:;_  ? ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]   abcdefghijklmnopqrstuvwxyz                                   ");
	mTitle = CManagedTexture::Load(&gResourceManager, "data/chronicles.png");
	mSelect = CManagedTexture::Load(&gResourceManager, "data/sword.png");
	
	mSelected = 0;
	mLevel = 1;
	mDungeon = gDungeonList.GetFirst();

	t.Clear();
	t.Step();
}

void CMenuInput::Update( float dt )
{
	if (mInputState->IsKeyHit(K_UP))
	{
		if (mSelected > 0)
			mSelected--;
	}

	if (mInputState->IsKeyHit(K_DOWN))
	{
		if (mSelected < 3)
			mSelected++;
	}

	if (mInputState->IsKeyHit(K_LEFT))
	{
		if (mSelected == 1)
		{
			mDungeon = mDungeon->GetPrev();
			if (mDungeon == NULL)
				mDungeon = gDungeonList.GetLast();
			if (mLevel > mDungeon->data->curLevel)
				mLevel = mDungeon->data->curLevel;
		} else if (mSelected == 2) {
			if (mLevel > 1)
				mLevel--;
		}
	}

	if (mInputState->IsKeyHit(K_RIGHT))
	{
		if (mSelected == 1)
		{
			mDungeon = mDungeon->GetNext();
			if (mDungeon == NULL)
				mDungeon = gDungeonList.GetFirst();
			if (mLevel > mDungeon->data->curLevel)
				mLevel = mDungeon->data->curLevel;
		} else if (mSelected == 2) {
			if (mLevel < mDungeon->data->curLevel)
				mLevel++;
		}
	}

	if  (mInputState->IsKeyHit(K_START))
	{
		// start game
		gLevelNumber = mLevel -1;
		gCurrentDungeon = mDungeon->data;
		ChangeGameState( NEXT_LEVEL_STATE );
	}

	if (mInputState->IsKeyHit(K_SWING))
	{

		if (mSelected == 0)
		{
			// start game
			gLevelNumber = mLevel -1;
			gCurrentDungeon = mDungeon->data;
			ChangeGameState( NEXT_LEVEL_STATE );
		} else if (mSelected == 1) {
			// change dungeon
			//mDungeon = (mDungeon+1)%NUM_DUNGEONS;
			mDungeon = mDungeon->GetNext();
			if (mDungeon == NULL)
				mDungeon = gDungeonList.GetFirst();
			if (mLevel > mDungeon->data->curLevel)
				mLevel = mDungeon->data->curLevel;
		} else if (mSelected == 2) {
			// change level
			mLevel++;
			if (mLevel > mDungeon->data->curLevel)
				mLevel = 1;
		} else {
			EndGame(); 
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
void CMenuInput::RenderOrtho()
{
	glPushMatrix();
	mSelect->Bind();
		glDisable( GL_LIGHTING );
		glDisable( GL_FOG );
		glEnable( GL_BLEND );
		glDisable( GL_DEPTH_TEST );
		glDisable( GL_CULL_FACE );
		//glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(-0.3f, 0.18f+0.07f*mSelected, 0.0f);
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


	mFont->RenderString("Play", 0.3f, 0.23f, 0.08f, 0.08f, 0.035f);
	mFont->RenderString(mDungeon->data->name, 0.3f, 0.30f, 0.08f, 0.08f, 0.035f);
	std::stringstream s;
	s << "Level " << mLevel;
	mFont->RenderString(s.str(), 0.3f, 0.37f, 0.08f, 0.08f, 0.035f);
	mFont->RenderString("Quit", 0.3f, 0.44f, 0.08f, 0.08f, 0.035f);

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
	glEnable( GL_LIGHTING );
	//glEnable( GL_FOG );
	gLevel->Fog();

}
