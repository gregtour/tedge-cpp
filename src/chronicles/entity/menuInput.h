#ifndef _MENUINPUT_H
#define _MENUINPUT_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/inputtingObj.h"
#include "../../engine/entity/graphicalObj.h"
#include "level.h"

class CFontRenderer;
class CTexture;
class CMenuInput: public CEntity, public IInputtingObj, public IGraphicalObj
{
public:
	CMenuInput();

	void Update( float );
	void RenderOrtho();

private:
	
	CTimer t;
	CFontRenderer*	mFont;
	CTexture*		mTitle;
	CTexture*		mSelect;
	int				mSelected;
	int				mLevel;
	CListEntry<SDungeonEntry>* mDungeon;
};

#endif
