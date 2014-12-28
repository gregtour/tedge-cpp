/* profiling game performance
 * by subsystem module   */

#include <cstdio>
#include <cstring>
#include "profile.h"

struct timespec start_time;
struct timespec end_time;
int             frame_count;
struct timespec p_start[NUM_MODULES];
struct timespec p_finish[NUM_MODULES];
double p_elapsed[NUM_MODULES];
int    p_running[NUM_MODULES];

void ProfileInit() 
{
    for (int m = 0; m < NUM_MODULES; m++) 
    {
        memset(&p_start[m], 0, sizeof(timespec));
        memset(&p_finish[m], 0, sizeof(timespec));
        p_elapsed[m] = 0.0;
        p_running[m] = 0;
    }

    clock_gettime(CLOCK_MONOTONIC, &start_time);
    frame_count = 0;
}

void ProfileIncrementFrame()
{
    frame_count++;
}

void ProfileStartClock(int module)
{
    clock_gettime(CLOCK_MONOTONIC, &p_start[module]);
    p_running[module] = 1;
}

void ProfileStopClock(int module)
{
    clock_gettime(CLOCK_MONOTONIC, &p_finish[module]);
    p_elapsed[module] += (p_finish[module].tv_sec - p_start[module].tv_sec);
    p_elapsed[module] += (p_finish[module].tv_nsec - p_start[module].tv_nsec) 
        / 1000000000.0;
    p_running[module] = 0;
}

void ProfileResults()
{
    double total_time = 0.0;
    int m;

    clock_gettime(CLOCK_MONOTONIC, &end_time);

    for (m = 0; m < NUM_MODULES; m++) {
        total_time += p_elapsed[m];
    }

    printf("Profile Results\n");
    printf("===============\n");
    printf("MODULE\t\tTIME (s)\t\t%%\n");
    printf("physics\t\t%f\t\t%2.2f\n", 
        p_elapsed[P_PHYSICS],
        100*p_elapsed[P_PHYSICS]/total_time);
    printf("kinematics\t%f\t\t%2.2f\n", 
        p_elapsed[P_KINEMATICS],
        100*p_elapsed[P_KINEMATICS]/total_time);
    printf("collision\t%f\t\t%2.2f\n", 
        p_elapsed[P_COLLISION],
        100*p_elapsed[P_COLLISION]/total_time);
    printf("update\t\t%f\t\t%2.2f\n",
        p_elapsed[P_UPDATE],
        100*p_elapsed[P_UPDATE]/total_time);
    printf("render\t\t%f\t\t%2.2f\n",
        p_elapsed[P_RENDER],
        100*p_elapsed[P_RENDER]/total_time);
    printf("vsync\t\t%f\t\t%2.2f\n",
        p_elapsed[P_VSYNC],
        100*p_elapsed[P_VSYNC]/total_time);

    printf("\n");
    double game_time = end_time.tv_sec - start_time.tv_sec;
    game_time += (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;
    printf("Simulated %i frames in %f seconds.\n", frame_count, game_time);
    printf("%.3f frames per second.\n\n", frame_count/game_time);
}

