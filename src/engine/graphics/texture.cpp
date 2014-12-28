#include "texture.h"
#include "../common/log.h"
#include "../engine.h"
#include "SDL_image.h"
#include <sstream>

bool CTexture::gTexEnabled = false;


CTexture::CTexture()
{
	mWidth = 0;
	mHeight = 0;
	mLoaded = false;
}

CTexture::CTexture( std::string file )
{
	mLoaded = false;
	Load( file );
}

CTexture::~CTexture()
{
	Unload();
}

void CTexture::LoadAlpha( std::string file )
{
	std::stringstream er;
	er << "Creating alpha texture for file " << file;
	gLog.LogItem(new CLogMessage(er.str()));

	SDL_Surface* temp = IMG_Load( file.c_str() );

	if ( !temp )// Failed to Load
	{
		std::string error = IMG_GetError();
		std::string message = "IMG_Load failed on file " + file + " with error: " + error;
		gLog.LogItem( new CLogMessage(message) );
		mLoaded = false;
		return;
	}

	mWidth = temp->w;
	mHeight = temp->h;

	
#ifndef _PSP
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
#endif

	glGenTextures( 1, &mTextureId );
	glBindTexture( GL_TEXTURE_2D, mTextureId );

#ifndef _PSP
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
#endif

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);

	SDL_FreeSurface( temp );
	mLoaded = true;

	std::stringstream er2;
	er2 << "Texture loaded successfully for file " << file;
	gLog.LogItem(new CLogMessage(er2.str()));
}

void CTexture::Load( std::string file )
{
	SDL_Surface* temp = IMG_Load(file.c_str());
		if (!temp)
		{
			std::string message = "LoadBMP failed on file " + file;
			gLog.LogItem( new CLogMessage(message) );
			mLoaded = false;
			return;
		}
	//}

	mWidth = temp->w;
	mHeight = temp->h;

#ifndef _PSP
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
#endif

	glGenTextures( 1, &mTextureId );
	glBindTexture( GL_TEXTURE_2D, mTextureId );

#ifndef _PSP
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
#endif

#ifndef _PSP
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, temp->w, temp->h, GL_RGB,
		GL_UNSIGNED_BYTE, temp->pixels );
#else
	glTexImage2D(GL_TEXTURE_2D, 0, 3, temp->w, temp->h, 0, GL_RGB, GL_UNSIGNED_BYTE, temp->pixels);
#endif
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAPS, GL_TRUE);

	SDL_FreeSurface( temp );
	mLoaded = true;
}

void CTexture::CreateBlank( int width, int height )
{
	char* temp = new char[width * height * 4];

	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

	glGenTextures( 1, &mTextureId );
	glBindTexture( GL_TEXTURE_2D, mTextureId );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT,
		GL_UNSIGNED_BYTE, (void*)temp );

	delete[] temp;
}

void CTexture::Unload()
{
	if ( mLoaded )
	{
		glDeleteTextures( 1, &mTextureId );
		mLoaded = false;
	}
}

void CTexture::Bind()
{
	if (mLoaded)
	{
		if (!gTexEnabled)
		{
			glEnable(GL_TEXTURE_2D);
			gTexEnabled = true;
		}
		glBindTexture( GL_TEXTURE_2D, mTextureId );
	}
}

void CTexture::Unbind()
{
	glDisable(GL_TEXTURE_2D);
	gTexEnabled = false;
}

bool CTexture::IsLoaded()
{
	return mLoaded;
}

int CTexture::Width() { return mWidth; }
int CTexture::Height() { return mHeight; }


CManagedTexture::CManagedTexture( std::string file, bool alpha )
{
	mResource = RESOURCE_TEXTURE;
	mReferences = 0;
	mLoaded = false;
	LoadResource( file, alpha );
}


CManagedTexture::~CManagedTexture()
{
	UnloadResource();
}

CTexture* CManagedTexture::Load( CResourceManager* m, std::string file)
{
	return CManagedTexture::Load(m, file, false);
}

CTexture* CManagedTexture::Load( CResourceManager* m, std::string file, bool alpha )
{
	CManagedTexture* r = (CManagedTexture*)(m->FindManagedResource( file ));
	if ( !r )
	{
		r = new CManagedTexture( file, alpha );
		m->AddResource( r );
	}

	r->IncrementReferenceCount();
	return r->GetTexture();
}

bool CManagedTexture::Unload( CResourceManager* m, CTexture* t )
{
	CLinkedList<CManagedResource>* list = m->GetList();
	CListEntry<CManagedResource>* entry = list->GetFirst();
	while ( entry )
	{
		if ( entry->data->ResourceType() == RESOURCE_TEXTURE )
		{
			if ( ((CManagedTexture*)entry->data)->GetTexture() == t )
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

CTexture* CManagedTexture::GetTexture()
{
	return &mData;
}

bool CManagedTexture::LoadResource( std::string file, bool alpha )
{
	if (alpha)
		mData.LoadAlpha(file);
	else
		mData.Load( file );
	mFile = file;
	mLoaded = mData.IsLoaded();
	return mLoaded;
}

void CManagedTexture::UnloadResource()
{
	mData.Unload();
	mLoaded = false;
}
