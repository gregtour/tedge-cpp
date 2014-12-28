#include "music.h"
#include "../common/log.h"
#include "../engine.h"

#ifdef _PSP
// SFX
/*
extern "C"
{
	#include "mp3/mp3playerME.h"
}*/
#endif


CMusic::CMusic()
{
	mLoaded = false;
}

CMusic::CMusic( std::string file )
{
	mLoaded = false;
	Load( file );
}

CMusic::~CMusic()
{
	Unload();
}

void CMusic::Load( std::string file )
{
	if ( !mLoaded )
	{
#ifdef _PSP
/*
	FILE_INFO fhandle;
	sprintf( fhandle.filePath, file.c_str() );
	MP3ME_Init( 1 );
	MP3ME_Load( &fhandle );
	mLoaded = true;
*/
#else
		mMusic = Mix_LoadMUS( file.c_str() );
		if ( mMusic )
			mLoaded = true;
		else
		{
			mLoaded = false;
			std::string message = "LoadMUS failed on file " + file;
			gLog.LogItem( new CLogMessage(message) );
		}
#endif
	}
}


void CMusic::Unload()
{
	if ( mLoaded )
	{
#ifndef _PSP
		Mix_FreeMusic( mMusic );
#endif
		mLoaded = false;
	}
}

/* Returns the channel of the playing sound,
	-1 means failure				*/
void CMusic::Play( int loops )
{
#ifndef NO_MUSIC
	if ( mLoaded )
	{
#ifdef _PSP
//		MP3ME_Play( );
#else
		Mix_PlayMusic( mMusic, loops );
#endif
	}
#endif
}

bool CMusic::IsLoaded()
{
	return mLoaded;
}
