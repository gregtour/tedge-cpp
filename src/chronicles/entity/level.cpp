// GREG:
// this monstrosity must be destroyed
// they know not what they have done
// continue at your own risk:

#include <sstream>
#include "../main.h"

#include "spike.h"
#include "gate.h"
#include "level.h"
#include "player.h"
#include "item.h"
#include "cameraController.h"
#include "gremling.h"
#include "boss.h"
#include "ghost.h"
#include "bat.h"
#include "portal.h"
#include "pickup.h"
#include "particles/spin.h"
#include <sstream>
#include <iostream>



#include "../../engine/update.h"
#include "../../engine/physics/physics.h"
#include "../../engine/physics/object.h"
#include "../../engine/graphics/graphics.h"
#include "../../engine/graphics/lighting.h"
//#include "../../engine/graphics/model/MilkshapeModel.h"
#include "../../engine/graphics/model/msModel.h"
#include "../../engine/entity/camera.h"
#include "../game/game.h"
#include <cmath>
#include <cstdlib>


#define		MAX_LEVEL_TRIS		10000
#define		FLAT_SHADED

I3DVERTBUF*		CLevel::gTempVert = NULL;

#ifdef 		_PSP
#define		SOFTWARE_CLIPPING
#include "../../engine/graphics/clipping/math.h"
#include "../../engine/graphics/clipping/clipping.h"

extern ScePspFMatrix4 	model_view_matrix;
extern ScePspFMatrix4 	projection_view_matrix;
extern ScePspFMatrix4 	projection_view_model_matrix;
#endif


CLevel::CLevel()
{
	if (!gTempVert)
		gTempVert = new I3DVERTBUF(true, I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_DIFFUSE | I3D_VC_UV1, MAX_LEVEL_TRIS*3);

	mLevelText = CManagedTexture::Load( &gResourceManager, "data/level.bmp" );
	mLevelMask = CManagedTexture::Load( &gResourceManager, "data/levelmask.bmp" );
	mLevelFont = new CFontRenderer( "data/digits.bmp", "data/digitsmask.bmp", 10, "0123456789" );

	mLevelModel = NULL;
}


/*
	~CLevel()
		Deletes any excess data, especially data created from Load().
*/
CLevel::~CLevel()
{
	delete mLevelFont;
}

void CLevel::CreateMonster(SVector3 pos, CLevel::MONSTERTYPE type)
{
	int num = gLevelNumber;
	if (type == M_BOSS || type == M_BAT)
		num = num / 2;
	for (int i = 0; i < num; i++)
	{
		pos.x += (rand()%100-50)/7.0f;
		pos.z += (rand()%100-50)/7.0f;

		gLog.LogItem( new CLogMessage("found monster") );
		CMonster* monster;
		SBoundingBox	b;
		SSphere			s;

		switch(type)
		{
		case M_BAT:
			s.radius = 0.7f;
			monster = new CBat( -40.1f, 1.0f, 2.0f, 2.0f );
			monster->SetHealth(2);
			break;

		case M_BOSS:
			s.radius = 3.5f;
			monster = new CBoss( 17.0f, 20.0f );
			monster->SetHealth(7);
			break;

		case M_GHOST:
			s.radius = 0.7f;
			monster = new CGhost( -40.7f );
			monster->SetHealth(2);
			break;

		default:
		case M_GREMLING:
			s.radius = 1.3f;
			monster = new CGremling(30.0f, 35.0f);
			monster->SetHealth(3);
			break;
		}

		monster->CreatePhysical(mWorld);
		monster->SetRot(pos.x+pos.y);

		CPObject* obj = monster->GetPhysicalObject();
		s.position = SVector3( 0.0f, 0.0f, 0.0f );
		b.min = SVector3( -s.radius, -s.radius, -s.radius );
		b.max = SVector3( s.radius, s.radius, s.radius );
		obj->SetBoundingBox( b );
		obj->SetSphere( s );
		obj->SetPosition( SVector3( pos.x, pos.y, pos.z ) );
		obj->SetObjectType(MONSTER_OBJ_TYPE);

		obj->SetCollisionMask( 1 | 2 );
		if (type != M_BAT && type != M_GHOST)
			obj->SetDefaultForce( SVector3( 0.0f, -45.0f, 0.0f ) );
		else 
			obj->SetDefaultForce( SVector3( 0.0f, 0.0f, 0.0f ) );
		obj->SetFriction( 3.25f );
		//obj->SetFriction( 1.25f );
		//gEntities.Add(monster);
		mObjectsData.Add(monster);
	}
}

