#include "common.h"


/*	Special thanks to Kasper Fauerby */
float SignedDistanceToPlane( SVector3 point, SVector3 normal, float planeConstant )
{
	return ( point.DotProduct( normal ) ) + planeConstant;
}

SVector3 PlaneLineIntersection(SVector3 p1, SVector3 p2, SVector3 normal, float planeConstant)
{
	float u = (p1.DotProduct(normal) + planeConstant) / (normal.DotProduct(p1-p2));
	SVector3 r = p1 + (p2 - p1) * u;
	return r;
/*
	SVector3 m = p1 - p2;
	SVector3 p = m * t;

	p.DotProduct(normal) + planeConstant = 0.0f;
	m.x * normal.x + m.y * normal.y + m.z * normal.z = -planeConstant/t
	|m| cos theta = -planeConstant/t*/
}


/*	Special thanks to Kasper Fauerby */
typedef unsigned int uint32;
#define in(a) ((uint32&) a)
bool PointIsInTriangle( SVector3 point,
	SVector3 pa, SVector3 pb, SVector3 pc )
{
	SVector3 e10 = pb-pa;
	SVector3 e20 = pc-pa;
	float a = e10.DotProduct(e10);
	float b = e10.DotProduct(e20);
	float c = e20.DotProduct(e20);
	float ac_bb = ( a * c ) - ( b * b );

	SVector3 vp( point.x - pa.x, point.y - pa.y, point.z - pa.z );

	float d = vp.DotProduct(e10);
	float e = vp.DotProduct(e20);
	float x = ( d * c ) - ( e * b );
	float y = ( e * a ) - ( d * b );
	float z = x + y - ac_bb;
	return (( in(z)& ~(in(x)|in(y)) ) & 0x80000000);
}

/*	Another Borrowed Algorithm	*/
SVector3 ClosestPointOnLineSegment( SVector3 line1, SVector3 line2, SVector3 point )
{
	SVector3 c, v;
	float d, t;

	c = point - line1;
	v = line2 - line1;

	d = v.Length();
	v = v / d;			// Normalize, except dont recompute the length
	t = v.DotProduct( c );

	if ( t < 0 ) return line1;
	if ( t > d ) return line2;

	v = v * t;
	line1 = line1 + v;
	return line1;
}
