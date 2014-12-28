/**
 * Physics.cpp
 * CHRONICLES PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23 -
 *	All Rights Reserved.
**/

#include <set>
#include "physics.h"
#include <stdio.h>

CPhysics::CPhysics()
{
	mBounds = false;
	SetCollisionFunction( NULL );
	mSortedObjects = NULL;
	mIsSorted = false;
	mBounds = false;

#ifdef PHYSICS_LOGGING	
	gLog.LogItem( new CLogMessage( "New Physics Simulation Created" ) );
#endif
}

CPhysics::~CPhysics( )
{
	if ( mObjects.GetLength() > 0 )
	{
#ifdef PHYSICS_LOGGING
		std::stringstream stream;
		stream << "Physics Warning: " << (mObjects.length + mStaticObjects.length) << " physics object(s) left in simulation during object destruction.";
		gLog.LogItem( new CLogMessage( stream.str() ) );
#endif

		if ( mIsSorted )
			delete[] mSortedObjects;

		CListEntry<CPObject>* object = mObjects.GetFirst();
		while ( object )
		{
			delete object->data;
			mObjects.RemoveEntry( object );
			object = mObjects.GetFirst();
		}

		CListEntry<CPTriangle>* staticObject = mStaticObjects.GetFirst();
		while ( staticObject )
		{
			delete staticObject->data;
			mStaticObjects.RemoveEntry( staticObject );
			staticObject = mStaticObjects.GetFirst();
		}
	}
	
#ifdef PHYSICS_LOGGING
	gLog.LogItem( new CLogMessage( "Physics Simulation Destroyed" ) );
#endif
}


#ifdef ALLOW_OBJECT_ACCESS
CLinkedList<CPObject>*	CPhysics::GetObjectList()
{
	return &mObjects;
}

CLinkedList<CPTriangle>*	CPhysics::GetStaticObjectList()
{
	return &mStaticObjects;
}
#endif

bool CPhysics::IsObjectInSimulation( CPObject* o )
{
	return mObjects.FindEntry( o ) != NULL;
}

bool CPhysics::IsObjectInSimulation( CPTriangle* o )
{
	return mStaticObjects.FindEntry( o ) != NULL;
}

void CPhysics::Update( float dt )
{
	SyncObjects( dt );
	SyncCollisions();

	CListEntry<CPObject>* e = mObjects.GetFirst();
	while ( e )
	{
		e->data->mForce = SVector3(0.0f, 0.0f, 0.0f);
		e = e->GetNext();
	}
}


void CPhysics::SortStaticObjects( int resolutionX, int resolutionY, int resolutionZ )
{
	if ( !mIsSorted && mBounds )
	{
		/*	Copy Information	*/
		mIsSorted = true;
		mSortX = resolutionX;
		mSortY = resolutionY;
		mSortZ = resolutionZ;

		/*	Create std::set Array	*/
		mSortedObjects = new std::set<CPTriangle*>*[ mSortX * mSortY * mSortZ ];
		for ( int i = 0; i < (mSortX*mSortY*mSortZ); i++ )
			mSortedObjects[i] =	NULL;	//new std::set<CPObject*>();

		/*	Fill with Sorted Objects	*/
		CListEntry<CPTriangle>* e = mStaticObjects.GetFirst();
		while ( e )
		{
			SBoundingBox aabb = e->data->GetBoundingBox();
			SVector3 begin = (aabb.min - mBoundries.min)/(mBoundries.max - mBoundries.min);
			SVector3 end = (aabb.max - mBoundries.min)/(mBoundries.max - mBoundries.min);

			int beginX = MAX( MIN( (int)(begin.x * mSortX), mSortX-1 ), 0 );
			int beginY = MAX( MIN( (int)(begin.y * mSortY), mSortY-1 ), 0 );
			int beginZ = MAX( MIN( (int)(begin.z * mSortZ), mSortZ-1 ), 0 );
			int endX = MAX( MIN( (int)(end.x * mSortX), mSortX-1 ), 0 );
			int endY = MAX( MIN( (int)(end.y * mSortY), mSortY-1 ), 0 );
			int endZ = MAX( MIN( (int)(end.z * mSortZ), mSortZ-1 ), 0 );

			for ( int z = beginZ; z <= endZ; z++ )
				for ( int y = beginY; y <= endY; y++ )
					for ( int x = beginX; x <= endX; x++ )
					{
						if ( mSortedObjects[ x + y*mSortX + z*mSortX*mSortY ] == NULL )
							mSortedObjects[ x + y*mSortX + z*mSortX*mSortY ] = new std::set<CPTriangle*>();
						mSortedObjects[ x + y*mSortX + z*mSortX*mSortY ]->insert( e->data );
					}
				
			e = e->GetNext();
		}

	}
}


