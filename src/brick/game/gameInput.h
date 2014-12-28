#ifndef _GAMEINPUT_H
#define _GAMEINPUT_H

/* TODO - Should handle gamepad as well */

/*	Game Input h	*/

#include "../../engine/input/input.h"
#include "../../engine/input/inputData.h"

enum GAMEKEY
{
	K_LEFT = 0,
	K_RIGHT,
	K_SHOOT,
	NUM_KEYS
};


int  SetupInput();


#endif
