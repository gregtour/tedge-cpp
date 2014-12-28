#include "../graphics/model.h"
#include "../graphics/buffer/I3dVertBuf.h"
#include "physicalObj.h"
#include "../engine.h"

IPhysicalObj::IPhysicalObj()
{
	mObject = NULL;
	mWorld = NULL;
	mRot = 0.0f;
}

IPhysicalObj::~IPhysicalObj()
{
	if ( mObject )
	{
		SetWorld( NULL );
		delete mObject;
	}
}

void IPhysicalObj::CreatePhysical( CPhysics* world )
{
	mObject = new CPObject( true );
	mObject->SetOwner(this);
	SetWorld( world );
}

void IPhysicalObj::CreatePhysical(CPhysics* world, CModel* mesh)
{
	CPMesh* m = new CPMesh();
	mObject = m;
	mObject->SetOwner(this);
	
	I3DVERTBUF* verts = mesh->GetVerts();
	verts->Lock();
	for (int i = 0; i < verts->mNum; i += 3)
	{
		CPTriangle* tri = new CPTriangle(false);
		for (int j = 0; j < 3; j++)
		{
			SVector3 p;
            verts->GetPos(p.x, p.y, p.z);
			tri->SetPoint(p, j);
			verts->NextVert();
		}
		m->Add(tri);
	}
	verts->Unlock();

	SetWorld( world );
}

void IPhysicalObj::SetWorld( CPhysics* world )
{
	if ( mObject && mWorld )
		mWorld->Remove( mObject );

	mWorld = world;
	
	if ( mObject && mWorld )
		mWorld->Add( mObject );
}

CPObject* IPhysicalObj::GetPhysicalObject()
{
	return mObject;
}

float IPhysicalObj::GetRot()
{
	return mRot;
}

void IPhysicalObj::SetRot( float r )
{
	mRot = r;
}
