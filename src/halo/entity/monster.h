#ifndef _MONSTER_H
#define _MONSTER_H

#include "../main.h"
#include "character.h"
#include "../../engine/graphics/texture.h"

#define	MONSTER_OBJ_TYPE	3

class CMonster: public CCharacter
{
public:
	CMonster();
	virtual ~CMonster();

	virtual int AttackStrength();

	void Update( float );
	void Render();
protected:
	virtual void LoadTexture();
	CTexture* mTexture;
};

#endif
