#ifndef _ITEM_H
#define _ITEM_H

/*
	Item -
		An Item is a physical/graphical object in the game world
		It may have several triggers tied to it for Contact or Use by
		Players, Characters, or other Objects (items?)
		Examples: Soccer Ball, Crate, Tree, Chest, etc
*/


#include "../../engine/entity/entity.h"
#include "../../engine/entity/loggableObj.h"
#include "../../engine/entity/physicalObj.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/graphics/texture.h"

#define ITEM_OBJ_TYPE	2

class CItem: public CEntity, public IGraphicalObj, public IPhysicalObj
{
public:
	CItem();
	virtual ~CItem();

	virtual void Update( float dt );
	virtual void Render();

protected:
	CTexture* mTexture;
};

#endif