void CLevel::AddMonster(SVector3 pos, CLevel::MONSTERTYPE type)
{

	CMonster* monster;
	SBoundingBox	b;
	SSphere			s;

	switch(type)
	{
	case M_BAT:
		s.radius = 0.6f;
		monster = new CBat( -40.1f, 1.0f, 2.0f, 2.0f );
		monster->SetHealth(2);
		break;

	case M_BOSS:
		s.radius = 3.5f;
		monster = new CBoss( 12.0f, 15.0f );
		monster->SetHealth(7);
		break;

	case M_GHOST:
		s.radius = 0.6f;
		monster = new CGhost( -40.7f );
		monster->SetHealth(2);
		break;

	default:
	case M_GREMLING:
		s.radius = 1.3f;
		monster = new CGremling(8.0f, 10.0f);
		monster->SetHealth(3);
		break;
	}

	monster->CreatePhysical(mWorld);

	CPObject* obj = monster->GetPhysicalObject();
	s.position = SVector3( 0.0f, 0.0f, 0.0f );
	b.min = SVector3( -s.radius, -s.radius, -s.radius );
	b.max = SVector3( s.radius, s.radius, s.radius );
	obj->SetBoundingBox( b );
	obj->SetSphere( s );
	obj->SetPosition( SVector3( pos.x, pos.y, pos.z ) );
	obj->SetObjectType(MONSTER_OBJ_TYPE);
	obj->SetOwner(monster);

	obj->SetCollisionMask( 1 | 2 );
	if (type != M_BAT && type != M_GHOST)
		obj->SetDefaultForce( SVector3( 0.0f, -45.0f, 0.0f ) );
	else 
		obj->SetDefaultForce( SVector3( 0.0f, 0.0f, 0.0f ) );
	obj->SetFriction( 3.25f );
	//obj->SetFriction( 1.25f );
	gEntities.Add(monster);
	//mObjectsData.Add(monster);
}

