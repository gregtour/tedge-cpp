#ifndef _HUD_H
#define _HUD_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/graphics/font.h"
#include "../game/game.h"
#include "../../engine/engine.h"

class CHud: public CEntity, public IGraphicalObj
{
public:
	CHud();
	~CHud();

	void RenderOrtho();

private:
	CFontRenderer*	mScoreFont;
	CTexture* mGameOverTexture;
	CTexture* mGameOverTexture2;
	CTexture* mBarTexture;
	CTexture* mHealthTexture;
	CTexture* mMagicTexture;
	CTexture* mCircleTexture;
};

#endif
