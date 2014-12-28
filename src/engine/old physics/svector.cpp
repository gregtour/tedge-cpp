#include <math.h>
#include "common.h"

float SVector3::Length()
{
	return sqrt( x*x + y*y + z*z );
}

void  SVector3::Normalize()
{
	float len = sqrt( x*x + y*y + z*z );
	if ( len > MIN_FLOAT_VALUE )
	{
		x /= len;
		y /= len;
		z /= len;
	}
}

SVector3 SVector3::RotateXY( float degrees )
{
	float rot = atan2( y, x );
	rot += degrees * RADS;
	float len = sqrt( x*x + y*y );
	SVector3 ret;
	ret.x = cos( rot ) * len;
	ret.y = sin( rot ) * len;
	ret.z = z;
	return ret;
}

SVector3 SVector3::RotateXZ( float degrees )
{
	float rot = atan2( z, x );
	rot += degrees * RADS;
	float len = sqrt( x*x + z*z );
	SVector3 ret;
	ret.x = cos( rot ) * len;
	ret.z = sin( rot ) * len;
	ret.y = y;
	return ret;
}

SVector3 SVector3::RotateYZ( float degrees )
{
	float rot = atan2( z, y );
	rot += degrees * RADS;
	float len = sqrt( y*y + z*z );
	SVector3 ret;
	ret.y = cos( rot ) * len;
	ret.z = sin( rot ) * len;
	ret.x = x;
	return ret;
}

float SVector3::XYRotation()
{
	return DEGREES * atan2( y, x );
}

float SVector3::XZRotation()
{
	return DEGREES * atan2( z, x );
}

float SVector3::YZRotation()
{
	return DEGREES * atan2( y, z );
}
