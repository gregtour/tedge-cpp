#ifndef _TANK_H
#define _TANK_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/loggableObj.h"
#include "../../engine/entity/physicalObj.h"
#include "../../engine/entity/graphicalObj.h"

#define TANK_SPEED			40.0f
#define TANK_ROT_SPEED		2.5f
#define TANK_SHOOT_TIME		0.125f
#define TANK_RESPAWN_TIME	1.0f

class CWaveSound;
class CModel;
class CTimer;
class CTexture;
class CBullet;
class CTank: public CEntity, public IGraphicalObj, public IPhysicalObj
{
public:
	virtual std::string	ClassName() {return "tank";}

	CTank();
	virtual ~CTank();

	virtual void Update( float dt );
	virtual void Render();
	
	void Spawn();
	void Shoot();
	void TurnLeft(float dt);
	void TurnRight(float dt);
	void MoveForward(float dt);
	void MoveBackward(float dt);
	void Hit(CBullet* by);
	void AddHealth();
	virtual void Explode();

protected:
	int mHealth;
	CTimer mLastShoot;
	CTimer mRespawnTime;
	CWaveSound* mShootSound;
	CWaveSound* mHitSound;
	CWaveSound* mExplodeSound;
	CModel* mModel;
	CTexture* mTexture;

	float	mPulse;

	int	mCannon;

	SVector3 mColor;
};

#endif
