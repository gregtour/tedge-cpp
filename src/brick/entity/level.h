#ifndef _LEVEL_H
#define _LEVEL_H

#include <string>

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"


class CTexture;
class CModel;
class I3DVERTBUF;
class CLevel: public CEntity, public IGraphicalObj
{
public:
	CLevel(const char** layout);
	~CLevel();

	void Update( float dt );
	void Render();

	void DecrementBricks() { mNumBricks--; }

protected:
	CTexture* mTexture;
	CModel*	mModel;
	float	mPulse;
	int		mNumBricks;
};

#endif
