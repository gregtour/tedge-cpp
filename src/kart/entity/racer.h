#ifndef _RACER_H
#define _RACER_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/loggableObj.h"
#include "../../engine/entity/physicalObj.h"
#include "../../engine/entity/graphicalObj.h"

#define RACER_SPEED			260.0f
#define RACER_ROT_SPEED		2.5f

class CWaveSound;
class CModel;
class CTimer;
class CTexture;
class CProjectile;
class CRacer: public CEntity, public IGraphicalObj, public IPhysicalObj
{
public:
	virtual std::string	ClassName() {return "racer";}

	CRacer();
	virtual ~CRacer();

	virtual void Update( float dt );
	virtual void Render();
    void Balloons();

    void Cupcake();
	
	void Spawn(SVector3 pos = SVector3(70.0f, 250.0f, 70.0f));
	void TurnLeft(float dt);
	void TurnRight(float dt);
	void MoveForward(float dt);
	void MoveBackward(float dt);
	void Hit(CProjectile* by);
    void Follow(CRacer* behind, float dt);

protected:
    float   mTime;
    int mNumBalloons;
	CWaveSound* mEngineSound;
	CModel* mModel;
	CTexture* mTexture;
	SVector3 mColor;
};

#endif
