#include "font.h"
#include "graphics.h"

CFontRenderer::CFontRenderer( std::string fontTexture, std::string fontMask, int width, std::string fontText )
{
	mFontText = fontText;
	mFontWidth = width;
	mFontHeight = (int)((float)mFontText.length() / width - 0.001f) + 1;
	mFont = CManagedTexture::Load( &gResourceManager, fontTexture );
	mFontMask = CManagedTexture::Load( &gResourceManager, fontMask );
	SetColor(1.0f,1.0f,1.0f,1.0f);
}

void CFontRenderer::SetColor(float r, float g, float b, float a)
{
	mR = r;
	mG = g;
	mB = b;
	mA = a;
}

CFontRenderer::~CFontRenderer()
{
	CManagedTexture::Unload( &gResourceManager, mFont );
	CManagedTexture::Unload( &gResourceManager, mFontMask );
}

void CFontRenderer::RenderString( std::string text, float x, float y, float width, float height, float spacing )
{
	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable( GL_DEPTH_TEST );
	//glEnable( GL_TEXTURE_2D );

	const char* ctext = text.c_str();
	for ( int i = 0; i < (int)text.length(); i++ )
	{
		RenderCharacter( ctext[i], x + (spacing*i), y, width, height );
	}

	mFont->Unbind();
	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
	//glEnable( GL_LIGHTING );
}

void CFontRenderer::RenderCharacter( char c, float x, float y, float width, float height )
{
	unsigned int index = mFontText.find( c, 0 );
	if ( index == std::string::npos )
		return;

	int row = index / mFontWidth;
	int col = index % mFontWidth;

	int iwidth = mFont->Width();
	int iheight = mFont->Height();

	/*	Render 2 Passes, 1st the Mask and 2nd the Image	*/
	for ( int i = 0; i < 2; i++ )
	{
		if (mA < 0.99f)
			i = 1;

		if ( i == 0 )
		{
			glBlendFunc( GL_DST_COLOR, GL_ZERO );
			mFontMask->Bind();
			glColor4f( 1.0f, 1.0f, 1.0f, mA );
		} 
		else 
		{
			glBlendFunc( GL_ONE, GL_ONE );	
			mFont->Bind();
			glColor4f( mR*mA, mG*mA, mB*mA, mA );
		}

		glBegin( GL_TRIANGLES );
			
			glTexCoord2f( (float)(col) / mFontWidth - 0.5f/iwidth, (float)(row + 1) / mFontHeight - 0.5f/iheight );
			glVertex2f( x, y + height );

			glTexCoord2f( (float)(col + 1) / mFontWidth - 0.5f/iwidth, (float)(row + 1) / mFontHeight - 0.5f/iheight );
			glVertex2f( x + width, y + height );

			glTexCoord2f( (float)(col + 1) / mFontWidth - 0.5f/iwidth, (float)(row) / mFontHeight + 0.5f/iheight );
			glVertex2f( x + width, y );


			glTexCoord2f( (float)(col) / mFontWidth - 0.5f/iwidth, (float)(row + 1) / mFontHeight - 0.5f/iheight );
			glVertex2f( x, y + height );

			glTexCoord2f( (float)(col + 1) / mFontWidth - 0.5f/iwidth, (float)(row) / mFontHeight + 0.5f/iheight );
			glVertex2f( x + width, y );
			glTexCoord2f( (float)(col) / mFontWidth - 0.5f/iwidth, (float)(row) / mFontHeight + 0.5f/iheight );
			glVertex2f( x, y );
		glEnd();
	}
}

