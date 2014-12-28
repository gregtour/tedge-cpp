#include "resource.h"
#include "log.h"
#include "../engine.h"
#include <string>
#include <sstream>
#include <stdio.h>

CManagedResource::CManagedResource()
{
	mResource = RESOURCE_GENERIC;

	mReferences = 0;
	mLoaded = false;
}

CManagedResource::CManagedResource( std::string file )
{
	mResource = RESOURCE_GENERIC;

	mReferences = 0;
	mLoaded = false;
	LoadResource( file );
}

CManagedResource::~CManagedResource()
{
	if ( mLoaded )
		UnloadResource();

	if ( mReferences > 0 )
	{
		std::stringstream stream;
		stream << "Warning: Resource " << mFile << " unloaded with " << mReferences << " remaing reference(s).";	
		std::string warning = stream.str();
		gLog.LogItem( new CLogMessage( warning ) );
	}
}


CResourceType CManagedResource::ResourceType()
{
	return mResource;
}

/*
CManagedResource* CManagedResource::Load( CResourceManager* m, const char* file )
{
	CManagedResource* r = m->FindResource( file );
	if ( !r )
	{
		r = new CManagedResource( file );
		m->AddResource( r );
	}

	r->IncrementReferenceCount();
	return r;
}
*/

void CManagedResource::Unload()
{
	mReferences--;
	if ( mReferences == 0 )
	{
		UnloadResource();
	}
}	

void CManagedResource::IncrementReferenceCount()
{
	mReferences++;
	if ( !mLoaded )
		Reload();
}

std::string CManagedResource::GetFile()
{
	return mFile;
}

bool CManagedResource::LoadResource( std::string file )
{
	mLoaded = true;
	mFile = file;
	return true;
}

void CManagedResource::UnloadResource()
{
	mLoaded = false;
}

void CManagedResource::Reload()
{
	if ( !mLoaded )
		LoadResource( mFile );
}


CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
	UnloadAll();
}

void CResourceManager::UnloadAll()
{
	CListEntry<CManagedResource>* resources = mResources.GetFirst();
	while ( resources )
	{
		delete resources->data;
		mResources.RemoveEntry( resources );
		resources = mResources.GetFirst();
	}
}

CManagedResource* CResourceManager::FindManagedResource( std::string file )
{
	CListEntry<CManagedResource>* resources = mResources.GetFirst();
	while ( resources )
	{
		if ( file == resources->data->GetFile() )
			return resources->data;
		resources = resources->GetNext();
	}

	return NULL;
}

void CResourceManager::AddResource( CManagedResource* resource )
{
	mResources.Add( resource );
}


CLinkedList<CManagedResource>* CResourceManager::GetList()
{
	return &mResources;
}
