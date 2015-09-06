#ifndef _SOUND_H
#define _SOUND_H

#include "SDL.h"
#ifndef NO_SOUND
#include "SDL_mixer.h"
#endif
#include "../common/resource.h"
#include <string>

int SoundStartup();
int SoundShutdown();

class CSound
{
public:
	CSound( int channel );
	~CSound();

	bool IsPlaying();
	void Stop();
	void SetVolume( int value );

private:
	int mChannel;
};


#endif

