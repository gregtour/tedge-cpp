#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>

class CEntity
{
public:
	virtual std::string	ClassName() {return "entity";}

	virtual ~CEntity();

	virtual void Update( float dt );

	void Destroy();
};

#endif
