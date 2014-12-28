
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
	gInputState->MapKey( CMapping( K_SHOOT, SDLK_SPACE ) );
	gInputState->MapKey( CMapping( K_DRIFT, SDLK_LSHIFT ) );
	gInputState->MapKey( CMapping( K_QUIT, SDLK_ESCAPE ) );

	/* Gamepad Mappings */
	gInputState->MapKey( CMapping( K_UP, 8, 0 ) );
	gInputState->MapKey( CMapping( K_DOWN, 6, 0 ) );
	gInputState->MapKey( CMapping( K_LEFT, 7, 0 ) );
	gInputState->MapKey( CMapping( K_RIGHT, 9, 0 ) );
	gInputState->MapKey( CMapping( K_SHOOT, 2, 0 ) );
	gInputState->MapKey( CMapping( K_DRIFT, 3, 0 ) );
	gInputState->MapKey( CMapping( K_QUIT,  10, 0 ) );

	return 1;
}

