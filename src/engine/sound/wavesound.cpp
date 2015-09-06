#include "wavesound.h"
#include "../common/log.h"
#include "../engine.h"
#include "../physics/svector.h"

CWaveSound::CWaveSound()
{
	mLoaded = false;
}

CWaveSound::CWaveSound( std::string file )
{
	mLoaded = false;
	Load( file );
}

CWaveSound::~CWaveSound()
{
	Unload();
}

void CWaveSound::Load( std::string file )
{
	if ( !mLoaded )
	{
#ifndef NO_SOUND
		mWave = Mix_LoadWAV( file.c_str() );
		if ( mWave )
			mLoaded = true;
		else
		{
			mLoaded = false;
			std::string message = "LoadWAV failed on file " + file;
			gLog.LogItem( new CLogMessage(message) );
		}
#else
			mLoaded = true;
#endif
	}
}


void CWaveSound::Unload()
{
	if ( mLoaded )
	{
#ifndef NO_SOUND
		Mix_FreeChunk( mWave );
#endif
		mLoaded = false;
	}
}

/* Returns the channel of the playing sound,
	-1 means failure				*/
int CWaveSound::Play( int loops )
{
#ifndef NO_SOUND
	if ( mLoaded )
	{
		int channel = Mix_PlayChannel( -1, mWave, loops );
		Mix_Volume( channel, 128 );
		return channel;
	}
	return -1;
#else
	return 0;
#endif
}

#define MIN_VOLUME	10
#define MAX_VOLUME	127

int CWaveSound::Play( int loops, SVector3 pos )
{
#ifndef NO_SOUND
	if ( mLoaded )
	{
		SVector3 dist = gCamera->GetPosition() - pos;
		SVector3 look = gCamera->GetPosition() - gCamera->GetLookAt();

		int volume = (int)(256.0f/(dist.Length() + 1.0f)*2.0f);
		dist.Normalize();
		look.Normalize();
		float sinang = (dist.CrossProduct(look)).y;

		if (volume > MAX_VOLUME)
			volume = MAX_VOLUME;

		if (volume > MIN_VOLUME)
		{
			int channel = Mix_PlayChannel( -1, mWave, loops );
			//Mix_Volume( channel, volume );
			Mix_SetPanning(channel, volume - sinang*volume, volume + sinang*volume);

			return channel;
		} else {
			return -1;
		}
	}
	return -1;
#else
	return 0;
#endif
}

bool CWaveSound::IsLoaded()
{
	return mLoaded;
}




CManagedWaveSound::CManagedWaveSound( std::string file )
{
	mResource = RESOURCE_WAVESOUND;
	mReferences = 0;
	mLoaded = false;
	LoadResource( file );
}

CManagedWaveSound::~CManagedWaveSound()
{
	UnloadResource();
}

CWaveSound* CManagedWaveSound::Load( CResourceManager* m, std::string file )
{
	CManagedWaveSound* r = (CManagedWaveSound*)(m->FindManagedResource( file ));
	if ( !r )
	{
		r = new CManagedWaveSound( file );
		m->AddResource( r );
	}

	r->IncrementReferenceCount();
	return r->GetSound();
}

bool CManagedWaveSound::Unload( CResourceManager* m, CWaveSound* ws )
{
	CLinkedList<CManagedResource>* list = m->GetList();
	CListEntry<CManagedResource>* entry = list->GetFirst();
	while ( entry )
	{
		if ( entry->data->ResourceType() == RESOURCE_WAVESOUND )
		{
			if ( ((CManagedWaveSound*)entry->data)->GetSound() == ws )
			{
				entry->data->Unload();
				return true;
			}
		}
		entry = entry->GetNext();
	}

	/*	Failed to find CWaveSound* ws in CResourceManager* m!	*/
	return false;
}


CWaveSound* CManagedWaveSound::GetSound()
{
	return &mData;
}

bool CManagedWaveSound::LoadResource( std::string file )
{
	mData.Load( file );
	mFile = file;
	mLoaded = mData.IsLoaded();
	return mLoaded;
}

void CManagedWaveSound::UnloadResource()
{
	mData.Unload();
	mLoaded = false;
}

