#include "player.h"
#include "../game/gameInput.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/model.h"
#include <cmath>

CPlayer::CPlayer()
{
	mModel = CManagedModel::Load(&gResourceManager, "player.ms3d");
	mHasJumped = false;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update( float dt )
{
	if (mObject)
	{
		SVector3 pos = mObject->GetPosition();
		SVector3 cam = gCamera->GetPosition();

		if (pos.y < -30.0f)
			ResetPosition();

		// Analog stick
		float push = mInputState->xaxis * mInputState->xaxis + mInputState->yaxis * mInputState->yaxis;
		if (push > (0.2f) * (0.2f))
		{
			SVector3 distance = pos - cam;
			push = sqrt(push);
			float ang = mRot - atan2(distance.z, distance.x) + atan2(mInputState->yaxis, mInputState->xaxis);
			float cosAng = cos(ang);
			float sinAng = sin(ang);

			if (sinAng < -0.35f)
				MoveForward(-sinAng * push * dt);
			else if (sinAng > 0.4f)
				MoveBackward(sinAng * push * dt);

			if (fabs(cosAng) > 0.1f)
				TurnLeft(cosAng * push * dt * 1.5f);
			/*float angDif = ang + PI/2.0f;
			while (angDif > PI)
				angDif -= PI*2.0f;
			while (angDif < -PI)
				angDif += PI*2.0f;
			TurnLeft(angDif * dt);*/

		// push = push_angle + cam_angle
		// cos(push_ang + cam_ang - facing)
		// moveForward( dt * analog_push * push . facing )
		//float theta = atan2(ang.z, ang.x);
		}


		// Digital Input
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

		if (mInputState->IsKeyHit(K_JUMP) && !mHasJumped)
		{
			Jump();
		}
	}

	SetGrounded(false);
}

void CPlayer::Render()
{
	if (mObject)
	{
		SVector3 pos = mObject->GetPosition();
		{
			float x, y, z;
			gCamera->GetPosition( &x, &y, &z );
			if ( (x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y) + (z-pos.z)*(z-pos.z) > VIEW_DISTANCE_SQUARED )
				return;
		}

		glTranslatef( pos.x, pos.y, pos.z );
		glRotatef(mRot * DEGREES, 0.0f, -1.0f, 0.0f );

		glColor3f(0.4f, 0.0f, 0.0f);
		mModel->Render();
		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

void CPlayer::TurnRight(float dt)
{
	mRot += 2.0f * dt;
}

void CPlayer::TurnLeft(float dt)
{
	TurnRight(-dt);
}

void CPlayer::MoveForward(float dt)
{
	SVector3 force;
	force.x = -cos(mRot) * 180.0f * dt;
	force.z = -sin(mRot) * 180.0f * dt;
	mObject->ApplyForce(force);
}

void CPlayer::MoveBackward(float dt)
{
	MoveForward(dt * -0.7f);
}

void CPlayer::SetGrounded(bool grounded)
{
	if (grounded)
		mHasJumped = false;
	mGrounded = grounded;
}

void CPlayer::SetSafe()
{
	mSafePos = mObject->GetOldPosition();
    mGrounded = true;
    mHasJumped = false;
}

void CPlayer::Jump()
{
	if (mGrounded)
	{
		mHasJumped = true;
		SVector3 force;
		force.y = 43.0f;
		mObject->ApplyForce(force);
	}
}

void CPlayer::ResetPosition()
{
	mObject->SetPosition(mSafePos);
	mObject->SetVelocity(SVector3(0.0f, 0.0f, 0.0f));
}
