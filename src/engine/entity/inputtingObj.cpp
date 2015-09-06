#include "inputtingObj.h"
#include <iostream>
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
	std::cout << "set input state" << std::endl;
	mInputState = state;
}
