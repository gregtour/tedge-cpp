#ifndef _HEALTH_H
#define _HEALTH_H

#include "pickup.h"

class CRacer;
class CHealth: public CPickup
{
public:
	CHealth();
	~CHealth();

	void Update( float );
	void Render();
	void PickedUp(CRacer* by);

    static int mMagic;

protected:
    float mTime;
};

#endif
