#include "gate.h"
#include <math.h>
#include "../../engine/sound/wavesound.h"
#include "../main.h"

CGate::CGate()
{
	mOpen = 0;
	mOpening = false;
	mTexture = CManagedTexture::Load( &gResourceManager, "data/gate.bmp" );
//	mTexture = CManagedTexture::Load( &gResourceManager, "data/gate.png", true );
	mMask = CManagedTexture::Load( &gResourceManager, "data/gateMask.bmp" );
}

CGate::~CGate()
{
	CManagedTexture::Unload( &gResourceManager, mTexture );
	CManagedTexture::Unload( &gResourceManager, mMask );
}

void CGate::CreatePhysical( CPhysics* world, SVector3* pp1, SVector3* pp2 )
{
	p1 = *pp1;
	p2 = *pp2;

	mTri1 = new CPTriangle(true);
	mTri2 = new CPTriangle(true);
	mTri1->SetOwner(this);
	mTri2->SetOwner(this);

	SVector3 p3 = p1;
	SVector3 p4 = p2;
	p3.y += 8.0f;
	p4.y += 8.0f;

	mTri1->SetPoint(p1, 0);
	mTri1->SetPoint(p2, 1);
	mTri1->SetPoint(p3, 2);
	mTri2->SetPoint(p1, 0);
	mTri2->SetPoint(p3, 1);
	mTri2->SetPoint(p4, 2);
	mTri1->SetObjectType(GATE_OBJ_TYPE);
	mTri2->SetObjectType(GATE_OBJ_TYPE);
	mTri1->SetCollisionMask(1);
	mTri2->SetCollisionMask(1);

	//mObject = new CPLine( p1, p2, &pos, &vec, false, GATE_OBJ_TYPE );
	//mObject->SetOwner( (IPhysicalObj*)this );

	mWorld = world;
	if ( mWorld )
	{
		//mWorld->Add( mObject );
		mWorld->Add(mTri1);
		mWorld->Add(mTri2);
	}

}

void CGate::Open( )
{
	mOpen = 1;
	mOpening = true;
	CWaveSound* s = CManagedWaveSound::Load( &gResourceManager, "data/gate.wav" );
	s->Play( 3 );
}

bool CGate::IsOpen()
{
	return (mOpen != 0);
}

void CGate::Update( float dt )
{
	if ( mOpening )
	{
		p1.x += (p2.x - p1.x) * dt;
		p1.z += (p2.z - p1.z) * dt;

		//((CPLine*)mObject)->SetPoints( &p1, &p2 );
		SVector3 p3 = p1;
		SVector3 p4 = p2;
		p3.y += 8.0f;
		p4.y += 8.0f;

		mTri1->SetPoint(p1, 0);
		mTri1->SetPoint(p2, 1);
		mTri1->SetPoint(p3, 2);
		mTri2->SetPoint(p1, 0);
		mTri2->SetPoint(p3, 1);
		mTri2->SetPoint(p4, 2);


		if ( fabs(p1.x - p2.x) + fabs(p1.y-p2.y) < 0.2f )
		{
			mOpen = 2;
		}
	}
}

void CGate::RenderAlpha()
{
	if ( mOpen != 2 )
	{
		glDisable( GL_CULL_FACE );
		glEnable( GL_BLEND );
		//glEnable( GL_TEXTURE_2D );
		glDisable( GL_LIGHTING );
		glDisable( GL_FOG );

		//SPoint p1 = ((CPLine*)mObject)->GetPoint1();
		//SPoint p2 = ((CPLine*)mObject)->GetPoint2();

	//		mTexture->BindTexture();
		for ( int i = 1; i < 2; i++ )
		{

			if ( i == 0 )
			{
				glBlendFunc( GL_DST_COLOR, GL_ZERO );
				mMask->Bind();
				glDepthMask(GL_FALSE);
			}
			else if ( i == 1 )
			{
//				glBlendFunc( GL_ONE, GL_ONE );
				mTexture->Bind();
				glDepthMask(GL_TRUE);
			}

//			glBlendFunc( GL_ONE, GL_ZERO );
///			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			mTexture->Bind();

			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			glBegin( GL_QUADS );
					glTexCoord2f( 0.0f, 0.01f );
				glVertex3f( p1.x, p1.y + 8.0f, p1.z );
					glTexCoord2f( 0.0f, 1.0f );
				glVertex3f( p1.x, p1.y, p1.z );
					glTexCoord2f( 1.0f, 1.0f );
				glVertex3f( p2.x, p2.y, p2.z );
					glTexCoord2f( 1.0f, 0.01f );
				glVertex3f( p2.x, p2.y + 8.0f, p2.z );
			glEnd();
		}

		mTexture->Unbind();

		glEnable( GL_CULL_FACE );
		glDisable( GL_BLEND );
		glEnable( GL_LIGHTING );
		//glEnable( GL_FOG );
		gLevel->Fog();
	}
}
