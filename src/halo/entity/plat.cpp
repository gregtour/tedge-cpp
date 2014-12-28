#include "plat.h"
#include "player.h"
#include "platform.h"
#include "gun.h"
#include "../game/game.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/model/msModel.h"
#include "../../engine/graphics/buffer/I3dVertBuf.h"
#include <cmath>

#include "spike.h"
//#include "gate.h"
#include "plat.h"
#include "player.h"
//#include "item.h"
#include "cameraController.h"
#include "gremling.h"
#include "boss.h"
#include "ghost.h"
#include "bat.h"
#include "portal.h"
#include "pickup.h"
#include "particles/spin.h"

const char*		PLAT_GROUND_TEXTURE = "brick.bmp";

CPlat::CPlat(CPhysics* p)
{
    mWorld = p;
	mTexture = CManagedTexture::Load(&gResourceManager, PLAT_GROUND_TEXTURE);
	mPlatform = NULL;
}


CPlat::~CPlat()
{

}


void CPlat::CreateMonster(SVector3 pos, CPlat::MONSTERTYPE type)
{
	int num = 5;
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
		gEntities.Add(monster);
	}
}

void CPlat::AddMonster(SVector3 pos, CPlat::MONSTERTYPE type)
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


IPhysicalObj* CPlat::LoadMS3D(const char* file, CPhysics* world)
{
	//CPlayer*	player = NULL;

	msModel* levelModel = new msModel();
	levelModel->Load(file);

	gLog.LogItem( new CLogMessage("ms3d open") );

	int numGroups = levelModel->GetNumGroups();

	for (int i = 0; i < numGroups; i++)
	{
		ms3d_group_t *group = levelModel->GetGroup(i);
		ms3d_triangle_t *first = levelModel->GetTriangle(group->triangleIndices[0]);
		SVector3 pos(levelModel->GetVertex(first->vertexIndices[0])->vertex);
		SVector3 pos2(levelModel->GetVertex(first->vertexIndices[1])->vertex);

		if (strncmp(group->name, "spa", 3) == 0)	// Player Spawn Point
 		{
			CreateMonster(pos, M_BOSS);

		} else if (strncmp(group->name, "mov", 3) == 0) {	// Moving platform, hacked to only have radius of 25f, and to circle in the XY
			for (int i = 0; i < 2; i++)
			{
				CPlatform* platform = new CPlatform();
				platform->Create(pos, 25.0f);
				platform->mTheta = PI * i;
				gEntities.Add(platform);
			}
		} else if (strncmp(group->name, "spi", 3) == 0) {	// Spinning thingy, hacked as a platform with a radius of 0
			for (int i = 0; i < 1; i++)
			{
				CPlatform* platform = new CPlatform();
				platform->Create(pos, 0.0f);
				platform->mTheta = PI * i;
				gEntities.Add(platform);
			}
		} else if (strncmp(group->name, "gun", 3) == 0) {	// Shoots cannon balls
			CGun* gun = new CGun();
			gun->SetPosition(pos);
			gun->mTime = (i%3)*0.33f;
			gEntities.Add(gun);
		} else {						// Everything else is level geometry

			bool isEnd = (strncmp(group->name,  "end", 3) == 0);	// special case end block
			int autouv = 1;

			// GRAPHICS
			int numTris = group->triangleIndices.size();

			int flags = I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_UV1;
			if (isEnd)
				flags |= I3D_VC_DIFFUSE;

			I3DVERTBUF* vertBuf = new I3DVERTBUF(true, flags, numTris*3);

			vertBuf->Lock(numTris*3);
			for ( int j = 0; j < numTris; j++ )
			{
				ms3d_triangle_t *triangle = levelModel->GetTriangle(group->triangleIndices[j]);

				for ( int k = 0; k < 3; k++ )
				{
					ms3d_vertex_t *vertex = levelModel->GetVertex(triangle->vertexIndices[k]);

					SVector3 p = vertex->vertex;

					vertBuf->SetPos((VECTOR3*)&p);

					#ifdef FLAT_SHADED
					vertBuf->SetNormal((VECTOR3*)triangle->normal);
					#else
					vertBuf->SetNormal((VECTOR3*)triangle->vertexNormals[k]);
					#endif

					if (isEnd)
						vertBuf->SetColor(1.0f, 1.0f, 1.0f);

					vertBuf->SetUV0((p.x)/2.0f, (p.z)/2.0f);
					vertBuf->NextVert();
					
				}
			}
			vertBuf->Unlock();

			// PHYSICS
			if (strncmp(group->name,  "fal", 3) != 0)	// fake block is passable
			{
				for ( int j = 0; j < numTris; j++ )
				{
					ms3d_triangle_t *triangle = levelModel->GetTriangle(group->triangleIndices[j]);

					CPTriangle* physTri = new CPTriangle(false);
					
					physTri->SetCollisionMask(1 | 2);
					
					if (isEnd)
						physTri->SetObjectType(END_OBJ_TYPE);
					else
						physTri->SetObjectType(GROUND_OBJ_TYPE);
					
					for ( int k = 0; k < 3; k++ )
					{
						ms3d_vertex_t *vertex = levelModel->GetVertex(triangle->vertexIndices[k]);
						SVector3 p = vertex->vertex;
						physTri->SetPoint(p, k);
						
					}
					
					world->Add(physTri);
				}

			}

			mVertBufs.Add(vertBuf);
		}
	}


	// Done with milkshape data
	delete levelModel;
	return NULL;
}



void CPlat::Update( float dt )
{
	return;
}

void CPlat::Render()
{
	int i = 0;
	mTexture->Bind();

	CListEntry<I3DVERTBUF>* verts = mVertBufs.GetFirst();
	while(verts)
	{

		SVector3 color = HSV((i%12)*30, 1.0f, 1.0f);
		glColor4f(color.x, color.y, color.z, 1.0f);

		I3dSelectBuffers(verts->data, NULL);
		I3dRenderPrim(0, verts->data->mNum/3, verts->data->mNum);

		verts = verts->GetNext();
		i++;
	}

	mTexture->Unbind();
}