void CLevel::LoadMS3D(char* file)
{
	if (mLevelModel)
	{
		delete mLevelModel;
		mLevelModel = NULL;
	}

	
	gLog.LogItem( new CLogMessage("opening ms3d") );
	//mLevelModel = new MilkshapeModel();
	//mLevelModel->loadModelData(file);
	
	std::stringstream f;
//	f << "poly/" << file;
	f << "phys/" << file;
	mLevelModel = new msModel();
	mLevelModel->Load(f.str().c_str());

	std::stringstream p;
//	p << "phys/" << file;
	msModel* low;
	low = mLevelModel;
//	low = new msModel();
//	low->Load(p.str().c_str());

	gLog.LogItem( new CLogMessage("ms3d open") );

	int numGroups = mLevelModel->GetNumGroups();

	

	/*int numGroups = mLevelModel->NumMeshes();
	Model::Mesh* groups = mLevelModel->GetMeshes();
	Model::Vertex* verts = mLevelModel->GetVerts();
	Model::Triangle* tris = mLevelModel->GetTris();
	Model::Material* mats = mLevelModel->GetMaterials();*/

	mPortal = true;

	for (int i = 0; i < numGroups; i++)
	{
		ms3d_group_t *group = mLevelModel->GetGroup(i);
		ms3d_triangle_t *first = mLevelModel->GetTriangle(group->triangleIndices[0]);
		SVector3 pos(mLevelModel->GetVertex(first->vertexIndices[0])->vertex);
		SVector3 pos2(mLevelModel->GetVertex(first->vertexIndices[1])->vertex);

		// first, check the name for the type of group
		// spawn, end, bat*, grem*, level*
		if (strncmp(group->name, "spa", 3) == 0)
 		{
			// m_pVertices[m_pTriangles[m_pMeshes[i].m_pTriangleIndicies[0-100]].m_vertexIndices[0-3]].m_location
			gLog.LogItem( new CLogMessage("Found spawn") );
			mPlayerX = pos.x;
			mPlayerY = pos.y;
			mPlayerZ = pos.z;
			mPlayerRot = 0.0f;
			mPlayerRadius = 1.2f;
			mPlayerBounce = 0.50f;
			mPlayerMass = 5.0f;
			mPlayerFriction = 0.01f;
			mPlayerDynamic = true;
			mObjectsData.Add( new CSpin( pos.x, pos.y-3.0f, pos.z ) );

			// save spawn position
		} else if (strncmp(group->name,  "end", 3) == 0) {
			// save portal position
			mPortalX = pos.x;
			mPortalY = pos.y - 2.0f;
			mPortalZ = pos.z;
			CLight* light = new CLight();
			float position[] = {pos.x, pos.y, pos.z, 1.0f};
			float ambient[] = {0.6f, 0.6f, 1.0f, 1.0f}; 
			float diffusive[] = {0.6f, 0.6f, 1.0f, 1.0f};
			float attenuation[] = {0.0f, 0.006f, 0.00017f};
			light->SetPosition( position );
			light->SetAmbient( ambient );
			light->SetDiffusive( diffusive );
			light->SetAttenuation( attenuation );
			mLights.Add(light);
		} else if (strncmp(group->name, "bat", 3) == 0) {
			// make monsters
			CreateMonster(pos, M_BAT);
		} else if (strncmp(group->name, "gre", 3) == 0) {
			CreateMonster(pos, M_GREMLING);
		} else if (strncmp(group->name, "bos", 3) == 0) {
			mPortal = false;
			CreateMonster(pos, M_BOSS);
		} else if (strncmp(group->name, "gat", 3) == 0) {
			CGate* gate = new CGate();
			gate->CreatePhysical( mWorld, &pos, &pos2 );
			mObjectsData.Add( gate );
		} else if (strncmp(group->name, "spi", 3) == 0) {
			CSpike* spike = new CSpike(0.5f + (rand()%250/300.0f));
			spike->CreatePhysical( mWorld, &pos, &pos2 );
			mObjectsData.Add( spike );
		} else if (strncmp(group->name, "gho", 3) == 0) {
			//mObjectsData.Add( new CFire( pos.x, pos.y, pos.z ) );
			CreateMonster(pos, M_GHOST);
		} else if (strncmp(group->name, "way", 3) == 0) {		// camera waypoint
			//mObjectsData.Add(new CWaypoint(pos));
			gLog.LogItem(new CLogMessage("Found waypoint."));
			std::cout << "Found waypoint.";
			mCameraController->AddWaypoint(pos);
		} else if (strncmp(group->name, "key", 3) == 0) {
			//mObjectsData.Add( new CFire( pos.x, pos.y, pos.z ) );
			//CreateMonster(pos, M_GHOST);
			CPickup* pickup = new CPickup(KEY_PICKUP_TYPE);
			pickup->CreatePhysical(mWorld);
			CPObject* obj = pickup->GetPhysicalObject();
			obj->SetCollisionMask(4);
			obj->SetObjectType(PICKUP_OBJ_TYPE);
			SBoundingBox	b;
			SSphere			s;
			s.radius = 3.0f;
			s.position = SVector3( 0.0f, 0.0f, 0.0f );
			b.min = SVector3( -s.radius, -s.radius, -s.radius );
			b.max = SVector3( s.radius, s.radius, s.radius );
			obj->SetBoundingBox( b );
			obj->SetSphere( s );
			obj->SetPosition( SVector3( pos.x, pos.y-2.5f, pos.z ) );
			//obj->SetDefaultForce(SVector3(0.0f, -5.0f, 0.0f));
			mObjectsData.Add(pickup);
		} else if (strncmp(group->name, "lvl", 3) == 0) {
			// deal with level geometry
			int autouv = 0;
			if (strncmp(group->name, "lvl_fl", 6) == 0)
				autouv = 1;
			else if (strncmp(group->name, "lvl_wa", 6) == 0)
				autouv = 2;

			int numTris = group->triangleIndices.size();

			I3DVERTBUF* vertBuf = new I3DVERTBUF(true, I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_DIFFUSE | I3D_VC_UV1, numTris*3);

			vertBuf->Lock(numTris*3);
			for ( int j = 0; j < numTris; j++ )
			{
				//int triangleIndex = groups[i].m_pTriangleIndices[j];
				//const Model::Triangle* pTri = &tris[triangleIndex];
				ms3d_triangle_t *triangle = mLevelModel->GetTriangle(group->triangleIndices[j]);

				for ( int k = 0; k < 3; k++ )
				{
					ms3d_vertex_t *vertex = mLevelModel->GetVertex(triangle->vertexIndices[k]);
					//int index = pTri->m_vertexIndices[k];

					// Physics:
					SVector3 p = vertex->vertex;

					//glNormal3fv( pTri->m_vertexNormals[k] );
					//glTexCoord2f( pTri->m_s[k], pTri->m_t[k] );
					//glVertex3fv( m_pVertices[index].m_location );
					vertBuf->SetPos((VECTOR3*)&p);

					#ifdef FLAT_SHADED
					vertBuf->SetNormal((VECTOR3*)triangle->normal);
					#else
					vertBuf->SetNormal((VECTOR3*)triangle->vertexNormals[k]);
					#endif

					vertBuf->SetColor(1.0f, 1.0f, 1.0f);

					if (autouv == 0)
					{
						vertBuf->SetUV0(triangle->s[k], triangle->t[k]);
					} else if (autouv == 1) {
						vertBuf->SetUV0((p.x)/10.0f, (p.z)/10.0f);
					} else {
						vertBuf->SetUV0((p.x+p.z)/10.0f, (p.y)/10.0f);
					}

					//vertBuf->SetUV0((p.x + p.z)/10.0f, (p.y + p.z - p.x)/10.0f);
					vertBuf->NextVert();
					
				}
				//indxBuf->AddTri(pTri->m_vertexIndices[0], pTri->m_vertexIndices[1],pTri->m_vertexIndices[2]);			

			}
			vertBuf->Unlock();

			mVertBufs.Add(vertBuf);
			mMeshTextures.Add(CManagedTexture::Load( &gResourceManager, mLevelModel->GetMaterial(group->materialIndex)->texture));
		} else if (strncmp(group->name, "lig", 3) == 0) {
			CLight* light = new CLight();
			float position[] = {pos.x, pos.y, pos.z, 1.0f};
			float ambient[] = {0.8f, 0.8f, 0.8f, 1.0f}; 
			float diffusive[] = {0.7f, 0.3f, 0.5f, 1.0f};
			float attenuation[] = {0.0f, 0.003f, 0.00017f};
			light->SetPosition( position );
			light->SetAmbient( ambient );
			light->SetDiffusive( diffusive );
			light->SetAttenuation( attenuation );
			mLights.Add(light);
			//mObjectsData.Add( new CFire( pos.x, pos.y-2.0f, pos.z ) );
		}
	}

	numGroups = low->GetNumGroups();
	for (int i = 0; i < numGroups; i++)
	{
		ms3d_group_t *group = low->GetGroup(i);
		ms3d_triangle_t *first = mLevelModel->GetTriangle(group->triangleIndices[0]);
		SVector3 pos(mLevelModel->GetVertex(first->vertexIndices[0])->vertex);
		SVector3 pos2(mLevelModel->GetVertex(first->vertexIndices[1])->vertex);

		// first, check the name for the type of group
		// spawn, end, bat*, grem*, level*
		if (strncmp(group->name, "lvl", 3) == 0) {

			int numTris = group->triangleIndices.size();

			for ( int j = 0; j < numTris; j++ )
			{
				//int triangleIndex = groups[i].m_pTriangleIndices[j];
				//const Model::Triangle* pTri = &tris[triangleIndex];
				ms3d_triangle_t *triangle = low->GetTriangle(group->triangleIndices[j]);

				CPTriangle* physTri = new CPTriangle(false);
				physTri->SetCollisionMask(1);
				
				for ( int k = 0; k < 3; k++ )
				{
					ms3d_vertex_t *vertex = low->GetVertex(triangle->vertexIndices[k]);
					SVector3 p = vertex->vertex;
					physTri->SetPoint(p, k);
					
				}
				//indxBuf->AddTri(pTri->m_vertexIndices[0], pTri->m_vertexIndices[1],pTri->m_vertexIndices[2]);			

				mWorld->Add(physTri);
			}
		}
	}


	// Done with milkshape data
	delete mLevelModel;
	mLevelModel = NULL;
//	delete low;
}


