#ifndef _RESOURCE_H
#define _RESOURCE_H

#include "list.h"
#include <string>

class CManagedResource;
class CResourceManager;


enum CResourceType
{
	RESOURCE_GENERIC,
	RESOURCE_TEXTURE,
	RESOURCE_WAVESOUND,
	RESOURCE_MODEL
};



/*	Virtual class for all types of resources to be managed with
	CResourceManager		*/
class CManagedResource
{
public:
	CManagedResource();	// Defuault mReferences to 1 and mLoaded to false
	CManagedResource( std::string file );
	virtual ~CManagedResource(); /* Decrement mReferences, and log an error if it is != 0
							Then UnloadResource()	*/	

	/* static virtual CManagedResource* Load( CResourceManager*, const char* file )
	Looks in CResourceManager if the file has already been loaded,
		if so, increment reference count & return previous instance
		otherwise, create a new CManagedResource (of the appropriate type)
		and have it load char* file.
	*/
//	static virtual CManagedResource* Load( CResourceManager*, const char* file );
	void Unload();

	void IncrementReferenceCount();
	std::string GetFile();

	CResourceType ResourceType();

protected:
	virtual void Reload();
	/*	Loads data and sets mLoaded to true	*/
	virtual bool LoadResource( std::string file );
	/*	Unloads data and sets mLoaded to false	*/
	virtual void UnloadResource();

	bool mLoaded;
	int	mReferences;
	std::string mFile;
	CResourceType mResource;
};




/*	Class to manage all different types of "ManagedResources"		
	Prevents duplicate loading and keeps track of times loaded and unloaded
*/
class CResourceManager
{
public:
	CResourceManager();
	~CResourceManager();

	void UnloadAll();
	CManagedResource*	FindManagedResource( std::string file );
	void				AddResource( CManagedResource* );
	CLinkedList<CManagedResource>* GetList();
protected:
	CLinkedList<CManagedResource>	mResources;
};



#endif
