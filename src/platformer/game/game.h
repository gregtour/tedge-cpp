#ifndef _GAME_H
#define _GAME_H


enum {
PLAYER_OBJ_TYPE = 1,
GROUND_OBJ_TYPE,
PLATFORM_OBJ_TYPE,
CANNON_OBJ_TYPE,
END_OBJ_TYPE
};

void HandleCollisions();

#endif
