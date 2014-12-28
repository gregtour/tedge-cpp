#include "player.h"
#include "../game/gameInput.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/model.h"
#include "particles/explosion.h"
#include "particles/fountain.h"
#include "particles/smoke.h"

const char* PLAYER_TEXTURE_FILE = "white.png";


CParticleEmitter* particles;

CPlayer::CPlayer()
{
//	mTexture = CManagedTexture::Load(&gResourceManager, PLAYER_TEXTURE_FILE);
	mColor = SVector3(0.0f, 1.0f, 1.0f);
	//mHealth = 10000;
    flying = 0.0f;
    mTime = 0.0f;


    particles = new CSmoke(0.0f, 0.0f, 0.0f);//, 0.2f, 1.0f, 0.2f, 5.0f, (int)(dt*100));
    gEntities.Add(particles);
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update( float dt )
{
    mTime += dt;
    if (mObject)
	{
		SVector3 pos = mObject->GetPosition();
        particles->SetPosition(pos.x, pos.y > 60.0f ? pos.y : 60.0f, pos.z);

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

		if (mInputState->IsKeyPressed(K_SHOOT))
		{
	        SVector3 force;
	        force.y = RACER_SPEED * dt;
	        mObject->ApplyForce(force);

            flying += dt;
		}
        else
            {
            if (flying > 0)
                flying -= dt;
            else
                flying = 0.0f;
            }
	}

    //mNumBalloons = (int)(3 + flying);

	CRacer::Update(dt);
}


void CPlayer::Render()
    {
    CRacer::Render();
    }

