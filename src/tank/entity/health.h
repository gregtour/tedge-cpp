#ifndef _HEALTH_H
#define _HEALTH_H

#include "pickup.h"

class CTank;
class CHealth: public CPickup
{
public:
	CHealth();
	~CHealth();

	//void Update( float );
	
	void PickedUp(CTank* by);

protected:
};

#endif
