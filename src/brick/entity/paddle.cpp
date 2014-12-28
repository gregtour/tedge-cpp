#include "paddle.h"
#include "ball.h"
#include "../game/game.h"
#include "../game/gameInput.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/model.h"

const char* PLAYER_MODEL_FILE = "paddle.ms3d";
const char* PLAYER_TEXTURE_FILE = "white.png";
const float PADDLE_SPEED = 10.0f;

CPaddle::CPaddle(int player)
{
	mTexture = CManagedTexture::Load(&gResourceManager, PLAYER_TEXTURE_FILE);
	mModel = CManagedModel::Load(&gResourceManager, PLAYER_MODEL_FILE);
	//mColor = SVector3(0.0f, 1.0f, 0.0f);

	SSphere paddlesphere;
	paddlesphere.position = SVector3(0.0f, 0.0f, 0.0f);
	paddlesphere.radius = 3.0f;

	SBoundingBox paddlebox;
	paddlebox.max = SVector3(-2.0f, 0.0f, 0.0f);
	paddlebox.min = SVector3(2.0f, 1.0f, 1.0f);

	CreatePhysical(gWorld);
	CPObject* obj = GetPhysicalObject();
	obj->SetPosition(SVector3(20.0f, 27.0f, 0.0f));
	obj->SetObjectType(PADDLE_OBJECT_TYPE);
	obj->SetFriction(0.0f);
	obj->SetBoundingBox(paddlebox);
	obj->SetSphere(paddlesphere);
	obj->SetCollisionMask(1);
}

CPaddle::~CPaddle()
{
}

void CPaddle::Update( float dt )
{
	if (mObject)
	{
		for (CListEntry<CBall>* b = mBalls.GetFirst();
			b; b = b->GetNext())
		{
			if (b->data->GetPhysicalObject() == NULL)
			{
				CBall* ball = b->data;
				mBalls.RemoveEntry(b);
				ball->Destroy();
			}
		}

		SVector3 pos = mObject->GetPosition();
		
		if ( mInputState->IsKeyPressed(K_RIGHT) )
		{
			SVector3 force;
			force.x = PADDLE_SPEED * dt;
			mObject->ApplyForce(force);
		}

		if ( mInputState->IsKeyPressed(K_LEFT) )
		{
			SVector3 force;
			force.x = -PADDLE_SPEED * dt;
			mObject->ApplyForce(force);
		}

		if (mInputState->IsKeyHit(K_SHOOT))
		{
			if (mBalls.length == 0)
			{
				CBall* ball = new CBall(SVector3(1.0f, 1.0f, 1.0f),
										mObject->GetPosition(),
										SVector3(1.0f, 1.0f, 0.0f));
				mBalls.Add(ball);
				gEntities.Add(ball);
			}
			//Shoot();
		}
	}
}

void CPaddle::RenderOrtho()
{
	if (mObject)
	{
		SVector3 pos = mObject->GetPosition();

		glScalef(1.0f/40.0f, 1.0f/40.0f, 1.0f/40.0f);
		//glTranslatef( pos.x, pos.y, pos.z );
		glColor3f(1.0f, 1.0f, 1.0f);

		mTexture->Bind();
		mModel->Render();
		mTexture->Unbind();

		glScalef(40.0f, 40.0f, 40.0f);
	}
}

