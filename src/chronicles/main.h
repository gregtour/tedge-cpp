#ifndef _MAIN_H
#define _MAIN_H

#include "../engine/engine.h"
#include "../engine/physics/physics.h"
#include "entity/level.h"
#include "entity/player.h"

//void StartLevel(SDungeonEntry* d, int num);

extern CPlayer*						gPlayer;
extern CLevel*						gLevel;
extern int							gLevelNumber;
extern CLinkedList<SDungeonEntry>	gDungeonList;
extern SDungeonEntry*				gCurrentDungeon;

#define NEXT_LEVEL_STATE	1
#define MAIN_MENU_STATE		2
#define PAUSE_STATE			3

#endif