void CLevel::LoadGameMenu()
{
	gPlayer = NULL;

	/*	Hard coded values instead of loading from file	*/
	mWorld = new CPhysics();
	mWorld->SetCollisionFunction(&GameCollisionFunction);

	SBoundingBox boundries;
	boundries.min = SVector3( 0.0f, -2.0f, 0.0f );
	boundries.max = SVector3( 340.0f, 80.0f, 300.0f );
	mWorld->SetBoundries( boundries );
	
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
	camControl->EnableWaypoints();
	mCameraController = camControl;
	//mCameraControllers.Add( camControl );


	LoadMS3D("menu.ms3d");
//	LoadMS3D("stone/level1.ms3d");
	mSky = CManagedTexture::Load( &gResourceManager, "marble.bmp"  );
	gLog.LogItem( new CLogMessage("ms3d done!") );

	mPortal = true;

	mWorld->SortStaticObjects( 20, 5, 20 );



	/*	Fog		*/
	mFog = false;
	mFogColorR = 0.0f;
	mFogColorG = 0.0f;
	mFogColorB = 0.0f;
	mFogColorA = 1.0f; 
	mFogDensity = 0.6f;
	mFogStart = 25.0f;
	mFogEnd = 30.0f;
}




void CLevel::LoadLevel(std::string path, int num)
{
	CPlayer* player = new CPlayer();
	gPlayer = player;

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
	camControl->FocusOnObject( gPlayer );
	mCameraController = camControl;

	/*	Hard coded values instead of loading from file	*/
	mWorld = new CPhysics();
	mWorld->SetCollisionFunction(&GameCollisionFunction);

	SBoundingBox boundries;
	boundries.min = SVector3( 0.0f, -2.0f, 0.0f );
	boundries.max = SVector3( 340.0f, 80.0f, 300.0f );
	mWorld->SetBoundries( boundries );
	
	gLog.LogItem( new CLogMessage("opening level ms3d") );
	char levelFile[128];
	if (num < 0)
		num = 0;
	while (num > 6)
		num -= 6;
	sprintf(levelFile, "%s/level%i.ms3d", path.c_str(), num);
	LoadMS3D(levelFile);
	mSky = CManagedTexture::Load( &gResourceManager, "marble.bmp"  );
	gLog.LogItem( new CLogMessage("ms3d done!") );

	mWorld->SortStaticObjects( 20, 10, 20 );

	/*	Fog		*/
	mFog = false;
	mFogColorR = 0.0f;
	mFogColorG = 0.0f;
	mFogColorB = 0.0f;
	mFogColorA = 1.0f; 
	mFogDensity = 0.6f;
	mFogStart = 25.0f;
	mFogEnd = 30.0f;
}

