#include "sound.h"

int SoundStartup()
{
#ifndef NO_SOUND
	int audioRate = 22050;
	Uint16 audioFormat = AUDIO_S16SYS;
	int audioChannels = 2;
	int audioBuffers = 4096;
//	int audioBuffers = 409;
	 
	if ( Mix_OpenAudio( audioRate, audioFormat, audioChannels, audioBuffers) != 0 ) 
	{
		return 0;
	}
#endif
	return 1;
}

int SoundShutdown()
{
#ifndef NO_SOUND
	Mix_CloseAudio();
#endif
	return 1;
}

CSound::CSound( int channel )
{
	mChannel = channel;
}

CSound::~CSound()
{
}

void CSound::SetVolume( int value )
{
#ifndef NO_SOUND
	Mix_Volume( mChannel, value );
#endif
}

bool CSound::IsPlaying()
{
#ifndef NO_SOUND
	return (mChannel != -1 && Mix_Playing( mChannel ));
#else
	return (mChannel != -1);
#endif
}

void CSound::Stop()
{
#ifndef NO_SOUND
	if ( mChannel != -1 )
		Mix_HaltChannel( mChannel );
#endif
}

