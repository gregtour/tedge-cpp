#include "sound.h"

int SoundStartup()
{
	int audioRate = 22050;
	Uint16 audioFormat = AUDIO_S16SYS;
	int audioChannels = 2;
	int audioBuffers = 4096;
//	int audioBuffers = 409;
	 
	if ( Mix_OpenAudio( audioRate, audioFormat, audioChannels, audioBuffers) != 0 ) 
	{
		return 0;
	}

	return 1;
}

int SoundShutdown()
{
	Mix_CloseAudio();
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
	Mix_Volume( mChannel, value );
}

bool CSound::IsPlaying()
{
	return (mChannel != -1 && Mix_Playing( mChannel ));
}

void CSound::Stop()
{
	if ( mChannel != -1 )
		Mix_HaltChannel( mChannel );
}

