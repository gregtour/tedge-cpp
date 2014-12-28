#include "../../engine/graphics/graphics.h"
#include "../../engine/sound/wavesound.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/common/timer.h"

void RenderSplash( float time, CTexture* image );
void RenderTitle( float time, CTexture* image );

void SplashScreen()
{
	gLog.LogItem( new CLogMessage("sploosh") );

	CGameTimer timer;
	timer.Clear();
	timer.Step();

	CTexture* splashImage = new CTexture( "data/solid-dh.bmp" );
	//CTexture* title = new CTexture( "data/title.bmp" );
//	gLog.LogItem( new CLogMessage("Open sound (why?)") );
	CWaveSound* splashSound = new CWaveSound( "data/opening.wav" );
	CSound* sound = new CSound( splashSound->Play( 0 ) );

	//glEnable( GL_TEXTURE_2D );
	splashImage->Bind();
	glDisable( GL_DEPTH_TEST );

//	while ( sound->IsPlaying() )
	while (timer.Time() < 6.0f)
	{
		RenderSplash( timer.Time(), splashImage );
	}

	glEnable( GL_DEPTH_TEST );
	//glDisable( GL_TEXTURE_2D );
	splashImage->Unbind();


/*
	bool going = true;
	//glEnable( GL_TEXTURE_2D );
	title->Bind();
	glDisable( GL_DEPTH_TEST );

//	while ( sound->IsPlaying() )
	while (going)
	{
SDL_Event event;

	while ( SDL_PollEvent( &event ) )
	{
		switch( event.type )
		{
		//	Window Focus	
		case SDL_ACTIVEEVENT:
			if ( event.active.gain == 0 )
				gActive = false;
			else
				gActive = true;
			break;	

		// Any of the Input Event Types 
		case SDL_MOUSEBUTTONUP:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			going = false;
			break;
		
		case SDL_QUIT:
			EndGame();
			break;
		}
	}

//	gLog.LogItem( new CLogMessage("iteration") );
		RenderTitle( timer.Time(), title );
	}

	glEnable( GL_DEPTH_TEST );
	//glDisable( GL_TEXTURE_2D );
	title->Unbind();

*/

	//delete sound;
	//delete title;
	delete splashImage;
	delete splashSound;

}

void RenderSplash( float time, CTexture* image )
{
	glMatrixMode (GL_PROJECTION);
//	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0.0, 1.0*480/272, 1.0, 0.0, -1.0, 1.0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();


	float bright = 1.0f;
	if ( time > 4.0f )
	{
		bright = (6.0f - time)/2.0f;
	} 
	else if ( time < 1.3f ) 
	{
		bright = (time-0.3f);
	}

	glColor4f( bright, bright, bright, 1.0f );		
	glClearColor(bright,bright,bright, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glTranslatef(480.0f/272.0f/2.0f-0.5f, 0.0f, 0.0f);

	glBegin( GL_TRIANGLES );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex2f( 0.0f,  0.0f );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex2f( 0.0f,  1.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex2f( 1.0f,  1.0f );


		glTexCoord2f( 0.0f, 0.0f );
		glVertex2f( 0.0f,  0.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex2f( 1.0f,  1.0f );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex2f( 1.0f,  0.0f );
	glEnd();
/*
	glBegin( GL_TRIANGLES );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex2f( 0.0f,  0.0f );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex2f( 0.0f,  1.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex2f( 1.0f,  1.0f );

		glTexCoord2f( 0.0f, 0.0f );
		glVertex2f( 0.0f,  0.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex2f( 1.0f,  1.0f );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex2f( 1.0f,  0.0f );
	glEnd();
*/
	glFinish();
	SDL_GL_SwapBuffers();
}


void RenderTitle( float time, CTexture* image )
{
	glMatrixMode (GL_PROJECTION);
//	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0.0, 1.0, 1.0, 0.0, -1.0, 1.0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );		
	//glClearColor(bright,bright,bright, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glTranslatef(480.0f/272.0f/2.0f-0.5f, 0.0f, 0.0f);

	glBegin( GL_TRIANGLES );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex2f( 0.0f,  0.0f );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex2f( 0.0f,  1.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex2f( 1.0f,  1.0f );


		glTexCoord2f( 0.0f, 0.0f );
		glVertex2f( 0.0f,  0.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex2f( 1.0f,  1.0f );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex2f( 1.0f,  0.0f );
	glEnd();
/*
	glBegin( GL_TRIANGLES );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex2f( 0.0f,  0.0f );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex2f( 0.0f,  1.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex2f( 1.0f,  1.0f );

		glTexCoord2f( 0.0f, 0.0f );
		glVertex2f( 0.0f,  0.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex2f( 1.0f,  1.0f );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex2f( 1.0f,  0.0f );
	glEnd();
*/
	glFinish();
	SDL_GL_SwapBuffers();
}
