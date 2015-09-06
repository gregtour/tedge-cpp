/**
 * Collision.cpp
 * CHRONICLES PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23 -
 *	All Rights Reserved.
**/

/*	Rough Check			*/
//	if ( !s1->box->intersect( s2->box ) )
//		return;

#include "collision.h"
#include "common.h"
#include "object.h"
#include "physics.h"
#include <stdio.h>
#include <math.h>



/*		****************************************************	*/


void CPhysics::BoundingBoxCollision( CPObject* object1, CPObject* object2 )
{
	SVector3 normal;
	SVector3 contact;
	if ( object1->GetCollisionMask() & object2->GetCollisionMask() )
	{
		/*	Fix Overlap	*/
		SBoundingBox box1 = object1->GetBoundingBox();
		SBoundingBox box2 = object2->GetBoundingBox();

		float overlapX = MIN( box1.max.x, box2.max.x ) - MAX( box1.min.x, box2.min.x );
		float overlapY = MIN( box1.max.y, box2.max.y ) - MAX( box1.min.y, box2.min.y );
		float overlapZ = MIN( box1.max.z, box2.max.z ) - MAX( box1.min.z, box2.min.z );
		
		if ( overlapX < overlapY && overlapX < overlapZ )
		{
			SVector3 translation( overlapX / 2.0f, 0.0f, 0.0f );
			if ( box1.min.x < box2.min.x )
				translation = translation * -1.0f;
			object1->Translate( translation );
			object2->Translate( translation * -1.0f );
			normal = SVector3(1.0f, 0.0f, 0.0f);
		} 
		else if ( overlapY < overlapX && overlapY < overlapZ )
		{
			//	***	Non Gravity World
			/*SVector3 translation( 0.0f, overlapY / 2.0f, 0.0f );
			if ( box1.min.y < box2.min.y )
				translation = translation * -1.0f;
			object1->Translate( translation );
			object2->Translate( translation * -1.0f );*/
			
			//	***	Gravity World
			SVector3 translation( 0.0f, overlapY, 0.0f );
			if ( box1.min.y > box2.min.y )
			{
				object1->Translate( translation );
				SVector3 vel = object1->GetVelocity();
				vel.y = 0.0f;
				object1->SetVelocity(vel);
				object1->SetSurfaceVelocity(object2->GetVelocity());
				contact = SVector3((box1.min.x + box1.max.x) * 0.5f, box1.min.y, (box1.min.z + box1.max.z) * 0.5f);
			}
			else
			{
				object2->Translate( translation );
				SVector3 vel = object2->GetVelocity();
				vel.y = 0.0f;
				object2->SetVelocity(vel);
				object2->SetSurfaceVelocity(object1->GetVelocity());
				contact = SVector3((box2.min.x + box2.max.x) * 0.5f, box2.min.y, (box2.min.z + box2.max.z) * 0.5f);
			}
			normal = SVector3(0.0f, 1.0f, 0.0f);

		}
		else
		{
			SVector3 translation( 0.0f, 0.0f, overlapZ / 2.0f );
			if ( box1.min.z < box2.min.z )
				translation = translation * -1.0f;
			object1->Translate( translation );
			object2->Translate( translation * -1.0f );
			normal = SVector3(0.0f, 0.0f, 1.0f);
		}
	}

	if ( mCollisionFunction != NULL )
	{
		mCollisionFunction( CCollision(object1->GetOwner(), object2->GetOwner(), object1->GetObjectType(), object2->GetObjectType(), contact, normal) );
	}

	return;
}

