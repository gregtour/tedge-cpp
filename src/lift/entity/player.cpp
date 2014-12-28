#include <math.h>
#include "player.h"
#include "../main.h"
#include "../../engine/engine.h"
#include "../game/gameInput.h"
#include "../../engine/graphics/graphics.h"
#include "../../engine/common/log.h"
#include "cameraController.h"

#include "../game/ship.h"


CPlayer::CPlayer()
{
	gPlayer = this;

	loaded = false;
}

CPlayer::~CPlayer()
{
	gPlayer = NULL;
}

void CPlayer::Load()
{
	mTestTexture = CManagedTexture::Load( &gResourceManager, "test2.bmp" );

	/* Construct Vertex Buffer Object	*/
	
	int numFaces = (sizeof(Ship_vidx)/sizeof(long)/3);
	mVertBuf = new I3DVERTBUF( true, I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_DIFFUSE | I3D_VC_UV1, numFaces * 3 );
	mIndxBuf = new I3DINDXBUF( true, numFaces * 3 );

	mIndxBuf->Lock();

	
	for ( int i=0; i < numFaces*3; i+=3 )
	{
		//--- This is how to use the provided texture coordinates
		// glTexCoord2f( sph01.uvs[sph01.v_idx[i]].x, sph01.uvs[sph01.v_idx[i]].y );


		CPTriangle tri = CPTriangle( true );
		for ( int z = 0; z < 3; z++ )
			tri.SetPoint( SVector3( Ship_vertex[ Ship_vidx[ i + z ] ].x, 
				Ship_vertex[ Ship_vidx[ i + z ] ].y, Ship_vertex[ Ship_vidx[ i + z ] ].z ), z );

		mVertBuf->Lock(3);
		
		// Draw Triangle
		SVector3 normal = tri.GetNormal();

		for (int z = 0; z < 3; z++)
		{
			SVector3 p = tri.GetPoint(z);
			mVertBuf->SetPos( p.x, p.y, p.z );
			mVertBuf->SetNormal( normal.x, normal.y, normal.z );
			mVertBuf->SetColor( 0.8f, 0.5f, 0.5f );
			mVertBuf->SetUV0(Ship_uv[Ship_vidx[i+z]].x, Ship_uv[Ship_vidx[i+z]].y);
			mVertBuf->NextVert();
		}
		
		mIndxBuf->AddTri( i + 0, i + 1, i + 2 );
	
//		object = object->GetNext();
		mVertBuf->Unlock();
	}
//	mVertBuf->Unlock();
	
	mIndxBuf->Unlock();//*/
}


void CPlayer::Update( float dt )
{
	if (!loaded)
	{
		Load();
		loaded = true;
	}

	SVector3 pos = mObject->GetPosition();

	if ( mInputState->IsKeyPressed(K_UP) )
	{
		SVector3 force;
		mUpRot = atan2(pos.y, pos.z) + PI / 2.0f;
		force.x = -cos( mRot ) * PLAYER_SPEED * dt;
		force.z = -sin( mRot ) * cos( mUpRot ) * PLAYER_SPEED * dt;
		force.y = -sin( mRot ) * sin( mUpRot ) * PLAYER_SPEED * dt;
		mObject->ApplyForce( force );
	}

	if ( mInputState->IsKeyPressed(K_DOWN) )
	{
		SVector3 force;
		mUpRot = atan2(pos.y, pos.z) + PI / 2.0f;
		force.x = cos( mRot ) * PLAYER_SPEED * 0.7f * dt;
		force.z = sin( mRot ) * cos( mUpRot ) * PLAYER_SPEED * dt;
		force.y = sin( mRot ) * sin( mUpRot ) * PLAYER_SPEED * dt;
		mObject->ApplyForce( force );
	}
	
	if ( mInputState->IsKeyPressed(K_RIGHT) )
	{
		mRot += PLAYER_ROT_SPEED * dt;
	}

	if ( mInputState->IsKeyPressed(K_LEFT) )
	{
		mRot -= PLAYER_ROT_SPEED * dt;
	}

	if ( mInputState->IsKeyPressed(K_SWING) )
	{
		SVector3 force;
		//force.y = 50.0f * dt;
		force = SVector3(0.0f, -pos.y/gTrackLength*5.0f, -pos.z/gTrackLength*5.0f);
		mObject->ApplyForce( force );
	}


	/*
	if (pos.z > 225.0f)
	{
		pos.z -= 380.0f;
		mObject->SetPosition(pos);
	}
	if (pos.z < -155.0f)
	{
		pos.z += 380.0f;
		mObject->SetPosition(pos);
	}*/

	//mObject->SetDefaultForce(SVector3(0.0f, pos.y/gTrackLength*75.0f, pos.z/gTrackLength*75.0f));
	mObject->SetDefaultForce(SVector3(0.0f, 0.0f, 0.0f));
/*
	mRot = mInputState->mousePosition.mMouseX / 200.0f;
	mUpRot = -mInputState->mousePosition.mMouseY / 200.0f + 1.0f;
*/
}

