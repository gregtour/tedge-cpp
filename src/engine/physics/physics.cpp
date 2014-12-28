/**
 * Physics.cpp
 * CHRONICLES PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23 -
 *	All Rights Reserved.
**/

#include <set>
#include <stdio.h>
#include <assert.h>

#include "physics.h"
#include "../graphics/buffer/I3dVertBuf.h"
#include "../common/profile.h"

#ifdef THREADED_PHYSICS
// threading
#include <sys/types.h>
#include <time.h>
#include <pthread.h>

void* col_thread_routine(void* arguments);
void* obj_thread_routine(void* arguments);

typedef struct {
    int start;
    int limit;
    int span;
    //particle_t* particles;
    CPhysics*  world;
    CPObject** objects;
    float dt;
    void* return_value;
} THREAD_PARAMS;

typedef struct {
    int count;
} THREAD_RESULTS;

#endif

//#ifdef

CPhysics::CPhysics()
{
	mBounds = false;
	SetCollisionFunction( NULL );
	mSortedObjects = NULL;
	mIsSorted = false;
	mBounds = false;
    mSortedDynamicObjects = NULL;
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
    ProfileStartClock(P_PHYSICS);

    ClearDynamicSortedObjects();

    CPObject** objects;
    CListEntry<CPObject>* e;
    int i;

    objects = new CPObject*[mObjects.length];
    e = mObjects.GetFirst();
    for (i = 0; e && i < mObjects.length; i++) 
    {
        objects[i] = e->data;
        e = e->GetNext();
    }

    ProfileStopClock(P_PHYSICS);

#ifdef THREADED_PHYSICS
    ProfileStartClock(P_KINEMATICS);
	SyncObjectsThreaded(objects, mObjects.length, dt);
    ProfileStopClock(P_KINEMATICS);

    ProfileStartClock(P_COLLISION);
    //SyncCollisions();
	SyncCollisionsThreaded(objects, mObjects.length);
    ProfileStopClock(P_COLLISION);
#else
    ProfileStartClock(P_KINEMATICS);
    SyncObjects(dt);
    ProfileStopClock(P_KINEMATICS);

    ProfileStartClock(P_COLLISION);
    SyncCollisions();
    ProfileStopClock(P_COLLISION);
#endif

    ProfileStartClock(P_PHYSICS);

    // spring dampening
    {
        CListEntry<CPSpring>* e = mSprings.GetFirst();
        while (e)
        {
            CPObject *a, *b;
            float length, k;
            SVector3 a_pos;
            SVector3 b_pos;
            float distance;
            SVector3 delta;

            a = e->data->a;
            b = e->data->b;
            length = e->data->length;
            k = e->data->k;

            a_pos = a->GetPosition();
            b_pos = b->GetPosition();

            delta = a_pos - b_pos;
            distance = delta.Length();
            
            a_pos = a_pos + delta * (length - distance) / distance / 2.0f;
            b_pos = b_pos - delta * (length - distance) / distance / 2.0f;

            a->SetPosition(a_pos);
            b->SetPosition(b_pos);
            
            //a->ApplyForce( delta * k * dt * 10.0f * (length - distance) / distance / 2.0f );
            //b->ApplyForce( delta * k * dt * 10.0f * (length - distance) / distance / 2.0f );

            e = e->GetNext();
        }
    }

    // reset forces
	e = mObjects.GetFirst();
	while ( e )
	{
		e->data->mForce = SVector3(0.0f, 0.0f, 0.0f);
		e = e->GetNext();
	}

    delete[] objects;

    ProfileStopClock(P_PHYSICS);
}

