/*
 * TL_Functions.c
 *
 *  Created on: 29Sep.,2019
 *      Author: LukeT
 */
#include "TL_I1.h"

void setState(int northStraight, int northLeft,int northRight, int northPedestrian, int southStraight, int southLeft, int southRight, int southPedestrian, int eastStraight,  int eastLeft,  int eastRight,  int eastPedestrian,int westStraight,  int westLeft,  int westRight,  int westPedestrian, void *data)
{
    struct intersection **Ptr = (struct intersection*) data;

    (*Ptr)->north.straight   = northStraight;
    (*Ptr)->north.left       = northLeft;
    (*Ptr)->north.right      = northRight;
    (*Ptr)->north.pedestrian = northPedestrian;

    (*Ptr)->south.straight   = southStraight;
    (*Ptr)->south.left       = southLeft;
    (*Ptr)->south.right      = southRight;
    (*Ptr)->south.pedestrian = southPedestrian;

    (*Ptr)->east.straight    = eastStraight;
    (*Ptr)->east.left        = eastLeft;
    (*Ptr)->east.right       = eastRight;
    (*Ptr)->east.pedestrian  = eastPedestrian;

    (*Ptr)->west.straight    = westStraight;
    (*Ptr)->west.left        = westLeft;
    (*Ptr)->west.right       = westRight;
    (*Ptr)->west.pedestrian  = westPedestrian;

    // Uncomment incase of debug
    //    printf("Function %d\n",(*Ptr)->seqState);
    //    (*Ptr)->seqState = 3;
    //    printf("Function %d\n",(*Ptr)->seqState);
};

