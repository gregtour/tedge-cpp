#ifndef _SVECTOR_H	// avoid collision with <vector> _VECTOR_H
#define _SVECTOR_H

#define MIN_FLOAT_VALUE		(0.00000001f)

struct SVector3
{
	float x, y, z;

	SVector3() : x( 0.0f ), y( 0.0f ), z( 0.0f )
	{}
	
	SVector3( float xval, float yval, float zval ) : x( xval ), y( yval ), z( zval )
	{}

	SVector3(float vals[]) : x(vals[0]), y(vals[1]), z(vals[2])
	{}

	SVector3 operator + ( SVector3 translation )	// Translate
	{
		return SVector3( x + translation.x,
			y + translation.y, z + translation.z );
	}

	SVector3 operator - ( SVector3 translation )	// Difference
	{
		return SVector3( x - translation.x,
			y - translation.y, z - translation.z );
	}

	SVector3 operator * ( float scale )		// Scale
	{
		return SVector3( x*scale, y*scale, z*scale );
	}

	SVector3 operator / ( float scale )		// Scale
	{
		if (scale != 0.0f)
			return SVector3( x/scale, y/scale, z/scale );
		else
			return SVector3(x, y, z);
	}

	SVector3 operator * ( SVector3 scale )
	{
		return SVector3( x*scale.x, y*scale.y, z*scale.z );
	}

	SVector3 operator / ( SVector3 scale )
	{
		return SVector3( x/scale.x, y/scale.y, z/scale.z );
	}

	float		DotProduct( SVector3 arg )
	{
		return x*arg.x + y*arg.y + z*arg.z;
	}

	SVector3	CrossProduct( SVector3 arg )
	{
		return SVector3( y*arg.z - z*arg.y, z*arg.x - x*arg.z,
			x*arg.y - y*arg.x );
	}

	float Length();

	float LengthSquared()
	{
		return x*x + y*y + z*z;
	}

	float SquareLength()
	{
		return x + y + z;
	}

	void  Normalize();

	SVector3 RotateXY( float degrees );
	SVector3 RotateXZ( float degrees );
	SVector3 RotateYZ( float degrees );
	float XYRotation();
	float XZRotation();
	float YZRotation();
};


#endif	// _SVECTOR_H
