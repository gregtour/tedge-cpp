// GREG:
// this monstrosity must be destroyed
// they know not what they have done
// continue at your own risk:

#include <sstream>
#include "../main.h"

#include "level.h"
#include "player.h"
#include "item.h"
#include "cameraController.h"

#include "../../engine/update.h"
#include "../../engine/physics/physics.h"
#include "../../engine/physics/object.h"
#include "../../engine/graphics/graphics.h"
#include "../../engine/graphics/lighting.h"
#include "../../engine/entity/camera.h"
#include "../game/game.h"
#include <cmath>

//#include "../game/ktrack1.h"
//#include "../game/testTrack.h"
#include "../game/loopTrack3.h"



float gTrackLength;



CLevel::CLevel()
{
}

CLevel::CLevel( char* level )
{
}

/*
	~CLevel()
		Deletes any excess data, especially data created from Load().
*/
CLevel::~CLevel()
{
}

void CLevel::LoadLevel()
{
	int numFaces = (sizeof(Level_vidx)/sizeof(long));

	float zmin = Level_vertex[0].z;
	float zmax =  Level_vertex[0].z;
	for (int i = 0; i < sizeof(Level_vertex)/sizeof(_point3); i++)
	{
		if (Level_vertex[i].z < zmin)
			zmin = Level_vertex[i].z;
		else if (Level_vertex[i].z > zmax)
			zmax = Level_vertex[i].z;
	}
	gTrackLength = zmax - zmin;

	for ( int i=0; i < numFaces; i+=3 )
	{
		//--- This is how to use the provided texture coordinates
		// glTexCoord2f( sph01.uvs[sph01.v_idx[i]].x, sph01.uvs[sph01.v_idx[i]].y );

/*		glVertex3f(	sph01.vertices[sph01.v_idx[i]].x,
					sph01.vertices[sph01.v_idx[i]].y,
					sph01.vertices[sph01.v_idx[i]].z );			*/

		CPTriangle* tri = new CPTriangle( true );
		tri->SetCollisionMask( 1 );
		
		for ( int z = 0; z < 3; z++ )
		{
			SVector3 p = SVector3( Level_vertex[ Level_vidx[ i + z ] ].x, 
				Level_vertex[ Level_vidx[ i + z ] ].y, Level_vertex[ Level_vidx[ i + z ] ].z );
			float rads = p.z / gTrackLength * 2.0f * PI + 3.0f * PI / 2.0f;
			p.z = cos(rads) * (gTrackLength/PI/2.0f - p.y);
			p.y = sin(rads) * (gTrackLength/PI/2.0f - p.y);
			p.x = p.x;
			tri->SetPoint(p, z);
		}
		mWorld->Add( tri );
	}

	mTestTexture = CManagedTexture::Load( &gResourceManager, "metal.bmp" );
}

void CLevel::FakeLoad()
{

	mWorld = new CPhysics();

	SBoundingBox boundries;
//	boundries.min = SVector3( -58.0f, 4.9f, -54.0f );
//	boundries.max = SVector3( 53.0f, 17.0f, 69.0f );
//	mWorld->SetBoundries( boundries );
	
	LoadLevel();

//	mWorld->SortStaticObjects( 70, 3, 90 );

	//	Set up Player	
	mPlayerX = 0.0f;
	mPlayerY = -gTrackLength/PI/2.0f + 5.0f;
	mPlayerZ = 0.0f;
	mPlayerRot = 0.0f;
	mPlayerRadius = 1.2f;
	mPlayerBounce = 0.50f;
	mPlayerMass = 5.0f;
	mPlayerFriction = 0.01f;
	mPlayerDynamic = true;


		// None

	/*	Cameras	*/
	CCamera* cam = new CCamera();
	cam->SetPosition( 30.0f, 25.0f, 30.0f );
	cam->LookAt( 0.0f, 0.0f, 0.0f );
	mCameras.Add( cam );
	mBoundCamera = cam;
	
	/* Set up Controller for the Camera */
	CCameraController* camControl = new CCameraController();
	camControl->SetAnchored( false );
	camControl->SetViewAngle( 0.75f );
	camControl->SetViewDistance( 15.0f, 17.0f );
	camControl->ControlCamera( cam );
	camControl->FocusOnObject( NULL );
	mCameraControllers.Add( camControl );

	/*	Fog		*/
	mFog = false;
	mFogColorR = 0.0f;
	mFogColorG = 0.0f;
	mFogColorB = 0.0f;
	mFogColorA = 1.0f; 
	mFogDensity = 0.6f;
	mFogStart = 25.0f;
	mFogEnd = 30.0f;

	{
		GLfloat ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
		GLfloat diffusiveLight[] = { 0.6f, 0.7f, 0.6f, 1.0f };
		GLfloat position[] = { 0.5773f, 0.4773f, 0.3773f, 0.0f };
	//	GLfloat position[] = { -30.0f, 6.0f, -30.0f, 1.0f };
		mLights.Add( new CLight( ambientLight, diffusiveLight, position ) );
	}
	{
		GLfloat ambientLight[] = { 0.4f, 0.4f, 0.9f, 1.0f };
		GLfloat diffusiveLight[] = { 0.6f, 0.6f, 0.9f, 1.0f };
//		GLfloat position[] = { 0.5773f, 0.4773f, 0.3773f, 0.0f };
		GLfloat position[] = { -30.0f, 15.0f, -30.0f, 1.0f };
		GLfloat attenuation[] = { 0.0f, 0.2f, 0.0f };
		mLights.Add( new CLight( ambientLight, diffusiveLight, position, attenuation ) );
	}
}

