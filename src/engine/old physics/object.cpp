/**
 * Object.cpp
 * CHRONICLES PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23 -
 *	All Rights Reserved.
**/

#include <math.h>
#include "common.h"
#include "object.h"

#ifndef NULL
#define NULL 0
#endif

CPObject::CPObject()
{
	CPObject(true);
}

CPObject::CPObject( bool dynamic )
{
	mDynamic = dynamic;
//	mMass = 0.0f;
	mObjectType = 0;
	mCollisionMask = 0;
	mOwner = NULL;
//	mBounce = false;
//	mBounceEfficiency = 0.0f;
	mFriction = 1.0f;
	mShape = CP_SIMPLE;
}

CPObject::~CPObject()
{
}
	
void CPObject::Update( float dt )
{
	mOldPosition = mPosition;

	//mForce = SVector3(0.0f, 0.0f, 0.0f);
//	mForce = mDefaultForce * dt;

	/* Default Force [ie. Gravity or Buoyancy]	*/
	mVelocity = mVelocity + (mDefaultForce * dt);
	
	/* Friction	*/
	/*float frictionScale = 1.0f - dt * mFriction;
	float frictionScaleY = 1.0f - dt * mFriction * 0.33f;
	if ( frictionScale < 0.0f )
		frictionScale = 0.0f;
	if ( frictionScaleY < 0.0f )
		frictionScaleY = 0.0f;
	mVelocity.x = mVelocity.x * frictionScale;
	mVelocity.z = mVelocity.z * frictionScale;
	mVelocity.y = mVelocity.y * frictionScaleY;*/
	mVelocity.x = mVelocity.x + (mSurfaceVelocity.x - mVelocity.x) * mFriction * dt;
	mVelocity.y = mVelocity.y + (mSurfaceVelocity.y - mVelocity.y) * mFriction * dt * 0.2f;
	mVelocity.z = mVelocity.z + (mSurfaceVelocity.z - mVelocity.z) * mFriction * dt;

	//SetSurfaceVelocity(SVector3(0.0f, mVelocity.y, 0.0f));

	/*	Movement	*/
	mPosition = mPosition + (mVelocity * dt);
}

void	CPObject::SetPosition( SVector3 p )
{
	mPosition = p;
}

SVector3 CPObject::GetPosition()
{
	return mPosition;
}

SVector3 CPObject::GetOldPosition()
{
	return mOldPosition;
}

void	CPObject::SetVelocity( SVector3 v )
{
	mVelocity = v;
}
	
SVector3 CPObject::GetVelocity()
{
	return mVelocity;
}




void	CPObject::SetSphere( SSphere s )
{
	mSphere = s;
}

SSphere CPObject::GetSphere()
{
	SSphere ret = mSphere;
	ret.position = ret.position + mPosition;
	return ret;
}

SSphere CPObject::GetOldSphere()
{
	SSphere ret = mSphere;
	ret.position = ret.position + mOldPosition;
	return ret;
}





void	CPObject::SetBoundingBox( SBoundingBox b )
{
	mBoundingBox = b;
}

SBoundingBox CPObject::GetBoundingPath()
{
	SBoundingBox b1 = mBoundingBox;
	SBoundingBox b2 = mBoundingBox;
	SBoundingBox ret;
	b1.min = b1.min + mPosition;
	b1.max = b1.max + mOldPosition;
	b2.min = b2.min + mOldPosition;
	b2.max = b2.max + mOldPosition;
	
	ret.min.x = MIN(b1.min.x, b2.min.x);
	ret.min.y = MIN(b1.min.y, b2.min.y);
	ret.min.z = MIN(b1.min.z, b2.min.z);

	ret.max.x = MIN(b1.max.x, b2.max.x);
	ret.max.y = MIN(b1.max.y, b2.max.y);
	ret.max.z = MIN(b1.max.z, b2.max.z);
	return ret;
}

