#ifndef _CMODEL_H
#define _CMODEL_H

#include "../common/resource.h"
#include "graphics.h"
#include <string>
//#include "buffer/I3d

class I3DVERTBUF;

class CModel
{
public:
	CModel();
	CModel( std::string file );
	CModel(I3DVERTBUF* verts);
	~CModel();

	void Load( std::string file );
	void Unload();

	I3DVERTBUF* GetVerts()
	{
		return mVerts;
	}

	void Render();
	bool IsLoaded();

protected:
	bool mLoaded;
	I3DVERTBUF*	mVerts;
};

class CManagedModel: public CManagedResource
{
public:
	CManagedModel( std::string file );
	~CManagedModel(); 

	static CModel* Load( CResourceManager* m, std::string file );
	static bool Unload( CResourceManager* m, CModel* t );

	CModel* GetModel();

protected:
	bool LoadResource( std::string file );
	void UnloadResource();

	CModel mData;
};



#endif
