#ifndef _PLAYER_H
#define _PLAYER_H

#include "character.h"
#include "../../engine/entity/inputtingObj.h"
#include "../../engine/sound/wavesound.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/buffer/I3dVertBuf.h"
#include "../../engine/graphics/buffer/I3dIndxBuf.h"

#define PLAYER_SPEED		120.0f
#define PLAYER_ROT_SPEED	1.0f
#define PLAYER_OBJ_TYPE		1

class CPlayer: public CCharacter, public IInputtingObj
{
public:
	CPlayer();
	~CPlayer();

	void Render();
	void Update( float );
	CMousePosition GetMouse();

protected:
	void Load();
	I3DVERTBUF* mVertBuf;
	I3DINDXBUF*	mIndxBuf;
	CTexture* mTestTexture;
	bool loaded;
};

#endif