SBoundingBox CPObject::GetBoundingBox()
{
	SBoundingBox ret = mBoundingBox;
	ret.min = ret.min + mPosition;
	ret.max = ret.max + mPosition;
	return ret;
}
/*
void	CPObject::SetMass( float m )
{
	mMass = m;
}

float	CPObject::GetMass()
{
	return mMass;
}

void	CPObject::SetBounce( bool b )
{
	mBounce = b;
}

bool	CPObject::GetBounce()
{
	return mBounce;
}

void	CPObject::SetBounceEfficiency( float b )
{
	mBounceEfficiency = b;
}

float	CPObject::GetBounceEfficiency()
{
	return mBounceEfficiency;
}
*/
void	CPObject::SetOwner( IPhysicalObj* o )
{
	mOwner = o;
}

IPhysicalObj*	CPObject::GetOwner()
{
	return mOwner;
}

void	CPObject::SetObjectType( int type )
{
	mObjectType = type;
}

int		CPObject::GetObjectType()
{
	return mObjectType;
}

void	CPObject::SetCollisionMask( int mask )
{
	mCollisionMask = mask;
}

int		CPObject::GetCollisionMask()
{
	return mCollisionMask;
}
/*	
CPShape	CPObject::GetShape()
{
	return CP_OBJECT;
}
*/	
void CPObject::SetFriction( float f )
{
	mFriction = f;
}

void CPObject::SetDefaultForce( SVector3 force )
{
	mDefaultForce = force;
}

void CPObject::SetSurfaceVelocity(SVector3 v)
{
	mSurfaceVelocity = v;
}
	/*
bool	CPObject::IsDynamic()
{
	return mDynamic;
}
*/
void	CPObject::ApplyForce( SVector3 force )
{
	mForce = mForce + force;
	mVelocity = mVelocity + force;
}

void	CPObject::Translate( SVector3 translation )
{
	mPosition = mPosition + translation;
}


/* ********	CPMesh	************************************************************	*/

CPMesh::CPMesh()
{
	CPObject(true);
	mShape = CP_MESH;
	mRot = 0.0f;
}

CPMesh::~CPMesh()
{

}

void CPMesh::Add( CPTriangle* o )
{
	for (int j = 0; j < 3; j++)
	{
		SVector3 p = o->GetPoint(j);
		if (mTris.length == 0 && j == 0)
		{
			mBoundingBox.min = SVector3(p.x - TRIANGLE_THICKNESS, p.y - TRIANGLE_THICKNESS, p.z - TRIANGLE_THICKNESS);
			mBoundingBox.max = SVector3(p.x + TRIANGLE_THICKNESS, p.y + TRIANGLE_THICKNESS, p.z + TRIANGLE_THICKNESS);
		}
		SVector3 min = mBoundingBox.min;
		SVector3 max = mBoundingBox.max;
		mBoundingBox.min = SVector3(MIN(p.x, min.x-TRIANGLE_THICKNESS), MIN(p.y, min.y-TRIANGLE_THICKNESS), 
			MIN(p.z, min.z+TRIANGLE_THICKNESS));
		mBoundingBox.max = SVector3(MAX(p.x, max.x+TRIANGLE_THICKNESS), MAX(p.y, max.y+TRIANGLE_THICKNESS), 
			MAX(p.z, max.z+TRIANGLE_THICKNESS));
	}
	mTris.Add(o);	
}

void CPMesh::Remove( CPTriangle* o )
{
	CListEntry<CPTriangle>* e;

	e = mTris.FindEntry( o );

	if ( e )
	{
		mTris.RemoveEntry( e );
	}	
}

//CLinkedList<CPTriangle> mTris;	


/* ********	CPTriangle	************************************************************	*/


CPTriangle::CPTriangle( bool steep )
{
	mSteep = steep;
	mObjectType = 0;
	mCollisionMask = 0;
	mOwner = NULL;
}

CPTriangle::~CPTriangle()
{
}

