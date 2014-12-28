#include "entity.h"
#include "../engine.h"

CEntity::~CEntity()
{
}

void CEntity::Update( float dt )
{
	return;
}

void CEntity::Destroy()
{
	gEntities.RemoveEntry(gEntities.FindEntry(this));
	delete this;
}