void CPhysics::SphereMeshCollision( CPObject* sphere1, CPMesh* mesh )
{
	CListEntry<CPTriangle>* tri = mesh->mTris.GetFirst();
	while (tri)
	{
		CPTriangle temp = *tri->data;
		for (int i = 0; i < 3; i++)
		{
			SVector3 p = temp.GetPoint(i);
			if (mesh->mRot == 0.0f)
				temp.SetPoint(p + mesh->GetPosition(), i);
			else
			{
				float theta = atan2(p.y, p.x);
				float d = sqrt(p.y*p.y + p.x*p.x);
				p.x = cos(theta + mesh->mRot) * d;
				p.y = sin(theta + mesh->mRot) * d;
				temp.SetPoint(p + mesh->GetPosition(), i);
			}
		}

		CPTriangle* triangle = &temp; // = tri->data;

		SSphere sphere = sphere1->GetSphere();
		SSphere osphere = sphere1->GetOldSphere();
		sphere.radius += TRIANGLE_THICKNESS;

		float distance = SignedDistanceToPlane( sphere.position, 
			triangle->GetNormal(), triangle->GetPlaneConstant() );

		float sign = SignedDistanceToPlane( osphere.position, 
			triangle->GetNormal(), triangle->GetPlaneConstant() );

		bool hasCrossed = (distance * sign < 0.0f);

		if ( !(fabs( distance ) > sphere.radius && !hasCrossed) )	// No Collision
		{
			bool collision = false;
			SVector3 contact;
			SVector3 normal;

			/*if ( triangle->IsSteep() )
			{*/

			if (hasCrossed)
			{
				contact = PlaneLineIntersection(sphere.position, osphere.position, triangle->GetNormal(), triangle->GetPlaneConstant());
			}
			else
			{
				if ( triangle->IsSteep() )
				{
					SVector3 translate = triangle->GetNormal() * -1.0f * distance;
					contact = sphere.position + translate;		
				} else {
					SVector3 below = sphere.position;
					below.y -= 1.0f;
					contact = PlaneLineIntersection(sphere.position, below, triangle->GetNormal(), triangle->GetPlaneConstant());
				}
				//contact = PlaneLineIntersection(sphere.position, osphere.position, triangle->GetNormal(), triangle->GetPlaneConstant());
			}

			if ( PointIsInTriangle( contact, triangle->GetPoint(0),
				triangle->GetPoint(1), triangle->GetPoint(2) ) )
			{
				collision = true;

				//	Intersecting Face of Triangle
				// Fix Intersection
				if ( sphere1->GetCollisionMask() & mesh->GetCollisionMask() )
				{
					SVector3 newPosition = contact;
					SVector3 correction;

					//	Keep on same side of triangle as it was before
					
					if ( triangle->IsSteep() )
					{
						normal = triangle->GetNormal();
						correction = normal * sphere.radius;
						if ( sign < 0.0f )
							correction = correction * -1.0f;
						SVector3 v = sphere1->GetVelocity();
						float amtIntoPlane = v.DotProduct(normal);
						v = v - normal * amtIntoPlane;
						sphere1->SetVelocity(v);

					}
					else	// Keeps things from sliding down hills
					{
						normal = SVector3( 0.0f, 1.0f, 0.0f );
						correction = normal * sphere.radius * normal.DotProduct(triangle->GetNormal()); 
						if ( sign < 0.0f )
							correction = correction * -1.0f;
						SVector3 v = sphere1->GetVelocity();
						v.y = mesh->GetVelocity().y;
						sphere1->SetVelocity(v);
					}

					newPosition = newPosition + correction;
					sphere1->Translate( newPosition - sphere.position );
				}
			}
			else
			{
				SSphere s = sphere;
				s.radius -= TRIANGLE_THICKNESS * 2;

				for ( int i = 0; i < 3; i++ )
				{
					int p1 = i;
					int p2 = (i+1)%3;

					contact = ClosestPointOnLineSegment( triangle->GetPoint(p1), triangle->GetPoint(p2), s.position );
					distance = (contact - s.position).LengthSquared();
					
					if ( distance < s.radius * s.radius )
					{
						collision = true;
						if ( sphere1->GetCollisionMask() & mesh->GetCollisionMask() )
						{	// Correct Intersection
							SVector3 difference = s.position - contact;
							
							normal = difference / sqrt( distance );	// Normalize
							difference = normal * s.radius;
							SVector3 newPosition = contact + difference;

							sphere1->Translate( newPosition - s.position );
						}
					}
				}
			}

			//if ((triangle->GetObjectType() != 0 || (sphere1->GetObjectType() == 1 && !triangle->IsSteep())) && mCollisionFunction != NULL)
			//if (triangle->GetObjectType() != 0 && mCollisionFunction != NULL)
			if (collision && mCollisionFunction != NULL)
				mCollisionFunction( CCollision(sphere1->GetOwner(), mesh->GetOwner(), sphere1->GetObjectType(), mesh->GetObjectType(), contact, normal) );
			if (collision)
			{
				sphere1->SetSurfaceVelocity(mesh->GetVelocity());
				sphere1->ApplyForce(mesh->mForce);
				return;
			}
		}

		tri = tri->GetNext();
	}
}



