#ifndef _LEVEL_H
#define _LEVEL_H

#include "../../engine/graphics/buffer/I3dVertBuf.h"
#include "../../engine/graphics/buffer/I3dIndxBuf.h"
//#include "../../engine/graphics/buffer/SimpleMesh.h"

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/common/list.h"
#include "../../engine/physics/physics.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/font.h"
#include "../../engine/entity/camera.h"
#include "cameraController.h"
#include "../../engine/graphics/lighting.h"
#include "../../engine/common/timer.h"


extern float gTrackLength;

struct SDungeonEntry
{
std::string name;
std::string path;
int numLevels;
int curLevel;
};


//class MilkshapeModel;
class msModel;

class CLevel: public CEntity, public IGraphicalObj
{
public:
	CLevel();
	//CLevel( char* level );
	~CLevel();

	void LoadGameMenu();
	void LoadLevel(std::string path, int num);
	void Init();
	void Shutdown();

	void Update( float dt );
	void Render();
	void Lighting();
	void Light( float x, float y, float z );
	void Light( float x, float z );
	void Light( SVector3 p );
	void Fog();
	
	void CreatePortal();

	void LoadMS3D(char* file);

	CTexture* mLevelText;
	CTexture* mLevelMask;
	CFontRenderer*	mLevelFont;
	CTimer mLevelTextTimer;

    enum MONSTERTYPE {
        M_GREMLING = 1,
        M_BAT,
        M_GHOST,
        M_BOSS
    };

public:

    void CreateMonster(SVector3 pos, CLevel::MONSTERTYPE type);
    void AddMonster(SVector3 pos, CLevel::MONSTERTYPE type);

	// Physical
	CPhysics* mWorld;

	CLinkedList<CCamera>	mCameras;
	CCameraController*		mCameraController;
	CCamera*				mBoundCamera;
	CLinkedList<CLight>		mLights;

	// Fog
	bool mFog;
	float mFogColorR, mFogColorG, mFogColorB, mFogColorA;
	float mFogDensity, mFogStart, mFogEnd;

	// Player
	float mPlayerX, mPlayerY, mPlayerZ, mPlayerRot;
	float mPlayerRadius, mPlayerBounce, mPlayerMass, mPlayerFriction;
	bool mPlayerDynamic;

	bool mPortal;	// Whether or not the portal initially exists
	float mPortalX, mPortalY, mPortalZ;

	// Objects
	CLinkedList<CEntity>	mObjectsData;
	//CLinkedList<BUFFEREDMESH>	mMeshes;

	/*I3DVERTBUF* mVertBuf;
	I3DINDXBUF*	mIndxBuf;
	I3DVERTBUF* mVertBufFloor;
	I3DINDXBUF*	mIndxBufFloor;*/
	//SIMPMESH	mLevelMesh;
	//CTexture* mTestTexture;

	msModel*	mLevelModel;
	CLinkedList<I3DVERTBUF>	mVertBufs;
	CLinkedList<CTexture>	mMeshTextures;

	static I3DVERTBUF*	gTempVert;

	CTexture*		mSky;
};

#endif