void CPhysics::Add( CPObject* obj )
{
	mObjects.Add( obj );
}



void CPhysics::Add( CPTriangle* obj )
{
	mStaticObjects.Add( obj );

	if ( mIsSorted )
	{
		SBoundingBox aabb = obj->GetBoundingBox();
		SVector3 begin = (aabb.min - mBoundries.min)/(mBoundries.max - mBoundries.min);
		SVector3 end = (aabb.max - mBoundries.min)/(mBoundries.max - mBoundries.min);

		int beginX = MAX( MIN( (int)(begin.x * mSortX), mSortX-1 ), 0 );
		int beginY = MAX( MIN( (int)(begin.y * mSortY), mSortY-1 ), 0 );
		int beginZ = MAX( MIN( (int)(begin.z * mSortZ), mSortZ-1 ), 0 );
		int endX = MAX( MIN( (int)(end.x * mSortX), mSortX-1 ), 0 );
		int endY = MAX( MIN( (int)(end.y * mSortY), mSortY-1 ), 0 );
		int endZ = MAX( MIN( (int)(end.z * mSortZ), mSortZ-1 ), 0 );

		for ( int z = beginZ; z <= endZ; z++ )
			for ( int y = beginY; y <= endY; y++ )
				for ( int x = beginX; x <= endX; x++ )
				{
					if ( mSortedObjects[ x + y*mSortX + z*mSortX*mSortY ] == NULL )
						mSortedObjects[ x + y*mSortX + z*mSortX*mSortY ] = new std::set<CPTriangle*>();
					mSortedObjects[ x + y*mSortX + z*mSortX*mSortY ]->insert( obj );		
				}
	}
}




void CPhysics::Remove( CPObject* obj )
{
	CListEntry<CPObject>* e;

	e = mObjects.FindEntry( obj );

	if ( e )
	{
		mObjects.RemoveEntry( e );
	} else {
		// Object not found in Object List
#ifdef PHYSICS_LOGGING
		gLog.LogItem( new CLogMessage("Physics Warning: Could not find object to remove.") );
#endif
	}
}



void CPhysics::Remove( CPTriangle* obj )
{
	CListEntry<CPTriangle>* e;

	e = mStaticObjects.FindEntry( obj );

	if ( e )
	{
		mStaticObjects.RemoveEntry( e );

		if ( mIsSorted )
		{
			SBoundingBox aabb = obj->GetBoundingBox();
			SVector3 begin = (aabb.min - mBoundries.min)/(mBoundries.max - mBoundries.min);
			SVector3 end = (aabb.max - mBoundries.min)/(mBoundries.max - mBoundries.min);

			int beginX = MAX( MIN( (int)(begin.x * mSortX), mSortX-1 ), 0 );
			int beginY = MAX( MIN( (int)(begin.y * mSortY), mSortY-1 ), 0 );
			int beginZ = MAX( MIN( (int)(begin.z * mSortZ), mSortZ-1 ), 0 );
			int endX = MAX( MIN( (int)(end.x * mSortX), mSortX-1 ), 0 );
			int endY = MAX( MIN( (int)(end.y * mSortY), mSortY-1 ), 0 );
			int endZ = MAX( MIN( (int)(end.z * mSortZ), mSortZ-1 ), 0 );

			for ( int z = beginZ; z <= endZ; z++ )
				for ( int y = beginY; y <= endY; y++ )
					for ( int x = beginX; x <= endX; x++ )
					{
						if ( mSortedObjects[ x + y*mSortX + z*mSortX*mSortY ] != NULL )
							mSortedObjects[ x + y*mSortX + z*mSortX*mSortY ]->erase( obj );
					}
				
		}
	} else {
		// Object not found in Object List
#ifdef PHYSICS_LOGGING
		gLog.LogItem( new CLogMessage("Physics Warning: Could not find object to remove.") );
#endif
	}
}


void CPhysics::SetCollisionFunction( void (*collisionFunction)(CCollision c) )
{
	mCollisionFunction = collisionFunction;
}


void CPhysics::SetBoundries( SBoundingBox boundries )
{
	mBounds = true;
	mBoundries = boundries;
}

