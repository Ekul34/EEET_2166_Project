/*
 * TL_Functions.c
 *
 *  Created on: 29Sep.,2019
 *      Author: LukeT
 */
#include "TL_I1.h"

void setState(int northPedestrian1, int northLeft, int northStraight, int northRight, int northPedestrian2,
              int southPedestrian1, int southLeft, int southStraight, int southRight, int southPedestrian2,
              int eastPedestrian1,  int eastLeft,  int eastStraight,  int eastRight,  int eastPedestrian2,
              int westPedestrian1,  int westLeft,  int westStraight,  int westRight,  int westPedestrian2,
              void *data)
{
    struct intersection **Ptr = (struct intersection*) data;

    (*Ptr)->north.pedestrian1 = northPedestrian1;
    (*Ptr)->north.straight   = northStraight;
    (*Ptr)->north.left       = northLeft;
    (*Ptr)->north.right      = northRight;
    (*Ptr)->north.pedestrian2 = northPedestrian2;

    (*Ptr)->south.pedestrian1 = southPedestrian1;
    (*Ptr)->south.straight   = southStraight;
    (*Ptr)->south.left       = southLeft;
    (*Ptr)->south.right      = southRight;
    (*Ptr)->south.pedestrian2 = southPedestrian2;

    (*Ptr)->east.pedestrian1  = eastPedestrian1;
    (*Ptr)->east.straight    = eastStraight;
    (*Ptr)->east.left        = eastLeft;
    (*Ptr)->east.right       = eastRight;
    (*Ptr)->east.pedestrian2  = eastPedestrian2;

    (*Ptr)->west.pedestrian1  = westPedestrian1;
    (*Ptr)->west.straight    = westStraight;
    (*Ptr)->west.left        = westLeft;
    (*Ptr)->west.right       = westRight;
    (*Ptr)->west.pedestrian2  = westPedestrian2;

    // Uncomment incase of debug
    //    printf("Function %d\n",(*Ptr)->seqState);
    //    (*Ptr)->seqState = 3;
    //    printf("Function %d\n",(*Ptr)->seqState);
};

