/*
 * TL_I1.h
 *
 *  Created on: 29Sep.,2019
 *      Author: LukeT
 */

#ifndef TL_I1_H_
#define TL_I1_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // Used for sleep()
#include <pthread.h>

enum lightState     {red, yellow, green, off, disabled, flashing};
enum sequenceState  {initial, day1, day2, day3, day4,   day5,   day6, day7, day8, day9, day10,   day11,   day12,
                                                night4, night5, night6,                 night10, night11, night12};
                          // Night sequences 4,5,6 and 10,11,12 are equal with the same day sequences numbers
struct light
{
    enum lightState straight;
    enum lightState left;
    enum lightState right;
    enum lightState pedestrian;
};

struct intersection
{
    struct light north;
    struct light south;
    struct light east;
    struct light west;
    enum sequenceState seqState;
};


/// For functions
void setState(int northPedestrian1, int northLeft, int northStraight, int northRight, int northPedestrian2,
              int southPedestrian1, int southLeft, int southStraight, int southRight, int southPedestrian2,
              int eastPedestrian2,  int eastStraight,  int eastLeft,  int eastRight,  int eastPedestrian2,
              int westStraight,  int westLeft,  int westRight,  int westPedestrian,
              void *data);

/// For Threads
void extCommunication(void);
void *gpioController(void *data);
void *daySequence(void *data);
void *nightSequence(void *data);
void *commandLineInputThread(void *data);

#endif /* TL_I1_H_ */
