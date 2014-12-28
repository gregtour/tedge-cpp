#ifndef _PORTAL_H
#define _PORTAL_H

#define PORTAL_OBJECT_TYPE	9
#define PORTAL_SIZE			4.0f

#include "../../engine/entity/entity.h"
#include "../../engine/entity/physicalObj.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/sound/wavesound.h"

class CPortal: public CEntity, public IPhysicalObj, public IGraphicalObj
{
public:
	CPortal();
	~CPortal();

	void Glow();

	void Update( float dt );
	void RenderAlpha();

	void OnCreatePhysical();

private:
	CWaveSound* mWarpSound;
	CWaveSound* mPortalSound;
	CSound* mSound;
	CTexture* mPortalTexture;
	CTexture* mShineTexture;
	float mRotation;
	float mShine;

	bool mGlowing;
	float mGlow;
};

#endif
