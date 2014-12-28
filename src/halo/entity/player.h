#ifndef _PLAYER_H
#define _PLAYER_H

#include "racer.h"
#include "../../engine/entity/inputtingObj.h"

#define PLAYER_OBJ_TYPE		1

class CPlayer: public CRacer, public IInputtingObj
{
public:
	virtual std::string	ClassName() {return "playa";}    

	CPlayer();
	~CPlayer();

	void Update( float );
    void Render();
    SVector3 GetPosition() { return CPlayer::GetPhysicalObject()->GetPosition(); }

protected:
    float   flying;
};

extern CPlayer* gPlayer;

#endif
