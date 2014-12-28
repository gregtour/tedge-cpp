#ifndef _INPUTDATA_H
#define _INPUTDATA_H

#include "../common/list.h"
#include "../common/timer.h"
#include "SDL.h"

#define	NON_KEY		-1
#define KB_KEY		0
#define JOY_KEY		1

#define	MAX_MAPPINGS	20
#define MAX_KEYS	10

struct CMousePosition
{
	CMousePosition();

	int mMouseX, mMouseY;
	float mMouseXSp, mMouseYSp;
	int mMouseLastX, mMouseLastY;
	CTimer mouseTime;
};

struct CMouseButtons
{
	CMouseButtons();
};



/*
struct CPadButton
{
	int number;
	u8 buttonNumber;
	bool value;
};

struct CPadAxis
{
	int number;
	u8 axisNumber;
	u8 value;
};
*/

/*
struct CGamePadData
{
	void SetButton( u8 button, bool state );
	void SetAxis( u8 axis, u8 value );

	u8 padNumber;

	CLinkedList<CPadAxis>	axes;
	CLinkedList<CPadButton>	buttons;
};
*/

struct CMapping
{
	CMapping();
	CMapping(int kN, SDLKey kb);
	CMapping(int kN, char j, int pad);

	int keyNumber;
	
	char keyType;
	SDLKey kbkey;
	char joykey;
};

/*		Class that contains all relevant info for an object		*/
class CInputState
{
public:
	CInputState();
	
	// Mouse Data:
	CMousePosition mousePosition;
	CMouseButtons mouseButtons;

	void Update();
	// Keyboard Data:
//	CLinkedList<CKeyState> keys;
//	CLinkedList<CKey> keyPresses;

	// Gamepad Data:
//	CGamePadData gamepad;

	CMapping mappedKeys[MAX_MAPPINGS];
	int pressedKeys[MAX_KEYS];
	int pressedKeysLast[MAX_KEYS];

	virtual bool IsKeyPressed( int keyNumber );
	virtual bool IsKeyHit( int keyNumber );
	void MapKey(CMapping m);

	void PressKey(int k);
	void ReleaseKey(int k);
	float xaxis;
	float yaxis;
};

class CBlankInputState: public CInputState
{
	bool IsKeyPressed( int keyNumber)
	{
		return false;
	}	

	bool IsKeyHit(int keyNumber)
	{
		return false;
	}
};



#endif

