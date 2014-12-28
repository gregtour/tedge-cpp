#ifndef _POSSE_H
#define _POSSE_H

#include "racer.h"

#define POSSE_OBJ_TYPE		2

class CPosse: public CRacer
{
public:
	CPosse(CRacer* numbahOne)
        {
        mBro = numbahOne;
        }
	~CPosse()
        {
        };

	void Update( float dt)
        {
        Follow(mBro, dt);
        }
    //void Render();

protected:
    CRacer* mBro;
};

#endif
