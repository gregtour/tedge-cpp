#ifndef _TOUR_H
#define _TOUR_H

#include "../main.h"
#include "../../engine/entity/entity.h"
#include "../../engine/entity/physicalObj.h"
#include "cameraController.h"

#define ENEMY_OBJ_TYPE		2

class CTourCam: public CEntity, public IPhysicalObj
{
public:
	CTourCam(CCameraController* c, IPhysicalObj* n);
	~CTourCam();

	void Spawn();
	virtual void Update( float );

protected:
	float mPulse;
	CCameraController* cam;
	IPhysicalObj* next;

};

#endif