void CPTriangle::SetPoint( SVector3 point, int index )
{
	if ( index >= 0 && index < 3 )
		mPoints[index] = point;
	if (index == 2)
	{
		CalculateNormal();
		CalculateBoundingBox();
		CalculateSphere();
	}
}

SVector3 CPTriangle::GetPoint( int index )
{
	SVector3 p;
	if ( index >= 0 && index < 3 )
		p = mPoints[index];
//	p = p + mPosition;		// Convert Local Points to Universal
	return p;
}

void CPTriangle::CalculateNormal()
{
	SVector3 a = mPoints[1] - mPoints[0];
	SVector3 b = mPoints[2] - mPoints[0];
	mNormal = a.CrossProduct( b );
	mNormal.Normalize();
	mPlaneConstant = -(mNormal.x * mPoints[0].x + mNormal.y * mPoints[0].y + mNormal.z * mPoints[0].z);
	float val = sin( (90.0f - STEEP_TRIANGLE_ANGLE) * RADS );
	if ( mNormal.y < val )
		mSteep = true;
	else
		mSteep = false;
}

void CPTriangle::CalculateBoundingBox()
{
	mBoundingBox.min.x = MIN( mPoints[0].x, MIN( mPoints[1].x, mPoints[2].x ) ) - TRIANGLE_THICKNESS;
	mBoundingBox.min.y = MIN( mPoints[0].y, MIN( mPoints[1].y, mPoints[2].y ) ) - TRIANGLE_THICKNESS;
	mBoundingBox.min.z = MIN( mPoints[0].z, MIN( mPoints[1].z, mPoints[2].z ) ) - TRIANGLE_THICKNESS;
	mBoundingBox.max.x = MAX( mPoints[0].x, MAX( mPoints[1].x, mPoints[2].x ) ) + TRIANGLE_THICKNESS;
	mBoundingBox.max.y = MAX( mPoints[0].y, MAX( mPoints[1].y, mPoints[2].y ) ) + TRIANGLE_THICKNESS;
	mBoundingBox.max.z = MAX( mPoints[0].z, MAX( mPoints[1].z, mPoints[2].z ) ) + TRIANGLE_THICKNESS;
}

void CPTriangle::CalculateSphere()
{
	SVector3 center;
	center = center + mPoints[0];
	center = center + mPoints[1];
	center = center + mPoints[2];
	center = center / 3.0f;
	float radius = 0.0f;
	radius = MAX( radius, (center - mPoints[0]).Length() );
	radius = MAX( radius, (center - mPoints[1]).Length() );
	radius = MAX( radius, (center - mPoints[2]).Length() );
/*
	mSphere.position = center;
	mSphere.radius = radius;
*/
/*	mEllipsoid.position = center;
	mEllipsoid.eSpace = SMatrix3( SVector3( radius, 0.0f, 0.0f ),
		SVector3( 0.0f, radius, 0.0f ), SVector3( 0.0f, 0.0f, radius ) );*/
}
/*
CPShape	CPTriangle::GetShape()
{
	return CP_TRIANGLE;
}
*/
SVector3 CPTriangle::GetNormal()
{
	return mNormal;
}

float CPTriangle::GetPlaneConstant()
{
	return mPlaneConstant;
}

bool CPTriangle::IsSteep()
{
	return mSteep;
}


SBoundingBox CPTriangle::GetBoundingBox()
{
	return mBoundingBox;
}

void	CPTriangle::SetObjectType( int type )
{
	mObjectType = type;
}

int		CPTriangle::GetObjectType()
{
	return mObjectType;
}

void	CPTriangle::SetCollisionMask( int mask )
{
	mCollisionMask = mask;
}

int		CPTriangle::GetCollisionMask()
{
	return mCollisionMask;
}

void	CPTriangle::SetOwner( IPhysicalObj* o )
{
	mOwner = o;
}

IPhysicalObj*	CPTriangle::GetOwner()
{
	return mOwner;
}

