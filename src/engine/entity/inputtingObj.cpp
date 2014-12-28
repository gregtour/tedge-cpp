#include "inputtingObj.h"

IInputtingObj::IInputtingObj()
{
	BindEntityToInput( this );
}

IInputtingObj::~IInputtingObj()
{
	UnbindEntityToInput( this );
}

void IInputtingObj::SetInputState( CInputState* state )
{
	mInputState = state;
}
