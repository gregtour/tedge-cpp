#ifndef _CAMERA_H
#define _CAMERA_H

#include "entity.h"
#include "../physics/svector.h"

class CCamera : public CEntity
{
public:
	CCamera();
	~CCamera();

	void SetPosition( float x, float y, float z );
	void LookAt( float x, float y, float z );
	void SetUp( float x, float y, float z );

	void GetPosition( float* x, float* y, float* z );
	void GetLookAt( float *x, float *y, float *z );
	SVector3 GetPosition()
	{
		return SVector3(mX, mY, mZ);
	}
	SVector3 GetLookAt()
	{
		return SVector3(mLookX, mLookY, mLookZ);
	}

    SVector3 GetDirection();

	void Update( float );
	void Render();

protected:
	float mX, mY, mZ;
	float mLookX, mLookY, mLookZ;
	float mUpX, mUpY, mUpZ;
};

#endif