void CPhysics::ClearDynamicSortedObjects() {
    if (mSortedDynamicObjects) {
        for (int i = 0; i < mSortX * mSortZ; i++)
            if (mSortedDynamicObjects[i]) {
                delete mSortedDynamicObjects[i];
                mSortedDynamicObjects[i] = NULL;
            }
        //delete mSortedDynamicObjects;
        //mSortedDynamicObjects = NULL;
    }
    else
    {
        mSortedDynamicObjects = new std::set<CPObject*>*[mSortX * mSortZ];
        for (int i = 0; i < mSortX * mSortZ; i++)
            mSortedObjects[i] = NULL;
    }

	/*	Fill with Sorted Objects	*/
	CListEntry<CPObject>* e = mObjects.GetFirst();
	while ( e )
	{
		SBoundingBox aabb = e->data->GetBoundingBox();
		SVector3 begin = (aabb.min - mBoundries.min)/(mBoundries.max - mBoundries.min);
		SVector3 end = (aabb.max - mBoundries.min)/(mBoundries.max - mBoundries.min);

		int beginX = MAX( MIN( (int)(begin.x * mSortX), mSortX-1 ), 0 );
		int beginZ = MAX( MIN( (int)(begin.z * mSortZ), mSortZ-1 ), 0 );
		int endX = MAX( MIN( (int)(end.x * mSortX), mSortX-1 ), 0 );
		int endZ = MAX( MIN( (int)(end.z * mSortZ), mSortZ-1 ), 0 );

		for ( int z = beginZ; z <= endZ; z++ )
				for ( int x = beginX; x <= endX; x++ )
				{
					if ( mSortedDynamicObjects[ x + z*mSortX ] == NULL )
						mSortedDynamicObjects[ x + z*mSortX ] = new std::set<CPObject*>();
					mSortedDynamicObjects[ x + z*mSortX ]->insert( e->data );
				}
			
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

void CPhysics::Add( I3DVERTBUF* verts, int type, int mask)
{
	assert(verts != NULL);

    SVector3 temp;

	verts->Lock();
	for (int i = 0; i < verts->mNum; i+=3)
	{
		CPTriangle* mTri = new CPTriangle(true);
		for (int j = 0; j < 3; j++)
		{
            verts->GetPos(temp.x, temp.y, temp.z);
			mTri->SetPoint(temp, j);
			verts->NextVert();
		}
		mTri->SetCollisionMask(mask);
		mTri->SetObjectType(type);
		Add(mTri);
	}
	verts->Unlock();
}

void CPhysics::Add( CPSpring* spring )
{
    mSprings.Add(spring);
}

void CPhysics::Remove( CPSpring* spring )
{
    CListEntry<CPSpring>* e;
    e = mSprings.FindEntry(spring);
    if (e) {
        mSprings.RemoveEntry(e);
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


#ifdef THREADED_PHYSICS
pthread_barrier_t synchronization_point;

void* obj_thread_routine(void* arguments)
{
    CPhysics*  world;
    CPObject** objects;
    THREAD_PARAMS parameters;
    THREAD_RESULTS* return_value;
    int num, count;
    float dt;

    parameters = *((THREAD_PARAMS*)arguments);
    return_value = (THREAD_RESULTS*)parameters.return_value;

    world = parameters.world;
    objects = parameters.objects;
    dt = parameters.dt;

    count = 0;
    for (num = parameters.start; num < parameters.limit; num += parameters.span)
    {
        objects[num]->Update(dt);
        world->CheckBounds(objects[num]);
        count++;
    }

        // wait for other threads to finish
        pthread_barrier_wait(&synchronization_point);

    col_thread_routine(arguments);

    return_value->count = count;
    pthread_exit(return_value);
    return return_value;
}
#define STRIPE
void CPhysics::SyncObjectsThreaded(CPObject** objects, int count, float dt)
{
    pthread_t thread_ids[NUM_THREADS];
    pthread_attr_t thread_attr[NUM_THREADS];
    THREAD_PARAMS parameters[NUM_THREADS];
    THREAD_RESULTS results[NUM_THREADS];
    void* values[NUM_THREADS];

    //printf("Running across %i threads...\n", NUM_THREADS);
    pthread_barrier_init(&synchronization_point, 0l, NUM_THREADS);

    {
        int thread;
        /* create threads */
        for (thread = 0; thread < NUM_THREADS; thread++)
        {
            pthread_attr_init(&thread_attr[thread]);
            pthread_attr_setdetachstate(&thread_attr[thread], PTHREAD_CREATE_JOINABLE);
            parameters[thread].world = this;
            parameters[thread].dt = dt;
#ifdef STRIPE
            parameters[thread].start = thread + 1;
            parameters[thread].limit = count /*NUM_PARTICLES*/;
            parameters[thread].span = NUM_THREADS;
#else
            parameters[thread].start = thread * (count/NUM_THREADS);
            parameters[thread].limit = (thread+1) * (count/NUM_THREADS);
            parameters[thread].span = 1;
#endif
            parameters[thread].objects = objects;
            parameters[thread].return_value = &results[thread];
            results[thread].count = 0;

            pthread_create(&thread_ids[thread], &thread_attr[thread], obj_thread_routine, &parameters[thread]);
        }

        /* wait for work to finish */
        count = 0;
        for (thread = 0; thread < NUM_THREADS; thread++)
        {
            int err = pthread_join(thread_ids[thread], &values[thread]);
            if (err) {
                printf("Error in thread %i.\n", thread);
                return;
            }
            count += results[thread].count;
        }
    }

    pthread_barrier_destroy(&synchronization_point);
}
#endif

void CPhysics::SyncObjects(float dt)
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

#ifdef THREADED_PHYSICS
//pthread_barrier_t synchronization_point;

void* col_thread_routine(void* arguments)
{
    CPhysics*  world;
    CPObject** objects;
    THREAD_PARAMS parameters;
    THREAD_RESULTS* return_value;
    int num, count;
    float dt;

    parameters = *((THREAD_PARAMS*)arguments);
    return_value = (THREAD_RESULTS*)parameters.return_value;

    world = parameters.world;
    objects = parameters.objects;
    dt = parameters.dt;

    count = 0;
    for (num = parameters.start; num < parameters.limit; num += parameters.span)
    {
        CPObject* object1 = objects[num];

{
	/* Dynamic - Dynamic Collisions	*/
	//CListEntry<CPObject>*	object1;
	CListEntry<CPObject>*	object2;
	CListEntry<CPTriangle>*	staticObject;

	//object1 = mObjects.GetFirst();
	//while ( object1 )
	{	
		if (object1->mShape == CP_SIMPLE)
		{

			//	Static Objects	
//			if ( !world->mIsSorted )
			{
/*
				staticObject = world->mStaticObjects.GetFirst();
				while ( staticObject )
				{
					world->Collision( object1, staticObject->data );
					staticObject = staticObject->GetNext();
				}
*/
			}	//	Sorted Static Objects
//			else
			{
				//	Construct Set
				std::set<CPTriangle*> collisions;

				SBoundingBox aabb = object1->GetBoundingPath();

				SVector3 begin = (aabb.min - world->mBoundries.min)/(world->mBoundries.max - world->mBoundries.min);
				SVector3 end = (aabb.max - world->mBoundries.min)/(world->mBoundries.max - world->mBoundries.min);

				int beginX = MAX( MIN( (int)(begin.x * world->mSortX), world->mSortX-1 ), 0 );
				int beginY = MAX( MIN( (int)(begin.y * world->mSortY), world->mSortY-1 ), 0 );
				int beginZ = MAX( MIN( (int)(begin.z * world->mSortZ), world->mSortZ-1 ), 0 );
				int endX = MAX( MIN( (int)(end.x * world->mSortX), world->mSortX-1 ), 0 );
				int endY = MAX( MIN( (int)(end.y * world->mSortY), world->mSortY-1 ), 0 );
				int endZ = MAX( MIN( (int)(end.z * world->mSortZ), world->mSortZ-1 ), 0 );

				for ( int z = beginZ; z <= endZ; z++ )
					for ( int y = beginY; y <= endY; y++ )
						for ( int x = beginX; x <= endX; x++ )
						{
							if ( world->mSortedObjects[ x + y*world->mSortX + z*world->mSortX*world->mSortY ] != NULL )
								collisions.insert( world->mSortedObjects[ x + y*world->mSortX + z*world->mSortX*world->mSortY ]->begin(),
									world->mSortedObjects[ x + y*world->mSortX + z*world->mSortX*world->mSortY ]->end() );
						}


				//	Iterate through Set
				std::set<CPTriangle*>::iterator it;
				for ( it = collisions.begin(); it != collisions.end(); it++ )
					world->Collision( object1, (*it) );
			} //

			//	Dynamic Objects		O(N^2) naive approach
/*
			object2 = world->mObjects.GetFirst(); //object1->GetNext();
			while ( object2 )
			{
                if (object2->data != object1) {
    				world->Collision( object1, object2->data );
                }
  				object2 = object2->GetNext();
			} 
*/
            // dynamic objects, sorted
            //	Construct Set
			std::set<CPObject*> dcollisions;
			SBoundingBox aabb = object1->GetBoundingPath();

			SVector3 begin = (aabb.min - world->mBoundries.min)/(world->mBoundries.max - world->mBoundries.min);
			SVector3 end = (aabb.max - world->mBoundries.min)/(world->mBoundries.max - world->mBoundries.min);

			int beginX = MAX( MIN( (int)(begin.x * world->mSortX), world->mSortX-1 ), 0 );
			int beginZ = MAX( MIN( (int)(begin.z * world->mSortZ), world->mSortZ-1 ), 0 );

			int endX = MAX( MIN( (int)(end.x * world->mSortX), world->mSortX-1 ), 0 );
			int endZ = MAX( MIN( (int)(end.z * world->mSortZ), world->mSortZ-1 ), 0 );

			for ( int z = beginZ; z <= endZ; z++ )
					for ( int x = beginX; x <= endX; x++ )
					{
						if ( world->mSortedDynamicObjects[ x + z*world->mSortX ] != NULL )
							dcollisions.insert( world->mSortedDynamicObjects[ x + z*world->mSortX ]->begin(),
								world->mSortedDynamicObjects[ x + z*world->mSortX ]->end() );
					}


			//	Iterate through Set
			std::set<CPObject*>::iterator it;
			for ( it = dcollisions.begin(); it != dcollisions.end(); it++ ) {
                if (object1 != (*it))
					world->Collision( object1, (*it) );
            }

		} else {
/*
			//	Dynamic Objects
			object2 = world->mObjects.GetFirst(); //object1->GetNext();
			while ( object2 )
			{
                if (object2->data != object1) {
    				world->Collision( object2->data, object1 );
                }
   				object2 = object2->GetNext();
			}
*/
		}

		//object1 = object1->GetNext();
	}
}


//        objects[num]->Update(dt);
//        world->CheckBounds(objects[num]);
//        count++;


    }

        // wait for other threads to finish
        pthread_barrier_wait(&synchronization_point);

    return_value->count = count;
    pthread_exit(return_value);
    return return_value;
}
#define STRIPE
void CPhysics::SyncCollisionsThreaded(CPObject** objects, int count)
{
    return;

    pthread_t thread_ids[NUM_THREADS];
    pthread_attr_t thread_attr[NUM_THREADS];
    THREAD_PARAMS parameters[NUM_THREADS];
    THREAD_RESULTS results[NUM_THREADS];
    void* values[NUM_THREADS];

    //printf("Running across %i threads...\n", NUM_THREADS);
    pthread_barrier_init(&synchronization_point, 0l, NUM_THREADS);

    {
        int thread;
        /* create threads */
        for (thread = 0; thread < NUM_THREADS; thread++)
        {
            pthread_attr_init(&thread_attr[thread]);
            pthread_attr_setdetachstate(&thread_attr[thread], PTHREAD_CREATE_JOINABLE);
            parameters[thread].world = this;
            //parameters[thread].dt = dt;
#ifdef STRIPE
            parameters[thread].start = thread + 1;
            parameters[thread].limit = count /*NUM_PARTICLES*/;
            parameters[thread].span = NUM_THREADS;
#else
            parameters[thread].start = thread * (count/NUM_THREADS);
            parameters[thread].limit = (thread+1) * (count/NUM_THREADS);
            parameters[thread].span = 1;
#endif
            parameters[thread].objects = objects;
            parameters[thread].return_value = &results[thread];
            results[thread].count = 0;

            pthread_create(&thread_ids[thread], &thread_attr[thread], col_thread_routine, &parameters[thread]);
        }

        /* wait for work to finish */
        count = 0;
        for (thread = 0; thread < NUM_THREADS; thread++)
        {
            int err = pthread_join(thread_ids[thread], &values[thread]);
            if (err) {
                printf("Error in thread %i.\n", thread);
                return;
            }
            count += results[thread].count;
        }
    }

    pthread_barrier_destroy(&synchronization_point);
}
#endif


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
            /*
			object2 = object1->GetNext();
			while ( object2 )
			{
				Collision( object1->data, object2->data );
				object2 = object2->GetNext();
			}*/

            // dynamic objects, sorted
            //	Construct Set
			std::set<CPObject*> dcollisions;
			SBoundingBox aabb = object1->data->GetBoundingPath();

            CPhysics* world = this;

			SVector3 begin = (aabb.min - world->mBoundries.min)/(world->mBoundries.max - world->mBoundries.min);
			SVector3 end = (aabb.max - world->mBoundries.min)/(world->mBoundries.max - world->mBoundries.min);

			int beginX = MAX( MIN( (int)(begin.x * world->mSortX), world->mSortX-1 ), 0 );
			int beginZ = MAX( MIN( (int)(begin.z * world->mSortZ), world->mSortZ-1 ), 0 );
			int endX = MAX( MIN( (int)(end.x * world->mSortX), world->mSortX-1 ), 0 );
			int endZ = MAX( MIN( (int)(end.z * world->mSortZ), world->mSortZ-1 ), 0 );

			for ( int z = beginZ; z <= endZ; z++ )
					for ( int x = beginX; x <= endX; x++ )
					{
						if ( world->mSortedDynamicObjects[ x + z*world->mSortX ] != NULL )
							dcollisions.insert( world->mSortedDynamicObjects[ x + z*world->mSortX ]->begin(),
								world->mSortedDynamicObjects[ x + z*world->mSortX ]->end() );
					}


			//	Iterate through Set
			std::set<CPObject*>::iterator it;
			for ( it = dcollisions.begin(); it != dcollisions.end(); it++ ) {
                if (object1->data != (*it))
					world->Collision( object1->data, (*it) );
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

