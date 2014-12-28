#include "spike.h"
#include <math.h>

CSpike::CSpike( float frequency )
{
	mUp = false;
	mY = 0.0f;
	mTime.Clear();
	mTime.Step();
	mFrequency = frequency;
	mSound = CManagedWaveSound::Load( &gResourceManager, "data/spike.wav" );

//	mQuadric = gluNewQuadric();
//	gluQuadricNormals( mQuadric, GLU_SMOOTH );
//	gluQuadricTexture( mQuadric, GL_TRUE );

}

CSpike::~CSpike()
{
//	gluDeleteQuadric( mQuadric );
	CManagedWaveSound::Unload( &gResourceManager, mSound );
}

void CSpike::LoadTexture()
{
	mTexture = NULL;
}


void CSpike::CreatePhysical( CPhysics* world, SVector3* pp1, SVector3* pp2 )
{
	p1 = *pp1;
	p2 = *pp2;

	//SPoint pos;
	//SVector vec;
/*
	pos.Add( p1 );
	pos.Add( p2 );
	pos.x /= 2;
	pos.y /= 2;
	
	SPoint pos1( p1->x, p1->y );
	SPoint pos2( p2->x, p2->y );
	pos1.x -= pos.x;
	pos2.x -= pos.x;
	pos1.y -= pos.y;
	pos2.y -= pos.y;*/

	mTri1 = new CPTriangle(true);
	mTri2 = new CPTriangle(true);
	

	SVector3 p3 = p1;
	SVector3 p4 = p2;
	p3.y += 4.0f;
	p4.y += 4.0f;

	mTri1->SetPoint(p1, 0);
	mTri1->SetPoint(p2, 1);
	mTri1->SetPoint(p3, 2);
	mTri2->SetPoint(p1, 0);
	mTri2->SetPoint(p4, 1);
	mTri2->SetPoint(p3, 2);
	mTri1->SetObjectType(SPIKE_OBJ_TYPE);
	mTri2->SetObjectType(SPIKE_OBJ_TYPE);
	mTri1->SetCollisionMask(0);
	mTri2->SetCollisionMask(0);
	mTri1->SetOwner(this);
	mTri2->SetOwner(this);

	mWorld = world;
	if ( mWorld )
	{
		//mWorld->Add( mObject );
		mWorld->Add(mTri1);
		mWorld->Add(mTri2);
	}

/*
	mObject = new CPLine( &pos1, &pos2, &pos, &vec, false, SPIKE_OBJ_TYPE );
	mObject->SetOwner( (IPhysicalObj*)this );

	mWorld = world;
	if ( mWorld )
		mWorld->Add( mObject );*/
}

void CSpike::Update( float dt )
{
	if ( mTime.Time() > mFrequency )
	{
		mTime.Step();
		mUp = !mUp;

		if ( mUp )
		{
			mY = -3.0f;

			float x, y, z;
			gCamera->GetPosition( &x, &y, &z );
			if (  ((x-p1.x)*(x-p1.x) + (z-p1.z)*(z-p1.z)) <= 2000.0f )
				mSound->Play( 0 );
		}
	}

}

void CSpike::Render()
{
	if ( mUp )
	{
		glDisable( GL_LIGHTING );
		glColor3f( 1.0f, 1.0f, 1.0f );

		/*CPLine* line = ((CPLine*)mObject);
		SPoint p1 = line->GetPoint1();
		p1.Add( &line->GetPosition() );
		SPoint p2 = line->GetPoint2();
		p2.Add( &line->GetPosition() );*/

		SVector3 v;
		v = p2 - p1;
		v = v * 0.25f;

		float height = fabs( (mTime.Time() - mFrequency / 2.0f) / (mFrequency / 2.0f) );
		//height = 1.0f - height;

		glPushMatrix();
		for ( int i = 0; i < 4; i++ )
		{
			glPopMatrix();
			glPushMatrix();
			glTranslatef( p1.x, p1.y + height*-4.0f - 1.0f, p1.z );
			glTranslatef( v.x / 2.0f + v.x*i, v.y / 2.0f + v.y*i, v.z / 2.0f + v.z*i );

			//glRotatef( 90.0f, -1.0f, 0.0f, 0.0f );
			//gluCylinder( mQuadric, 1.0f * height, 0.0f, 3.0f * height, 10, 4 );

			glBegin(GL_TRIANGLES);
				glVertex3f(-1.0f, 0.0f, -1.0f);
				glVertex3f(-1.0f, 0.0f,  1.0f);
				glVertex3f( 0.0f, 5.0f,  0.0f);

				glVertex3f(-1.0f, 0.0f,  1.0f);
				glVertex3f( 1.0f, 0.0f,  1.0f);
				glVertex3f( 0.0f, 5.0f,  0.0f);

				glVertex3f( 1.0f, 0.0f,  1.0f);
				glVertex3f( 1.0f, 0.0f, -1.0f);
				glVertex3f( 0.0f, 5.0f,  0.0f);

				glVertex3f( 1.0f, 0.0f, -1.0f);
				glVertex3f(-1.0f, 0.0f, -1.0f);
				glVertex3f( 0.0f, 5.0f,  0.0f);
			glEnd();
		}
		glPopMatrix();

		glEnable( GL_LIGHTING );
	}
}

bool CSpike::IsUp()
{
	float height = fabs( (mTime.Time() - mFrequency / 2.0f) / (mFrequency / 2.0f) );
	height = 1.0f - height;
	return mUp && height > 0.2f;
}


SVector3 CSpike::GetPosition()
{
	return (p1 + p2) * 0.5f;
}