/*
	Init()
		Clears current game set up and overwrites it with data stored in
		this CLevel object.
*/
void CLevel::Init()
{
	gLog.LogItem( new CLogMessage("init-ing level") );

	// Copy over the new CPhysics World
	if ( gWorld )
		delete gWorld;
	gWorld = mWorld;


	// Clear the entity list
	CListEntry<CEntity>* entity = gEntities.GetFirst();
	while (entity)
	{
		delete entity->data;
		gEntities.RemoveEntry( entity );
		entity = gEntities.GetFirst();
	}

	if (gPlayer)
	{
		CPlayer* player = gPlayer;
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
		obj->SetObjectType(PLAYER_OBJ_TYPE);

		obj->SetCollisionMask( 1 | 4 );
		obj->SetDefaultForce( SVector3( 0.0f, -45.0f, 0.0f ) );
		//obj->SetFriction( 6.25f );
		obj->SetFriction( 3.75f );

		player->SetRot( HALF_PI );
		gEntities.Add( (CEntity*)player );
	}


	if (mPortal)
	{
		CreatePortal();
	}


	gEntities.Add( mBoundCamera );
	BindCamera( mBoundCamera );

	gEntities.Add( mCameraController );

	CListEntry<CEntity>* objects = mObjectsData.GetFirst();
	while ( objects )
	{
		gEntities.Add( objects->data );
		objects = objects->GetNext();
	}


}


