/*
 * TL_Functions.c
 *
 *  Created on: 29Sep.,2019
 *      Author: LukeT
 */
#include "TL_I1.h"

void setState(int northStraight, int northLeft,int northRight, int northPedestrian, int southStraight, int southLeft, int southRight, int southPedestrian, int eastStraight,  int eastLeft,  int eastRight,  int eastPedestrian,int westStraight,  int westLeft,  int westRight,  int westPedestrian)
{
    intersection.north.straight   = northStraight;
    intersection.north.left       = northLeft;
    intersection.north.right      = northRight;
    intersection.north.pedestrian = northPedestrian;

    intersection.south.straight   = southStraight;
    intersection.south.left       = southLeft;
    intersection.south.right      = southRight;
    intersection.south.pedestrian = southPedestrian;

    intersection.east.straight    = eastStraight;
    intersection.east.left        = eastLeft;
    intersection.east.right       = eastRight;
    intersection.east.pedestrian  = eastPedestrian;

    intersection.west.straight    = westStraight;
    intersection.west.left        = westLeft;
    intersection.west.right       = westRight;
    intersection.west.pedestrian  = westPedestrian;
};

