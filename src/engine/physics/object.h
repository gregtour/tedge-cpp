/**
 * Object.h
 * CHRONICLES PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23 -
 *	All Rights Reserved.
**/


#ifndef _OBJECT_H
#define _OBJECT_H

#include "../common/list.h"

//#define STEEP_TRIANGLE_ANGLE	(65.0f)
#define STEEP_TRIANGLE_ANGLE	(25.0f)
#define TRIANGLE_THICKNESS		(0.15f)

enum CPShape
{
	CP_SIMPLE,
	CP_MESH
};

class IPhysicalObj;

class CPObject
{
public:
	/*	Constructors & Destructors	*/
	CPObject();
	CPObject( bool dynamic );
	virtual ~CPObject();
	
	void Update( float dt );

	void	SetPosition( SVector3 p );
	SVector3 GetPosition();
	SVector3 GetOldPosition();
	void	SetVelocity( SVector3 v );
	SVector3 GetVelocity();
	

	void	SetSphere( SSphere s );
	SSphere GetSphere();							// Returns Universal Coordinates
	SSphere GetOldSphere();							// Returns Universal Coordinates


	void	SetBoundingBox( SBoundingBox b );	
	SBoundingBox GetBoundingBox();					// Returns Universal Coordinates
	SBoundingBox GetBoundingPath();					// Universal Coordinates, path through space
//	void	SetMass( float m );
//	float	GetMass();
//	void	SetBounce( bool b );
//	bool	GetBounce();
//	void	SetBounceEfficiency( float b );
//	float	GetBounceEfficiency();
	void	SetOwner( IPhysicalObj* o );
	IPhysicalObj*	GetOwner();
	void	SetObjectType( int type );
	int		GetObjectType();
	void	SetCollisionMask( int mask );
	int		GetCollisionMask();
	
	void	SetFriction( float f );	
	void	SetDefaultForce( SVector3 force );

	void	SetSurfaceVelocity(SVector3 v);

	void	ApplyForce( SVector3 force );
	void	Translate( SVector3 translation );

	CPShape	mShape;

	SVector3 mForce;

protected:

	/*	Properties	*/
	SVector3 mOldPosition;
	SVector3 mPosition;
	SVector3 mVelocity;

	SVector3 mSurfaceVelocity;

	SBoundingBox mBoundingBox;	/*	Local Coordinates	*/
	SBoundingBox mPath;			// Global coordinates, bounding box of this and last frame's boxes
	SSphere	mSphere;	

	/*	Options	*/
	int		mObjectType;
	int		mCollisionMask;
	IPhysicalObj*	mOwner;
	
	/*	Behaviors	*/
	bool	mDynamic;
	SVector3 mDefaultForce;
	float	mFriction;

//	bool	mBounce;
//	float	mBounceEfficiency;
//	float	mMass;
};


class CPTriangle
{
public:
	CPTriangle( bool steep );
	~CPTriangle();

	void SetPoint( SVector3 point, int index );
	SVector3 GetPoint( int index );

	SBoundingBox GetBoundingBox();	

	void	SetObjectType( int type );
	int		GetObjectType();
	void	SetCollisionMask( int mask );
	int		GetCollisionMask();

	SVector3 GetNormal();
	float GetPlaneConstant();
	bool IsSteep();

	CPShape	GetShape();

	void	SetOwner( IPhysicalObj* o );
	IPhysicalObj*	GetOwner();

protected:	// 22 bytes vs 42


	void CalculateNormal();
	void CalculateBoundingBox();
	void CalculateSphere();

	SVector3 mPoints[3];
	SVector3 mNormal;
	float mPlaneConstant;
	bool mSteep;

	SBoundingBox mBoundingBox;
	int		mObjectType;
	int		mCollisionMask;
	IPhysicalObj*	mOwner;
};


class CPMesh: public CPObject
{
public:
	CPMesh();
	virtual ~CPMesh();

	void Add( CPTriangle* o );
	void Remove( CPTriangle* o );
	void ComputeSphere();

	CLinkedList<CPTriangle> mTris;	
	float mRot;
};

struct CPSpring 
{
    CPObject* a;
    CPObject* b;
    float     length;
    float     k;
};

#endif

