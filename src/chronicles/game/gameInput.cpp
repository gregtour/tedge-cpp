
#include "gameInput.h"


int SetupInput()
{
	/*	Create the Global Input State	*/
	gInputState = new CInputState();

	/*	Create the List of Recognized Keys	*/
	gInputState->MapKey( CMapping( K_UP, SDLK_UP ) );
	gInputState->MapKey( CMapping( K_DOWN, SDLK_DOWN ) );
	gInputState->MapKey( CMapping( K_LEFT, SDLK_LEFT ) );
	gInputState->MapKey( CMapping( K_RIGHT, SDLK_RIGHT ) );
	gInputState->MapKey( CMapping( K_START, SDLK_RETURN ) );
	gInputState->MapKey( CMapping( K_SWING, SDLK_SPACE ) );
	gInputState->MapKey( CMapping( K_JUMP, SDLK_a ) );
	gInputState->MapKey( CMapping( K_QUIT, SDLK_ESCAPE ) );

	gInputState->MapKey( CMapping( K_UP, 8, 0 ) );
	gInputState->MapKey( CMapping( K_DOWN, 6, 0 ) );
	gInputState->MapKey( CMapping( K_LEFT, 7, 0 ) );
	gInputState->MapKey( CMapping( K_RIGHT, 9, 0 ) );
	gInputState->MapKey( CMapping( K_START, 11, 0 ) );
	gInputState->MapKey( CMapping( K_SWING, 2, 0 ) );
	gInputState->MapKey( CMapping( K_JUMP, 1, 0 ) );
	gInputState->MapKey( CMapping( K_QUIT,  10, 0 ) );

	return 1;
}

