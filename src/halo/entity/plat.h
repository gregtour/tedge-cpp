#ifndef _PLATLEVEL_H
#define _PLATLEVEL_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/common/list.h"
#include "../../engine/physics/physics.h"

class IPhysicalObj;
class CPhysics;
class CTexture;
class CModel;
class I3DVERTBUF;
class CPlat: public CEntity, public IGraphicalObj
{
public:
	virtual std::string	ClassName() {return "plat";}

	CPlat(CPhysics* p);
	~CPlat();

	IPhysicalObj* LoadMS3D(const char* file, CPhysics* world);

	void Update( float dt );
	void Render();


	enum MONSTERTYPE
	{
		M_GREMLING,
		M_BAT,
		M_BOSS,
		M_GHOST
	};
	void	CreateMonster(SVector3 pos, MONSTERTYPE type);
	void	AddMonster(SVector3 pos, MONSTERTYPE type);

protected:

	CTexture* mTexture;
    CPhysics*               mWorld;
	CPMesh*					mPlatform;
	CLinkedList<I3DVERTBUF>	mVertBufs;
	CLinkedList<CTexture>	mMeshTextures;
};

#endif
