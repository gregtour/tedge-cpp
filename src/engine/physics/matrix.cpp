#include "matrix.h"


SMatrix3::SMatrix3()
{
	for ( int r = 0; r < 3; r++ )
		for ( int c = 0; c < 3; c++ )
			m[r][c] = 0.0f;
}

//	Axis Vectors
SMatrix3::SMatrix3( SVector3 x, SVector3 y, SVector3 z )
{
	// X Axis Vector
	m[0][0] = x.x;
	m[0][1] = x.y;
	m[0][2] = x.z;
	// Y Axis Vector
	m[1][0] = y.x;
	m[1][1] = y.y;
	m[1][2] = y.z;
	// Z Axis Vector
	m[2][0] = z.x;
	m[2][1] = z.y;
	m[2][2] = z.z;

/*
	// X Axis Vector
	m[0][0] = 1.0f / x.x;
	m[0][1] = 1.0f / x.y;
	m[0][2] = 1.0f / x.z;
	// Y Axis Vector
	m[1][0] = 1.0f / y.x;
	m[1][1] = 1.0f / y.y;
	m[1][2] = 1.0f / y.z;
	// Z Axis Vector
	m[2][0] = 1.0f / z.x;
	m[2][1] = 1.0f / z.y;
	m[2][2] = 1.0f / z.z;*/
}


SMatrix3::SMatrix3( float* coordinates )
{
	for ( int r = 0; r < 3; r++ )
		for ( int c = 0; c < 3; c++ )
			m[r][c] = coordinates[r*3 + c];
}

SVector3 SMatrix3::operator * ( SVector3 multiple )
{
	SVector3 ret;
	//	Axis Vector			X					Y						Z		Component
	ret.x = multiple.x * m[0][0] + multiple.y * m[1][0] + multiple.z * m[2][0];	//	X
	ret.y = multiple.x * m[0][1] + multiple.y * m[1][1] + multiple.z * m[2][1];	//	Y
	ret.z = multiple.x * m[0][2] + multiple.y * m[1][2] + multiple.z * m[2][2];	//	Z
	return ret;
}

SMatrix3 SMatrix3::operator * ( SMatrix3 mult )
{
	SMatrix3 ret;
	for ( int i = 0; i < 3; i++ )
	{		//Cycle through each vector of first matrix.
		ret.m[i][0] = m[i][0] * mult.m[0][0] +
			m[i][1] * mult.m[1][0] + m[i][2] * mult.m[2][0];
		ret.m[i][1] = m[i][0] * mult.m[0][1] +
			m[i][1] * mult.m[1][1] + m[i][2] * mult.m[2][1];
		ret.m[i][2] = m[i][0] * mult.m[0][2] +
			m[i][1] * mult.m[1][2] + m[i][2] * mult.m[2][2];
	}
	return ret;
}

SMatrix3 SMatrix3::FastInverse()
{
	SMatrix3 ret;
	ret.m[0][0] = m[0][0];
	ret.m[0][1] = m[1][0];
	ret.m[0][2] = m[2][0];
	ret.m[1][0] = m[0][1];
	ret.m[1][1] = m[1][1];
	ret.m[1][2] = m[2][1];
	ret.m[2][0] = m[0][2];
	ret.m[2][1] = m[1][2];
	ret.m[2][2] = m[2][2];
	return ret;
}