/*
	Init()
		Clears current game set up and overwrites it with data stored in
		this CLevel object.
*/
void CLevel::Init()
{
	// Copy over the new CPhysics World
	if ( gWorld )
		delete gWorld;
	gWorld = mWorld;


	// Clear the entity list
	CListEntry<CEntity>* entity = gEntities.GetFirst();
	while ( entity )
	{
		delete entity->data;
		gEntities.RemoveEntry( entity );
		entity = gEntities.GetFirst();
	}

	CPlayer* player = new CPlayer();
	((IPhysicalObj*)player)->CreatePhysical( gWorld ); //, &pos, &vel, mPlayerRadius, mPlayerMass, mPlayerFriction, mPlayerBounce, mPlayerDynamic, PLAYER_OBJ_TYPE );
	CPObject* obj = player->GetPhysicalObject();
	SBoundingBox b;
	b.min = SVector3( -0.9f, -1.2f, -0.9f );
	b.max = SVector3( 0.9f, 0.6f, 0.9f );
	obj->SetBoundingBox( b );
	SSphere s;
	s.position = SVector3( 0.0f, -0.3f, 0.0f );
	s.radius = 0.9f;
	obj->SetPosition( SVector3( mPlayerX, mPlayerY, mPlayerZ ) );
	obj->SetSphere( s );

	obj->SetCollisionMask( 1 );
	obj->SetDefaultForce( SVector3( 0.0f, -15.0f, 0.0f ) );
	//obj->SetFriction( 6.25f );
	obj->SetFriction( 1.25f );

	player->SetRot( HALF_PI );
	gEntities.Add( (CEntity*)player );


	CListEntry<CCamera>* cameras = mCameras.GetFirst();
	while ( cameras )
	{
		gEntities.Add( (CEntity*)cameras->data );
		cameras = cameras->GetNext();
	}
	BindCamera( mBoundCamera );

	CListEntry<CCameraController>* camControllers = mCameraControllers.GetFirst();
	while ( camControllers )
	{
		camControllers->data->FocusOnObject( player );
		gEntities.Add( (CEntity*)camControllers->data );
		camControllers = camControllers->GetNext();
	}

	CListEntry<CEntity>* objects = mObjectsData.GetFirst();
	while ( objects )
	{
		gEntities.Add( objects->data );
		objects = objects->GetNext();
	}


	/* Construct Vertex Buffer Object	*/
	int numFaces = (sizeof(Level_vidx)/sizeof(long)/3);
	int numVertices = (sizeof(Level_vertex)/sizeof(_point3));
//	mLevelMesh.Create( I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_DIFFUSE, numFaces * 3, numFaces );
	
	

#ifndef INDICES
	mVertBuf = new I3DVERTBUF( true, I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_DIFFUSE | I3D_VC_UV1, numFaces * 3 );
	mIndxBuf = new I3DINDXBUF( true, numFaces * 3 );
#else
	mVertBuf = new I3DVERTBUF( true, I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_DIFFUSE | I3D_VC_UV1, numVertices );
	mIndxBuf = new I3DINDXBUF( true, numFaces * 3 );
#endif

#ifdef INDICES
	
	for ( int i = 0; i < numVertices; i++ )
	{
		mVertBuf->Lock(1);

		mVertBuf->SetPos( Level_vertex[i].x, Level_vertex[i].y, Level_vertex[i].z );
		mVertBuf->SetNormal( Level_normal[i].x, Level_normal[i].y, Level_normal[i].z );
		mVertBuf->SetColor( 0.8f, 0.5f, 0.5f );
		mVertBuf->SetUV0( Level_uv[i].x, Level_uv[i].y );
		mVertBuf->NextVert();

		mVertBuf->Unlock();
	}

	mIndxBuf->Lock();
	int itr = 0;
	for ( int i = 0; i < numFaces*3; i += 3 )
	{
		mIndxBuf->AddTri( Level_vidx[i], Level_vidx[i+1], Level_vidx[i+2] );
	}
	mIndxBuf->Unlock();
	
	

#else

	
	mIndxBuf->Lock();

	int i = 0;
	CListEntry<CPTriangle>* object = gWorld->GetStaticObjectList()->GetFirst();
	
//	mVertBuf->Lock();
	while ( object )
	{	
		mVertBuf->Lock(3);
		// Draw Triangle
		CPTriangle* tri = ((CPTriangle*)object->data);
		SVector3 normal = tri->GetNormal();

		for (int z = 0; z < 3; z++)
		{
			SVector3 p = tri->GetPoint(z);
			mVertBuf->SetPos( p.x, p.y, p.z );
			mVertBuf->SetNormal( normal.x, normal.y, normal.z );
			mVertBuf->SetColor( 0.8f, 0.5f, 0.5f );
			float uvx = Level_vertex[ Level_vidx[ i + z ] ].x;
			float uvy = Level_vertex[ Level_vidx[ i + z ] ].z + Level_vertex[ Level_vidx[ i + z ] ].y;
			mVertBuf->SetUV0(uvx/10.0f, uvy/10.0f);
			mVertBuf->NextVert();
			
		}

		mIndxBuf->AddTri( i + 0, i + 1, i + 2 );
		i+=3;			
		object = object->GetNext();
		mVertBuf->Unlock();
	}
//	mVertBuf->Unlock();
	
	mIndxBuf->Unlock();
#endif

}