void CLevel::CreatePortal()
{
	SVector3 pos( mPortalX, mPortalY, mPortalZ );
	SBoundingBox b;
	SSphere s;

	CPortal* endPortal = new CPortal();
	//((IPhysicalObj*)endPortal)->CreatePhysical( gWorld, &pos, &vec, PORTAL_SIZE, 0.1f, 0.0f, 0.0f, false, PORTAL_OBJECT_TYPE );
	endPortal->CreatePhysical(gWorld);
	CPObject* obj = endPortal->GetPhysicalObject();
	b.min = SVector3( -PORTAL_SIZE, -4.2f, -PORTAL_SIZE );
	b.max = SVector3( PORTAL_SIZE, PORTAL_SIZE*1.5f, PORTAL_SIZE );
	obj->SetBoundingBox( b );
	s.position = SVector3( 0.0f, -0.3f, 0.0f );
	s.radius = PORTAL_SIZE;
	obj->SetPosition( pos );
	obj->SetSphere( s );
	obj->SetObjectType(PORTAL_OBJECT_TYPE);

	obj->SetCollisionMask( 4 );
	obj->SetFriction( 99.0f );

	gEntities.Add( endPortal );
}


/*
	Shutdown()
		Clears this level out of the current game.
*/
void CLevel::Shutdown()
{

	// Delete old milkshape model
	if (mLevelModel)
	{
		delete mLevelModel;
		mLevelModel = NULL;
	}

	// Delete old level geometry
	CListEntry<I3DVERTBUF>* verts = mVertBufs.GetFirst();
	CListEntry<CTexture>*   texs  = mMeshTextures.GetFirst();
	while(verts)
	{
		delete verts->data;

		mVertBufs.RemoveEntry(verts);
		mMeshTextures.RemoveEntry(texs);
		verts = mVertBufs.GetFirst();
		texs  = mMeshTextures.GetFirst();		
	}

	// Clear the entity list
	CListEntry<CEntity>* entity = gEntities.GetFirst();
	while ( entity )
	{
		//if ( entity->data != this )
		{
			delete entity->data;
			gEntities.RemoveEntry(entity);
			entity = gEntities.GetFirst();
		}
//		gEntities.RemoveEntry( entity );
	}
	
	// Delete the old physics world
	delete gWorld;
	gWorld = NULL;
}

void CLevel::Update( float dt )
{

}


/*
	Lighting() and Render()
		Sets up drawing and draws current level.
*/
void CLevel::Lighting()
{
//	GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat globalAmbient[] = { 0.9f, 0.9f, 0.9f, 0.0f };
	AmbientLighting( globalAmbient );

//	glEnable( GL_COLOR_MATERIAL );
//	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

	Light( 0.0f, 0.0f, 0.0f );
}


void CLevel::Light( float x, float y, float z )
{
	EnableLighting( &mLights, x, y, z );
}

void CLevel::Light( SVector3 p )
{
	EnableLighting( &mLights, p.x, p.y, p.z );
}

void CLevel::Light( float x, float z )
{
	Light( x, 3.0f, z );
}


