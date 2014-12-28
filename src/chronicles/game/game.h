#ifndef _GAME_H
#define _GAME_H

#define HIT_ENEMY_POINTS	5
#define KILLED_ENEMY_POINTS	50
#define POINTS_TO_CONTINUE	1000

#define NUM_OBJ_TYPES	16

void HandleCollisions();

/*	Player "stats" */
extern int gPlayerScore;
extern int gPlayerSwordLevel;

void InitPlayerStats();

#endif
