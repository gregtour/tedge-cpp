#ifndef _GAMEINPUT_H
#define _GAMEINPUT_H

/* TODO - Should handle gamepad as well */

/*	Game Input h	*/

#include "../../engine/input/input.h"
#include "../../engine/input/inputData.h"

enum GAMEKEY
{
	K_UP,
	K_DOWN,
	K_LEFT,
	K_RIGHT,
	K_SWING,
	K_BLOCK,
	K_CAST,
	K_CONTINUE,
	K_RESTART
};


int  SetupInput();


#endif
