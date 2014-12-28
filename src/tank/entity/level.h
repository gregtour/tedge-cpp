#ifndef _LEVEL_H
#define _LEVEL_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"

#define LEVEL_SIZE		100

class CTexture;
class CModel;
class I3DVERTBUF;
class CLevel: public CEntity, public IGraphicalObj
{
public:
	CLevel();
	~CLevel();

//	void SetupLevel();

	void Update( float dt );
	void Render();

protected:
	CTexture* mTexture;
	CModel*	mModel;
	CModel*	mBlocks;
	I3DVERTBUF*	mTempVert;
	float	mPulse;
};

#endif
