#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/loggableObj.h"
#include "../../engine/entity/physicalObj.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/common/timer.h"
#include "../../engine/sound/wavesound.h"

#define MIN_HIT_TIME	0.6f

class CCharacter: public CEntity, public IGraphicalObj, public IPhysicalObj
{
public:
	CCharacter();
	virtual ~CCharacter();
	
	virtual void LoadHitSound();

	virtual void DeathEffect();
	int GetHealth();
	void SetHealth( int h );
	virtual bool Hit( CCharacter* byWhom, int damage );
	virtual void Die();

	virtual void Update( float dt );
	virtual void Render();
	virtual SVector3 GetPosition();

	float GetUpRot();

	void Collision( IPhysicalObj* o );

protected:
	float mUpRot;
	int mHealth;
	CTimer mLastHit;
	CWaveSound* mHitSound;
};

#endif
