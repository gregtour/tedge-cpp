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


class CLevel: public CEntity, public IGraphicalObj
{
public:
	CLevel();
	CLevel( char* level );
	~CLevel();

	void LoadLevel();
	void FakeLoad();
	void Init();
	void Shutdown();

	void Update( float dt );
	void Render();
	void Lighting();
	void Light( float x, float y, float z );
	void Light( float x, float z );
	void Fog();

public:
	// Physical
	CPhysics* mWorld;

	CLinkedList<CCamera>	mCameras;
	CLinkedList<CCameraController>	mCameraControllers;
	CCamera* mBoundCamera;
	CLinkedList<CLight>		mLights;

	// Fog
	bool mFog;
	float mFogColorR, mFogColorG, mFogColorB, mFogColorA;
	float mFogDensity, mFogStart, mFogEnd;

	// Player
	float mPlayerX, mPlayerY, mPlayerZ, mPlayerRot;
	float mPlayerRadius, mPlayerBounce, mPlayerMass, mPlayerFriction;
	bool mPlayerDynamic;

	// Objects
	CLinkedList<CEntity>	mObjectsData;
	I3DVERTBUF* mVertBuf;
	I3DINDXBUF*	mIndxBuf;
	//SIMPMESH	mLevelMesh;
	CTexture* mTestTexture;
};

#endif
