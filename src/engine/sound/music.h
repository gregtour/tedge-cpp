#ifndef _MUSIC_H
#define _MUSIC_H

#include "sound.h"

//#define NO_MUSIC

class CMusic
{
public:
	CMusic();
	CMusic( std::string file );
	~CMusic();

	void Load( std::string file );
	void Unload();
	void Play( int loops );

	bool IsLoaded();
	
private:
	bool mLoaded;
	Mix_Music* mMusic;
};

#endif
