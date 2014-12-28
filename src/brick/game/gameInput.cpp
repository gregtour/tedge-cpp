
#include "gameInput.h"


int SetupInput()
{
	/*	Create the Global Input State	*/
	gInputState = new CInputState();

	/*	Create the List of Recognized Keys	*/
	gInputState->MapKey( CMapping( K_LEFT, SDLK_LEFT ) );
	gInputState->MapKey( CMapping( K_RIGHT, SDLK_RIGHT ) );
	gInputState->MapKey( CMapping( K_SHOOT, SDLK_SPACE ) );
	gInputState->MapKey( CMapping( K_QUIT, SDLK_ESCAPE ) );

	gInputState->MapKey( CMapping( K_LEFT, 7, 0 ) );
	gInputState->MapKey( CMapping( K_RIGHT, 9, 0 ) );
	gInputState->MapKey( CMapping( K_SHOOT, 2, 0 ) );
	gInputState->MapKey( CMapping( K_QUIT,  10, 0 ) );

	return 1;
}

