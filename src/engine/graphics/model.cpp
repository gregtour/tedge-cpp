#include "model.h"
#include "../common/log.h"
#include "../engine.h"
#include "model/msModel.h"
#include "buffer/I3dVertBuf.h"


CModel::CModel()
{
	mLoaded = false;
	mVerts = NULL;
}

CModel::CModel( std::string file )
{
	mLoaded = false;
	Load( file );
}

CModel::CModel(I3DVERTBUF* verts)
{
	mLoaded = (verts != NULL);
	mVerts = verts;
}

CModel::~CModel()
{
	Unload();
}

void CModel::Load( std::string file )
{
	msModel* ms = new msModel();
	ms->Load(file.c_str());
	if(ms->GetNumGroups()==0)
	{
		mLoaded = false;
		delete ms;
		return;
	}

	ms3d_group_t *group = ms->GetGroup(0);
	int numTris = group->triangleIndices.size();
	
	//mVerts = new I3DVERTBUF(true, I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_DIFFUSE | I3D_VC_UV1, numTris*3);
	mVerts = new I3DVERTBUF(true, I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_UV1, numTris*3);

	mVerts->Lock(numTris*3);
	for ( int j = 0; j < numTris; j++ )
	{
		ms3d_triangle_t *triangle = ms->GetTriangle(group->triangleIndices[j]);

        // calculate normal by hand
        SVector3 a(ms->GetVertex(triangle->vertexIndices[0])->vertex);
        SVector3 b(ms->GetVertex(triangle->vertexIndices[1])->vertex);
        SVector3 c(ms->GetVertex(triangle->vertexIndices[2])->vertex);

        SVector3 normal;
        normal = CrossProduct(c - a, c - b);
        normal.Normalize();

	
		for ( int k = 0; k < 3; k++ )
		{
			ms3d_vertex_t *vertex = ms->GetVertex(triangle->vertexIndices[k]);

			SVector3 p = vertex->vertex;
			mVerts->SetPos((VECTOR3*)vertex->vertex);
            mVerts->SetNormal((VECTOR3*)&normal);
/*			#ifdef FLAT_SHADED
			mVerts->SetNormal((VECTOR3*)triangle->normal);
			#else
			mVerts->SetNormal((VECTOR3*)triangle->vertexNormals[k]);
			#endif */

			//mVerts->SetColor(1.0f, 1.0f, 1.0f);
			mVerts->SetUV0(triangle->s[k], triangle->t[k]);
			mVerts->NextVert();
		}
	}
	mVerts->Unlock();

	delete ms;
	mLoaded = true;
}

void CModel::Unload()
{
	if ( mLoaded )
	{
		delete mVerts;
		mLoaded = false;
	}
}

void CModel::Render()
{
	if (mLoaded)
	{
		I3dSelectBuffers(mVerts, NULL);
		I3dRenderPrim(0, mVerts->mNum/3, mVerts->mNum);
	}
}

bool CModel::IsLoaded()
{
	return mLoaded;
}


CManagedModel::CManagedModel( std::string file )
{
	mResource = RESOURCE_MODEL;
	mReferences = 0;
	mLoaded = false;
	LoadResource( file );
}


CManagedModel::~CManagedModel()
{
	UnloadResource();
}

CModel* CManagedModel::Load( CResourceManager* m, std::string file )
{
	CManagedModel* r = (CManagedModel*)(m->FindManagedResource( file ));
	if ( !r )
	{
		r = new CManagedModel( file );
		m->AddResource( r );
	}

	r->IncrementReferenceCount();
	return r->GetModel();
}

bool CManagedModel::Unload( CResourceManager* m, CModel* t )
{
	CLinkedList<CManagedResource>* list = m->GetList();
	CListEntry<CManagedResource>* entry = list->GetFirst();
	while ( entry )
	{
		if ( entry->data->ResourceType() == RESOURCE_MODEL )
		{
			if ( ((CManagedModel*)entry->data)->GetModel() == t )
			{
				entry->data->Unload();
				return true;
			}
		}
		entry = entry->GetNext();
	}

	/*	Failed to find CTexture* t in CResourceManager* m!	*/
	return false;
}

CModel* CManagedModel::GetModel()
{
	return &mData;
}

bool CManagedModel::LoadResource( std::string file )
{
	mData.Load( file );
	mFile = file;
	mLoaded = mData.IsLoaded();
	return mLoaded;
}

void CManagedModel::UnloadResource()
{
	mData.Unload();
	mLoaded = false;
}
