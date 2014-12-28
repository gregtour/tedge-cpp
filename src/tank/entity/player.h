#ifndef _PLAYER_H
#define _PLAYER_H

#include "tank.h"
#include "../../engine/entity/inputtingObj.h"

#define PLAYER_OBJ_TYPE		1

class CPlayer: public CTank, public IInputtingObj
{
public:
	CPlayer();
	~CPlayer();

	void Update( float );

protected:
};

#endif