void CLevel::Fog()
{
	mFog = false;
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

#ifdef SOFTWARE_CLIPPING
int Clip(I3DVERTBUF* in, I3DVERTBUF* out)
{
	int outSize = 0;

	float camLook[3];
	// forward vector from model_view_matrix
	float fx, fy, fz;
/*	fx = model_view_matrix.z.x;
	fy = model_view_matrix.z.y;
	fz = model_view_matrix.z.z;*/
	float cx, cy, cz;
	gCamera->GetPosition(&cx, &cy, &cz);
	gCamera->GetLookAt(&camLook[0], &camLook[1], &camLook[2]);
	fx = cx - camLook[0];
	fy = cy - camLook[1];
	fz = cz - camLook[2];
	
	out->Lock(out->mNum);
	in->Lock(in->mNum);
	for (int i = 0; i < in->mNum; i += 3)
	{
		glvert_s tri[3];
		float normal[3];
		COLOR4B color;

		const glvert_s* res;
		size_t sizeClipped;
		int numClipped;

		in->GetPos(tri[0].xyz[0], tri[0].xyz[1], tri[0].xyz[2]);

		in->GetUVs(tri[0].st[0], tri[0].st[1]);
		in->GetNormal(normal[0], normal[1], normal[2]);
		in->GetColor(color);
		in->NextVert();
		for (int k = 1; k < 3; k++)
		{
			in->GetPos(tri[k].xyz[0], tri[k].xyz[1], tri[k].xyz[2]);
			in->GetUVs(tri[k].st[0], tri[k].st[1]);
			in->NextVert();
		}
		

		// check that at least one point is in front of near
		//   and at least one is in front of far
//		if(is_in_front(tri[0].xyz, tri[1].xyz, tri[2].xyz))
		{
	
			if (is_clipping_required(tri, 3))
			{
				clip(tri, 3, &res, &sizeClipped);
	//			numClipped = sizeClipped / sizeof(glvert_s);
				numClipped = sizeClipped;
				if (numClipped >= 3)
				{
					for (int t = 1; t < numClipped - 1; t++)
					{
						out->SetPos(res[0].xyz[0], res[0].xyz[1], res[0].xyz[2]);
						out->SetUV0(res[0].st[0], res[0].st[1]);
						out->SetColor(color);
						out->SetNormal((VECTOR3*)normal);
						out->NextVert();

						for (int k = t; k < t+2; k++)
						{
							out->SetPos(res[k].xyz[0], res[k].xyz[1], res[k].xyz[2]);
							out->SetUV0(res[k].st[0], res[k].st[1]);
							out->SetColor(color);
							out->SetNormal((VECTOR3*)normal);
							out->NextVert();				
						}
						outSize += 3;
					}
				}
			} else {
				for (int k = 0; k < 3; k++)
				{
					out->SetPos(tri[k].xyz[0], tri[k].xyz[1], tri[k].xyz[2]);
					out->SetUV0(tri[k].st[0], tri[k].st[1]);
					out->SetColor(color);
					out->SetNormal((VECTOR3*)normal);
					out->NextVert();
				}
				outSize += 3;
			}

		} 
	}
	in->Unlock();
	out->Unlock();

	return outSize;
}
#endif

void CLevel::Render()
{
	Lighting();

#ifdef SOFTWARE_CLIPPING
	begin_brush_model();
#endif


	// New Rendering
	CListEntry<I3DVERTBUF>* verts = mVertBufs.GetFirst();
	CListEntry<CTexture>*   texs  = mMeshTextures.GetFirst();
	while(verts)
	{
#ifdef SOFTWARE_CLIPPING
		int numClippedVerts = Clip(verts->data, gTempVert);
#endif


		texs->data->Bind();
		//I3dSelectBuffers(verts->data, indxs->data);
		//I3dRenderPrimIdx2(0, indxs->data->mNum/3, verts->data->mNum);
//		I3dSelectBuffers(verts->data, NULL);
//		I3dRenderPrim(0, verts->data->mNum/3, verts->data->mNum);

#ifdef SOFTWARE_CLIPPING
		I3dSelectBuffers(gTempVert, NULL);
		I3dRenderPrim(0, numClippedVerts / 3, numClippedVerts);
#else
		I3dSelectBuffers(verts->data, NULL);
		I3dRenderPrim(0, verts->data->mNum/3, verts->data->mNum);
#endif
		verts = verts->GetNext();
		texs  = texs->GetNext();
	}
	mMeshTextures.GetFirst()->data->Unbind();

#ifdef SOFTWARE_CLIPPING
	end_brush_model();
#endif
}

