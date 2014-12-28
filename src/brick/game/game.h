#ifndef _GAME_H
#define _GAME_H

enum {
	PADDLE_OBJECT_TYPE = 1,
	BALL_OBJECT_TYPE,
	BRICK_OBJECT_TYPE 
};

void HandleCollisions();

#endif
