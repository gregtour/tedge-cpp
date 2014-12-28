#ifndef _INPUT_H
#define _INPUT_H

#include "SDL.h"
#include "inputData.h"

class IInputtingObj;

#define	K_QUIT	99

void InputEvent( SDL_Event* e );
void BindEntityToInput( IInputtingObj* entity );
void UnbindEntityToInput( IInputtingObj* entity );

extern CInputState*			gInputState;

#endif