void CPlayer::Render()
{
//	gLevel->Lighting();

	mTestTexture->Bind();

	SVector3 pos = this->mObject->GetPosition();
	
	mUpRot = atan2(pos.y, pos.z) + PI / 2.0f;
	
	
	glTranslatef( pos.x, pos.y, pos.z );
	
	glRotatef( mUpRot * DEGREES, -1.0f, 0.0f, 0.0f );
	glRotatef( (mRot - HALF_PI) * DEGREES, 0.0f, -1.0f, 0.0f );


	I3dSelectBuffers( mVertBuf, mIndxBuf );
	//I3dSelectBuffers( mVertBuf, NULL );
	I3dRenderPrimIdx2( 0, mVertBuf->mNum/3, mVertBuf->mNum );
	//I3dRenderPrim( 0, mVertBuf->mNum/3, mVertBuf->mNum );
/*


	int numFaces = (sizeof(Ship_vidx)/sizeof(long)/3);
	for ( int i=0; i < numFaces*3; i+=3 )
	{
		//--- This is how to use the provided texture coordinates
		// glTexCoord2f( sph01.uvs[sph01.v_idx[i]].x, sph01.uvs[sph01.v_idx[i]].y );


		CPTriangle tri = CPTriangle( true );
		for ( int z = 0; z < 3; z++ )
			tri.SetPoint( SVector3( Ship_vertex[ Ship_vidx[ i + z ] ].x, 
				Ship_vertex[ Ship_vidx[ i + z ] ].y, Ship_vertex[ Ship_vidx[ i + z ] ].z ), z );

		//glPushMatrix();

		{	// Draw Triangle
			
			SVector3 normal = tri.GetNormal();
			SVector3 p1 = tri.GetPoint( 0 );
			SVector3 p2 = tri.GetPoint( 1 );
			SVector3 p3 = tri.GetPoint( 2 );
			
			glBegin( GL_TRIANGLES );	
				glNormal3f( normal.x, normal.y, normal.z );
				glColor3f(0.85f, 0.85f, 0.85f);
				glTexCoord2f( Ship_uv[Ship_vidx[i]].x, Ship_uv[Ship_vidx[i]].y);				
				glVertex3f( p1.x, p1.y, p1.z );
				glTexCoord2f( Ship_uv[Ship_vidx[i+1]].x, Ship_uv[Ship_vidx[i+1]].y);
				glVertex3f( p2.x, p2.y, p2.z );
				glTexCoord2f( Ship_uv[Ship_vidx[i+2]].x, Ship_uv[Ship_vidx[i+2]].y);
				glVertex3f( p3.x, p3.y, p3.z );
			glEnd();	
		}

		//object = object->GetNext();

		//glPopMatrix();
	}
*/
	mTestTexture->Unbind();
}


CMousePosition CPlayer::GetMouse()
{
	return mInputState->mousePosition;
}