SBoundingBox CPhysics::GetBoundries()
{
	return mBoundries;
}

void CPhysics::DisableBoundries()
{
	if ( !mIsSorted )
		mBounds = false;
}

bool CPhysics::AreBoundriesEnabled()
{
	return mBounds;
}

void CPhysics::CheckBounds( CPObject* obj )
{
	if ( mBounds )
	{
		SVector3 translate;
		SBoundingBox b = obj->GetBoundingBox();

		/*	lolhax - if / else coding	*/
		if ( b.min.y < mBoundries.min.y )
		{
			translate.y = mBoundries.min.y - b.min.y;
		}
		else if ( b.max.y > mBoundries.max.y )
		{
			translate.y = mBoundries.max.y - b.max.y;
		}

		if ( b.min.x < mBoundries.min.x )
		{
			translate.x = mBoundries.min.x - b.min.x;
		}
		else if ( b.max.x > mBoundries.max.x )
		{
			translate.x = mBoundries.max.x - b.max.x;
		}

		if ( b.min.z < mBoundries.min.z )
		{
			translate.z = mBoundries.min.z - b.min.z;
		}
		else if ( b.max.z > mBoundries.max.z )
		{
			translate.z = mBoundries.max.z - b.max.z;
		}

		obj->Translate( translate );
	}
}



void CPhysics::SyncObjects( float dt )
{
	/* Moves Objects & Keeps Them in Bounds	*/

	CListEntry<CPObject>* e = mObjects.GetFirst();
	while ( e )
	{
		e->data->Update( dt );
		CheckBounds( e->data );	
		e = e->GetNext();
	}
}



void CPhysics::SyncCollisions()
{
	/* Dynamic - Dynamic Collisions	*/
	CListEntry<CPObject>*	object1;
	CListEntry<CPObject>*	object2;
	CListEntry<CPTriangle>*	staticObject;

	object1 = mObjects.GetFirst();
	while ( object1 )
	{	
		if (object1->data->mShape == CP_SIMPLE)
		{

			/*	Static Objects	*/
			if ( !mIsSorted )
			{
				staticObject = mStaticObjects.GetFirst();
				while ( staticObject )
				{
					Collision( object1->data, staticObject->data );
					staticObject = staticObject->GetNext();
				}
			}	/*	Sorted Static Objects	*/
			else
			{
				/*	Construct Set	*/
				std::set<CPTriangle*> collisions;

				SBoundingBox aabb = object1->data->GetBoundingPath();

				SVector3 begin = (aabb.min - mBoundries.min)/(mBoundries.max - mBoundries.min);
				SVector3 end = (aabb.max - mBoundries.min)/(mBoundries.max - mBoundries.min);

				int beginX = MAX( MIN( (int)(begin.x * mSortX), mSortX-1 ), 0 );
				int beginY = MAX( MIN( (int)(begin.y * mSortY), mSortY-1 ), 0 );
				int beginZ = MAX( MIN( (int)(begin.z * mSortZ), mSortZ-1 ), 0 );
				int endX = MAX( MIN( (int)(end.x * mSortX), mSortX-1 ), 0 );
				int endY = MAX( MIN( (int)(end.y * mSortY), mSortY-1 ), 0 );
				int endZ = MAX( MIN( (int)(end.z * mSortZ), mSortZ-1 ), 0 );

				for ( int z = beginZ; z <= endZ; z++ )
					for ( int y = beginY; y <= endY; y++ )
						for ( int x = beginX; x <= endX; x++ )
						{
							if ( mSortedObjects[ x + y*mSortX + z*mSortX*mSortY ] != NULL )
								collisions.insert( mSortedObjects[ x + y*mSortX + z*mSortX*mSortY ]->begin(),
									mSortedObjects[ x + y*mSortX + z*mSortX*mSortY ]->end() );
						}


				/*	Iterate through Set	*/
				std::set<CPTriangle*>::iterator it;
				for ( it = collisions.begin(); it != collisions.end(); it++ )
					Collision( object1->data, (*it) );
			}

			/*	Dynamic Objects		*/
			object2 = object1->GetNext();
			while ( object2 )
			{
				Collision( object1->data, object2->data );
				object2 = object2->GetNext();
			}
		} else {
			/*	Dynamic Objects		*/
			object2 = object1->GetNext();
			while ( object2 )
			{
				Collision( object2->data, object1->data );
				object2 = object2->GetNext();
			}
		}

		object1 = object1->GetNext();
	}
}
