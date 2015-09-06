#include <iostream>
#include "inputData.h"

CMousePosition::CMousePosition()
{
	mMouseX = 0;
	mMouseY = 0;
	mMouseXSp = 0.0f;
	mMouseYSp = 0.0f;
	mMouseLastX = 0;
	mMouseLastY = 0;
}

CMouseButtons::CMouseButtons()
{
}

CMapping::CMapping()
{
	keyType = NON_KEY;
}

CMapping::CMapping(int kN, SDLKey kb)
{
	keyNumber = kN;
	kbkey = kb;
	keyType = KB_KEY;
}

CMapping::CMapping(int kN, char j, int pad)
{
	keyNumber = kN;
	joykey = j;
	keyType = JOY_KEY;
}

CInputState::CInputState()
{
	xaxis = 0.0f;
	yaxis = 0.0f;
	xaxis = 0.0f;
	yaxis = 0.0f;
	for (int i = 0; i < MAX_KEYS; i++)
		pressedKeys[i] = NON_KEY;
	for (int i = 0; i < MAX_MAPPINGS; i++)
	{
		mappedKeys[i].keyType == NON_KEY;
	}
}


/*	Do a quick search for the key and see if it is currently pressed	*/
bool CInputState::IsKeyPressed( int keyNumber )
{
	for (int i = 0; i < MAX_KEYS; i++)
		if (pressedKeys[i] == keyNumber)
			return true;
	return false;
}

bool CInputState::IsKeyHit( int keyNumber )
{
	if (!IsKeyPressed(keyNumber))
		return false;
	for (int i = 0; i < MAX_KEYS; i++)
		if (pressedKeysLast[i] == keyNumber)
			return false;
	return true;
}

void CInputState::Update()
{
	//std::cout << "cinputupdate ";
	for (int i = 0; i < MAX_KEYS; i++)
		pressedKeysLast[i] = pressedKeys[i];
}

void CInputState::MapKey(CMapping m)
{
//	std::cout << "Mapping key " << m.kbkey << std::endl;
	for (int i = 0; i < MAX_MAPPINGS; i++)
	{
//		std::cout << "mappedkey[" << i << "] mapped to " << mappedKeys[i].kbkey << " key type " << mappedKeys[i].keyType << std::endl;
		if (mappedKeys[i].keyType == NON_KEY)
		{
			mappedKeys[i] = m;
//			std::cout << "Mapping key " << m.kbkey << std::endl;
			return;
		}
	}
}

void CInputState::PressKey(int k)
{
	for (int i = 0; i < MAX_KEYS; i++)
		if (pressedKeys[i] == NON_KEY)
		{	//std::cout << "key i: " << i << std::endl;
			pressedKeys[i] = k;
			return;
		}
}

void CInputState::ReleaseKey(int k)
{
	for (int i = 0; i < MAX_KEYS; i++)
		if (pressedKeys[i] == k)
		{
			pressedKeys[i] = NON_KEY;
		}
}

/*
bool CInputState::IsButtonPressed( int buttonNumber )
{
	CListEntry<CPadButton>* button = gamepad.buttons.GetFirst();
	while ( key && key->data->key.keyNumber != keyNumber )
		key = key->GetNext();

	if ( key )
	{
		if ( key->data->status.GetLast() && key->data->status.GetLast()->data->status )
		{
			return true;
		}
	}

	return false;
}
*/
/*
	int AxisValue( int axisNumber );
*/
