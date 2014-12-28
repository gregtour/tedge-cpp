/**
 * common.h
 * CHRONICLES PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23 -
 *	All Rights Reserved.
**/

#ifndef _PHYSICS_COMMON_H
#define _PHYSICS_COMMON_H

#ifndef		RADS
#define		RADS			(0.017453f)
#endif
#ifndef		DEGREES
#define		DEGREES			(57.29578f)
#endif
#define		DEGS			(57.29578f)
#ifndef		PI
#define		PI				(3.141593f)
#endif
#define		HALF_PI			(1.570796f)
#ifndef		ROOT_TWO
#define		ROOT_TWO		(1.414214f)
#endif

#define MAX(a,b)	(((a) > (b)) ? (a) : (b))
#define MIN(a,b)	(((a) < (b)) ? (a) : (b))

#include "svector.h"
#include "matrix.h"

SVector3 PlaneLineIntersection(SVector3 p1, SVector3 p2, SVector3 normal, float planeConstant);
float SignedDistanceToPlane( SVector3 point, SVector3 normal, float planeConstant );
bool PointIsInTriangle( SVector3 point,	SVector3 triangle1, SVector3 triangle2, SVector3 triangle3 );
SVector3 ClosestPointOnLineSegment( SVector3 line1, SVector3 line2, SVector3 point );

struct SBoundingBox
{
	SVector3	min;
	SVector3	max;

	SVector3	GetCenter()
	{
		return (min + max) * 0.5f;
	}
};

struct SSphere
{
	SVector3	position;
	float		radius;
	float		height;
};

struct SEllipsoid
{
	SVector3	position;
	SMatrix3	eSpace;
};

/*		****************************************************	*/
inline bool BoundingBoxesIntersect( SBoundingBox box1, SBoundingBox box2 )
{
	return ( box1.max.x > box2.min.x && box1.min.x < box2.max.x && 
		box1.max.y > box2.min.y && box1.min.y < box2.max.y &&
		box1.max.z > box2.min.z && box1.min.z < box2.max.z );
}

#endif
