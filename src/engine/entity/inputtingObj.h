#ifndef _INPUTTINGOBJ_H
#define _INPUTTINGOBJ_H

#include "../input/input.h"
#include "../input/inputData.h"

#ifndef NULL
	#define NULL	0L
#endif

class IInputtingObj
{
public:
	IInputtingObj();
	virtual ~IInputtingObj();
	void SetInputState( CInputState* );

protected:
	CInputState* mInputState;
};

#endif
