#ifndef _MAIN_H
#define _MAIN_H

#include "../engine/engine.h"
#include "../engine/physics/physics.h"
#include "entity/level.h"
#include "entity/player.h"

extern CPlayer*	gPlayer;
extern CLevel*	gLevel;
extern int gLevelNumber;

#define NEXT_LEVEL_STATE	1

#endif