/*	Old, Sphere Triangle Collision*/
void CPhysics::SphereTriangleCollision( CPObject* sphere1, CPTriangle* triangle )
{
	SSphere sphere = sphere1->GetSphere();
	SSphere osphere = sphere1->GetOldSphere();
	sphere.radius += TRIANGLE_THICKNESS;

	float distance = SignedDistanceToPlane( sphere.position, 
		triangle->GetNormal(), triangle->GetPlaneConstant() );

	float sign = SignedDistanceToPlane( osphere.position, 
		triangle->GetNormal(), triangle->GetPlaneConstant() );

	bool hasCrossed = (distance * sign < 0.0f);

	if ( fabs( distance ) > sphere.radius && !hasCrossed )	// No Collision
		return;

	SVector3 contact;
	SVector3 normal;

	/*if ( triangle->IsSteep() )
	{*/

	if (hasCrossed)
	{
		contact = PlaneLineIntersection(sphere.position, osphere.position, triangle->GetNormal(), triangle->GetPlaneConstant());
	}
	else
	{
		if ( triangle->IsSteep() )
		{
			SVector3 translate = triangle->GetNormal() * -1.0f * distance;
			contact = sphere.position + translate;		
		} else {
			SVector3 below = sphere.position;
			below.y -= 1.0f;
			contact = PlaneLineIntersection(sphere.position, below, triangle->GetNormal(), triangle->GetPlaneConstant());
		}
		//contact = PlaneLineIntersection(sphere.position, osphere.position, triangle->GetNormal(), triangle->GetPlaneConstant());
	}

	if ( PointIsInTriangle( contact, triangle->GetPoint(0),
		triangle->GetPoint(1), triangle->GetPoint(2) ) )
	{
		//	Intersecting Face of Triangle
		// Fix Intersection
		if ( sphere1->GetCollisionMask() & triangle->GetCollisionMask() )
		{
			SVector3 newPosition = contact;
			SVector3 correction;

			//	Keep on same side of triangle as it was before
			
			if ( triangle->IsSteep() )
			{
				normal = triangle->GetNormal();
				correction = normal * sphere.radius;
				if ( sign < 0.0f )
					correction = correction * -1.0f;
				SVector3 v = sphere1->GetVelocity();
				float amtIntoPlane = v.DotProduct(normal);
				v = v - normal * amtIntoPlane;
				sphere1->SetVelocity(v);

			}
			else	// Keeps things from sliding down hills
			{
				normal = SVector3( 0.0f, 1.0f, 0.0f );
				correction = normal * sphere.radius * normal.DotProduct(triangle->GetNormal()); 
				if ( sign < 0.0f )
					correction = correction * -1.0f;
				SVector3 v = sphere1->GetVelocity();
				v.y = 0.0f;
				sphere1->SetVelocity(v);
			}

			newPosition = newPosition + correction;
			sphere1->Translate( newPosition - sphere.position );
		}
	}
	else
	{
		bool collision = false;

		SSphere s = sphere;
		s.radius -= TRIANGLE_THICKNESS * 2;

		for ( int i = 0; i < 3; i++ )
		{
			int p1 = i;
			int p2 = (i+1)%3;

			contact = ClosestPointOnLineSegment( triangle->GetPoint(p1), triangle->GetPoint(p2), s.position );
			distance = (contact - s.position).LengthSquared();
			
			if ( distance < s.radius * s.radius )
			{
				collision = true;
				if ( sphere1->GetCollisionMask() & triangle->GetCollisionMask() )
				{	// Correct Intersection
					SVector3 difference = s.position - contact;
					
					normal = difference / sqrt( distance );	// Normalize
					difference = normal * s.radius;
					SVector3 newPosition = contact + difference;

					sphere1->Translate( newPosition - s.position );
				}
			}
		}

		// If no contact, then no collision, return
		if ( !collision )
			return;
	}

	//if ((triangle->GetObjectType() != 0 || (sphere1->GetObjectType() == 1 && !triangle->IsSteep())) && mCollisionFunction != NULL)
	//if (triangle->GetObjectType() != 0 && mCollisionFunction != NULL)
	if (mCollisionFunction != NULL)
		mCollisionFunction( CCollision(sphere1->GetOwner(), triangle->GetOwner(), sphere1->GetObjectType(), triangle->GetObjectType(), contact, normal) );
	sphere1->SetSurfaceVelocity(SVector3(0.0f, 0.0f, 0.0f));
}


/*		****************************************************	*/
