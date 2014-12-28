#ifndef MP3ME_H
#define MP3ME_H

//   ** Modified sources from below author ***
//    LightMP3
//    Copyright (C) 2007 Sakya
//    sakya_tg@yahoo.it

//#include <mad.h>
#include <pspkernel.h>
#include <pspsdk.h>
#include <pspaudiocodec.h>
#include <pspaudio.h>
#include <pspaudiolib.h>
#include <math.h>
//#include "./filebrowser.h"
#include "./id3.h"
#include "./file_struct.h"

#define OPENING_OK 0
#define ERROR_OPENING -1
#define ERROR_INVALID_SAMPLE_RATE -2
#define ERROR_MEMORY -3
#define ERROR_CREATE_THREAD -4

#define STATE_MP3_NONE  0
#define STATE_MP3_PLAY  1
#define STATE_MP3_PAUSE 2
#define STATE_MP3_STOP  3

#define RAND_INT(low,high) (int)floor((low+(double)rand()*(high-low+1)/RAND_MAX))

extern int mp3_state;

extern float MP3ME_playingTime;

int start_mp3(struct FILE_INFO *file);
void set_track(int track);
void next_track(void);
void prev_track(void);

void MP3ME_Init(int channel);
int MP3ME_Play();
void MP3ME_Pause();
int MP3ME_Stop();
int MP3ME_Load(struct FILE_INFO *file);
int MP3ME_EndOfStream();
int MP3ME_GetPercentage(struct FILE_INFO *file);

#endif
