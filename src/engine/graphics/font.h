#ifndef _FONT_H
#define _FONT_H

#include "texture.h"
#include "../engine.h"
#include <string>

class CFontRenderer
{
public:
	CFontRenderer( std::string fontTexture, std::string fontMask, int width, std::string fontText );
	~CFontRenderer();

	void RenderString( std::string text, float x, float y, float width, float height, float spacing );

	void SetColor(float r, float g, float b, float a);
private:
	void RenderCharacter( char c, float x, float y, float width, float height );

	CTexture* mFont;
	CTexture* mFontMask;
	int mFontWidth, mFontHeight;
	std::string mFontText;
	float mR, mG, mB, mA;
};


#endif
