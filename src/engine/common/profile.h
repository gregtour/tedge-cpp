#ifndef _PROFILE_H
#define _PROFILE_H

#include <sys/types.h>
#include <time.h>

enum {
    P_PHYSICS = 0,
    P_KINEMATICS,
    P_COLLISION,
    P_UPDATE,
    P_RENDER,
    P_VSYNC,
    NUM_MODULES
};

extern struct timespec p_start[NUM_MODULES];
extern struct timespec p_finish[NUM_MODULES];
extern int             p_running[NUM_MODULES];
extern double          p_elapsed[NUM_MODULES];

void ProfileInit();
void ProfileIncrementFrame();
void ProfileStartClock(int module);
void ProfileStopClock(int module);
void ProfileResults();

#endif // _PROFILE_H

