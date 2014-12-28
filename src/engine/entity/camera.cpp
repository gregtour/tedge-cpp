
#include "camera.h"
#include "../graphics/graphics.h"

CCamera::CCamera()
{
	SetPosition( 0.0f, 0.0f, 0.0f );
	LookAt( 0.0f, 0.0f, 1.0f );
	SetUp( 0.0f, 1.0f, 0.0f );
}

CCamera::~CCamera()
{
	if ( this == gCamera )
	{
		BindCamera( NULL );
	}
}

void CCamera::SetPosition( float x, float y, float z )
{
	mX = x;
	mY = y;
	mZ = z;
}

void CCamera::LookAt( float x, float y, float z )
{
	mLookX = x;
	mLookY = y;
	mLookZ = z;
}

void CCamera::SetUp( float x, float y, float z )
{
	mUpX = x;
	mUpY = y;
	mUpZ = z;
}

void CCamera::Update( float dt )
{
	return;
}

void CCamera::Render()
{
	glLoadIdentity();
	gluLookAt( mX, mY, mZ, mLookX, mLookY, mLookZ, mUpX, mUpY, mUpZ );
}

SVector3 CCamera::GetDirection()
    {
    return SVector3(mLookX - mX, mLookY - mY, mLookZ - mZ);
    }

void CCamera::GetLookAt( float *x, float *y, float *z )
{
	*x = mLookX;
	*y = mLookY;
	*z = mLookZ;
}

void CCamera::GetPosition( float *x, float *y, float *z )
{
	*x = mX;
	*y = mY;
	*z = mZ;
}
