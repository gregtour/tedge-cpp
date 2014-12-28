/**
 * Physics.h
 * CHRONICLES PHYSICS ENGINE - QUAK PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23, 2007 - May 29th, 2007 - 
 *	All Rights Reserved.
**/

#ifndef _PHYSICS_H
#define _PHYSICS_H

#define ALLOW_OBJECT_ACCESS
// #define PHYSICS_LOGGING

#include <set>

#include "common.h"
#include "object.h"
#include "../common/list.h"

#ifdef PHYSICS_LOGGING
	#include "../common/log.h"
	extern CLog gLog;
	#include <string>
	#include <sstream>
#endif

#define NUM_THREADS     2

class I3DVERTBUF;
class IPhysicalObj;
struct CCollision;
class CPhysics
{
public:
	CPhysics();
	~CPhysics();

	bool IsObjectInSimulation( CPObject* o );
	bool IsObjectInSimulation( CPTriangle* o );

	void Update( float dt );

	void Add( CPObject* o );
	void Remove( CPObject* o );

	void Add( CPTriangle* o );
	void Remove( CPTriangle* o );

	void Add( I3DVERTBUF* verts, int type, int mask);

    void Add( CPSpring* spring );
    void Remove( CPSpring* spring );

#ifdef ALLOW_OBJECT_ACCESS
	CLinkedList<CPObject>*	GetObjectList();
	CLinkedList<CPTriangle>*	GetStaticObjectList();
#endif


	void SortStaticObjects( int resolutionX, int resolutionY, int resolutionZ );	// Precondition: Must be a bounded world

	void DisableBoundries();
	bool AreBoundriesEnabled();
	void SetBoundries( SBoundingBox );
	SBoundingBox GetBoundries();

	void SetCollisionFunction( void (*collisionFunction)(CCollision c) );

	void CheckBounds( CPObject* obj );

//protected:
	/*	Object Lists		*/
	CLinkedList<CPObject>		mObjects;
	CLinkedList<CPTriangle>		mStaticObjects;
	std::set<CPTriangle*>**		mSortedObjects;
    std::set<CPObject*>**     mSortedDynamicObjects;
    CLinkedList<CPSpring>       mSprings;

	/*	Sort Optimization	*/
	bool mIsSorted;
	int mSortX, mSortY, mSortZ;

	/*	Boundries			*/
	bool mBounds;
	SBoundingBox mBoundries;

	/*		Private Methods		*/

    void ClearDynamicSortedObjects();

	void SyncObjects( float );
	void SyncCollisions();
	void SyncObjectsThreaded(CPObject**, int, float);
	void SyncCollisionsThreaded(CPObject**, int);

	/*	Collision Function Between 2 Objects	*/
	inline void Collision( CPObject* object1, CPObject* object2 )
	{
		if ( !BoundingBoxesIntersect( object1->GetBoundingBox(), object2->GetBoundingBox() ) )
        //if ( !BoundingBoxesIntersect(object1->GetBoundingPath(), object2->GetBoundingPath()) )
			{ return; }

		if (object2->mShape == CP_SIMPLE && object1->mShape == CP_SIMPLE)
		{	
			BoundingBoxCollision( object1, object2 );
		} else if (object2->mShape == CP_MESH && object1->mShape == CP_SIMPLE) {
			SphereMeshCollision( object1, (CPMesh*)object2 );
		}
		else if (object1->mShape == CP_MESH && object2->mShape == CP_SIMPLE) {
			SphereMeshCollision( object2, (CPMesh*)object1 );
		}
	}

	inline void Collision( CPObject* object1, CPTriangle* object2 )
	{
		if ( !BoundingBoxesIntersect( object1->GetBoundingPath(), object2->GetBoundingBox() ) )
		{ return; }

		SphereTriangleCollision( object1, (CPTriangle*)object2 );
	}

	void BoundingBoxCollision( CPObject* object1, CPObject* object2 );
	void SphereTriangleCollision( CPObject* sphere, CPTriangle* triangle );
	void SphereMeshCollision(CPObject* sphere, CPMesh* mesh);

	void (*mCollisionFunction)(CCollision c);
};
#endif

