#ifndef _PAUSE_H
#define _PAUSE_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/inputtingObj.h"
#include "../../engine/entity/graphicalObj.h"
#include "level.h"


class CFontRenderer;
class CTexture;
class CPause: public CEntity, public IInputtingObj, public IGraphicalObj
{
public:
	CPause();
	~CPause();

	void Update( float );
	void RenderOrtho();

private:
	
	CTimer t;
	CFontRenderer*	mFont;
	CTexture*		mTitle;
	CTexture*		mSelect;
	int				mSelected;
	int				mLevel;
};

#endif
