#include "player.h"
#include "../game/gameInput.h"
#include "../../engine/graphics/texture.h"

const char* PLAYER_TEXTURE_FILE = "white.png";

CPlayer::CPlayer()
{
//	mTexture = CManagedTexture::Load(&gResourceManager, PLAYER_TEXTURE_FILE);
	mColor = SVector3(0.0f, 1.0f, 0.0f);
	//mHealth = 10000;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update( float dt )
{
	if (mObject)
	{
		SVector3 pos = mObject->GetPosition();

		if ( mInputState->IsKeyPressed(K_UP) )
		{
			MoveForward(dt*1.0f);
		}

		if ( mInputState->IsKeyPressed(K_DOWN) )
		{
			MoveBackward(dt);
		}
		
		if ( mInputState->IsKeyPressed(K_RIGHT) )
		{
			TurnRight(dt);
		}

		if ( mInputState->IsKeyPressed(K_LEFT) )
		{
			TurnLeft(dt);
		}

		//if (mInputState->IsKeyHit(K_SHOOT))
		if (mInputState->IsKeyPressed(K_SHOOT))
		{
			Shoot();
		}
	}

	CTank::Update(dt);
}
