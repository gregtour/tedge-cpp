#include "input.h"
#include "inputData.h"
#include "../entity/inputtingObj.h"
#include "../common/log.h"
#include "../engine.h"

CLinkedList<IInputtingObj>	gInputtingEntityQueue;
CInputState*			gInputState;
CBlankInputState		gBlankInputState;
//CInputState					gBlankInputState;
//CLinkedList<CKey>			gKeyList;

void MousePress( SDL_MouseButtonEvent m );
void MouseMovement( SDL_MouseMotionEvent m );
void PressKey( SDLKey k ); 
void ReleaseKey( SDLKey k ); 
void PressButton( char j ); 
void ReleaseButton( char j ); 
//void PressKey( SDL_KeyboardEvent k ); 
//void KeyStatus( SDL_KeyboardEvent k ); 
//void JoyInput( SDL_Event* e );

void InputEvent( SDL_Event* e )
{
	switch ( e->type )
	{
		case SDL_MOUSEMOTION:
			MouseMovement( e->motion );
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			MousePress( e->button );
			break;

		case SDL_JOYBUTTONDOWN:
			PressButton( (char)e->jbutton.button );
			break;
		case SDL_JOYBUTTONUP:
			ReleaseButton( (char)e->jbutton.button );
			break;
		case SDL_JOYAXISMOTION:
			if (e->jaxis.axis == 0)
				gInputState->xaxis = e->jaxis.value / 32767.0f;
			else if (e->jaxis.axis == 1)
				gInputState->yaxis = -e->jaxis.value / 32767.0f;
//			JoyInput( e );
			break;

		case SDL_KEYDOWN:
			PressKey( e->key.keysym.sym );
			break;
		case SDL_KEYUP:
			ReleaseKey( e->key.keysym.sym );
			break;		
	}
}
/*
void JoyInput( SDL_Event* e )
{
	switch ( e->type )
	{
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:



		case SDL_JOYAXISMOTION:
			JoyInput( e );
			break;
	}
}
*/
void MousePress( SDL_MouseButtonEvent m )
{

}

void MouseMovement( SDL_MouseMotionEvent m )
{
	gInputState->mousePosition.mMouseLastX = gInputState->mousePosition.mMouseX;
	gInputState->mousePosition.mMouseLastY = gInputState->mousePosition.mMouseY;
	gInputState->mousePosition.mouseTime.Step();
	gInputState->mousePosition.mMouseX = m.x;
	gInputState->mousePosition.mMouseY = m.y;
	gInputState->mousePosition.mMouseXSp = (gInputState->mousePosition.mMouseX - gInputState->mousePosition.mMouseLastX)/gInputState->mousePosition.mouseTime.DT();
	gInputState->mousePosition.mMouseYSp = (gInputState->mousePosition.mMouseY - gInputState->mousePosition.mMouseLastY)/gInputState->mousePosition.mouseTime.DT();
}

void PressKey( SDLKey k )
{
	for (int i = 0; i < MAX_MAPPINGS; i++)
	{
		if (gInputState->mappedKeys[i].keyType == KB_KEY && gInputState->mappedKeys[i].kbkey == k)
		{
			if (gInputState->mappedKeys[i].keyNumber == K_QUIT)
			{
				gRunning = false;
			}
			gInputState->PressKey(gInputState->mappedKeys[i].keyNumber);
			return;
		}
	}
}

void ReleaseKey( SDLKey k )
{
	for (int i = 0; i < MAX_MAPPINGS; i++)
	{
		if (gInputState->mappedKeys[i].keyType == KB_KEY && gInputState->mappedKeys[i].kbkey == k)
		{
			gInputState->ReleaseKey(gInputState->mappedKeys[i].keyNumber);
			return;
		}
	}

}

void PressButton( char j )
{

	for (int i = 0; i < MAX_MAPPINGS; i++)
	{
		if (gInputState->mappedKeys[i].keyType == JOY_KEY && gInputState->mappedKeys[i].joykey == j)
		{
			gInputState->PressKey(gInputState->mappedKeys[i].keyNumber);
			return;
		}
	}
}

void ReleaseButton( char j )
{
	for (int i = 0; i < MAX_MAPPINGS; i++)
	{
		if (gInputState->mappedKeys[i].keyType == JOY_KEY && gInputState->mappedKeys[i].joykey == j)
		{
			gInputState->ReleaseKey(gInputState->mappedKeys[i].keyNumber);
			return;
		}
	}

}



void BindEntityToInput( IInputtingObj* entity )
{
	CListEntry<IInputtingObj>* otherObjects = gInputtingEntityQueue.GetFirst();
	while ( otherObjects )
	{
		otherObjects->data->SetInputState( &gBlankInputState );
		otherObjects = otherObjects->GetNext();
	}

	gInputtingEntityQueue.Add( entity );
	gInputtingEntityQueue.GetLast()->data->SetInputState( gInputState );
}

void UnbindEntityToInput( IInputtingObj* entity )
{
	CListEntry<IInputtingObj>* objects = gInputtingEntityQueue.GetFirst();
	while ( objects )
	{
		if ( entity == objects->data )
		{
			gInputtingEntityQueue.RemoveEntry( objects );
			entity->SetInputState( &gBlankInputState );
			break;
		}

		objects = objects->GetNext();
	}

	if ( gInputtingEntityQueue.GetLast() )
		gInputtingEntityQueue.GetLast()->data->SetInputState( gInputState );
}
