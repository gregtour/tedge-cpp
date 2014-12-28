#ifndef _LEVEL_H
#define _LEVEL_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/common/list.h"
#include "../../engine/physics/physics.h"

class IPhysicalObj;
class CPhysics;
class CTexture;
class CModel;
class I3DVERTBUF;
class CLevel: public CEntity, public IGraphicalObj
{
public:
	CLevel();
	~CLevel();

	IPhysicalObj* LoadMS3D(const char* file, CPhysics* world);

	void Update( float dt );
	void Render();

protected:
	CTexture* mTexture;

	CPMesh*					mPlatform;
	CLinkedList<I3DVERTBUF>	mVertBufs;
	CLinkedList<CTexture>	mMeshTextures;
};

#endif
