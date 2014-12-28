#ifndef _ENEMY_H
#define _ENEMY_H

#include "tank.h"
#include "../../engine/entity/inputtingObj.h"

#define ENEMY_OBJ_TYPE		2

class CEnemy: public CTank
{
public:
	CEnemy();
	~CEnemy();

	//void Render();
	void Update( float );

protected:
	CTank*	mTarget;
};

#endif