/*
	Shutdown()
		Clears this level out of the current game.
*/
void CLevel::Shutdown()
{
	// Clear the entity list
	CListEntry<CEntity>* entity = gEntities.GetFirst();
	while ( entity )
	{
		if ( entity->data != this )
			delete entity->data;
//		gEntities.RemoveEntry( entity );
		entity = entity->GetNext();
	}
	
	// Copy over the old physics world
	delete gWorld;
	gWorld = NULL;
}

void CLevel::Update( float dt )
{
	CPObject* player = gPlayer->GetPhysicalObject();
	SVector3 pos = player->GetPosition();
	float hover = (pos.Length()-(gTrackLength / PI / 2.0f))*8.0f;
	if (hover < 0.0f)
		hover = -hover;
	hover = 1 / (hover*hover);
	pos.Normalize();
	pos = pos * -hover;
	player->ApplyForce(pos);
}


/*
	Lighting() and Render()
		Sets up drawing and draws current level.
*/
void CLevel::Lighting()
{
//	GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
//	GLfloat globalAmbient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	AmbientLighting( globalAmbient );

//	glEnable( GL_COLOR_MATERIAL );
//	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

	Light( 0.0f, 0.0f, 0.0f );
}


void CLevel::Light( float x, float y, float z )
{
	EnableLighting( &mLights, x, y, z );
}

void CLevel::Light( float x, float z )
{
	Light( x, 3.0f, z );
}


void CLevel::Fog()
{

	if ( mFog )
	{
		GLfloat fogColor[4] = { mFogColorR, mFogColorG, mFogColorB, mFogColorA };
		glClearColor( mFogColorR, mFogColorG, mFogColorB, 1.0f );
		glFogi( GL_FOG_MODE, GL_LINEAR );	/*	TODO: Use COptions	*/
		glFogfv( GL_FOG_COLOR, fogColor );
		glFogf( GL_FOG_DENSITY, mFogDensity );
//		glHint( GL_FOG_HINT, GL_DONT_CARE );
		glFogf( GL_FOG_START, mFogStart );
		glFogf( GL_FOG_END, mFogEnd );
		glEnable( GL_FOG );
	} else
		glDisable( GL_FOG );

}

void CLevel::Render()
{
	Lighting();

	mTestTexture->Bind();

	//glTranslatef(0.0f, 0.0f, -380.0f);
	//for (int i = 1; i < 2; i++)
	{

	#ifndef _IMMEDIATE_MODE

	#ifdef INDICES
		I3dSelectBuffers( mVertBuf, mIndxBuf );
		I3dRenderPrimIdx2( 0, mIndxBuf->mNum/3, mVertBuf->mNum );
	#else
		I3dSelectBuffers( mVertBuf, NULL );
		I3dRenderPrim( 0, mVertBuf->mNum/3, mVertBuf->mNum );
	#endif
		
	#else
		glColor3f( 0.8f, 0.5f, 0.5f );

		CListEntry<CPTriangle>* object = gWorld->GetStaticObjectList()->GetFirst();
		while ( object )
		{

			glPushMatrix();

			{	// Draw Triangle
				CPTriangle* tri = ((CPTriangle*)object->data);
				SVector3 normal = tri->GetNormal();
				SVector3 p1 = tri->GetPoint( 0 );
				SVector3 p2 = tri->GetPoint( 1 );
				SVector3 p3 = tri->GetPoint( 2 );
				
				glBegin( GL_TRIANGLES );	
					glNormal3f( normal.x, normal.y, normal.z );
					glVertex3f( p1.x, p1.y, p1.z );
					glVertex3f( p2.x, p2.y, p2.z );
					glVertex3f( p3.x, p3.y, p3.z );
				glEnd();	
			}

			object = object->GetNext();

			glPopMatrix();
		}

		

	#endif
		//glTranslatef(0.0f, 0.0f, 380.0f);
	}

	mTestTexture->Unbind();
}

