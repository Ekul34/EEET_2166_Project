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
              int westPedestrian1,  int westLeft,  int westStraight,  int westRight,  int westPedestrian2)
{

    intersection.north.pedestrian1 = northPedestrian1;
    intersection.north.straight   = northStraight;
    intersection.north.left       = northLeft;
    intersection.north.right      = northRight;
    intersection.north.pedestrian2 = northPedestrian2;

    intersection.south.pedestrian1 = southPedestrian1;
    intersection.south.straight   = southStraight;
    intersection.south.left       = southLeft;
    intersection.south.right      = southRight;
    intersection.south.pedestrian2 = southPedestrian2;

    intersection.east.pedestrian1  = eastPedestrian1;
    intersection.east.straight    = eastStraight;
    intersection.east.left        = eastLeft;
    intersection.east.right       = eastRight;
    intersection.east.pedestrian2  = eastPedestrian2;

    intersection.west.pedestrian1  = westPedestrian1;
    intersection.west.straight    = westStraight;
    intersection.west.left        = westLeft;
    intersection.west.right       = westRight;
    intersection.west.pedestrian2  = westPedestrian2;

    // Uncomment incase of debug
    //    printf("Function %d\n",intersection.seqState);
    //    intersection.seqState = 3;
    //    printf("Function %d\n",intersection.seqState);
};

//Converts strings into hash numbers for switch statementss
unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}
