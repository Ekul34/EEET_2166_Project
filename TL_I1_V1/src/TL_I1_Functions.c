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

	if(intersectionMode.north.pedestrian1 == automatic) intersection.north.pedestrian1	= northPedestrian1;
    if(intersectionMode.north.left == automatic) intersection.north.left     = northLeft;
    if(intersectionMode.north.straight == automatic) intersection.north.straight   = northStraight;
    if(intersectionMode.north.right == automatic) intersection.north.right      = northRight;
    if(intersectionMode.north.pedestrian2 == automatic) intersection.north.pedestrian2 = northPedestrian2;
	
    if(intersectionMode.south.pedestrian1 == automatic) intersection.south.pedestrian1 = southPedestrian1;
    if(intersectionMode.south.straight == automatic) intersection.south.straight   = southStraight;
    if(intersectionMode.south.left == automatic) intersection.south.left       = southLeft;
    if(intersectionMode.south.right == automatic) intersection.south.right      = southRight;
    if(intersectionMode.south.pedestrian2 == automatic) intersection.south.pedestrian2 = southPedestrian2;
	
    if(intersectionMode.east.pedestrian1 == automatic) intersection.east.pedestrian1  = eastPedestrian1;
    if(intersectionMode.east.straight == automatic) intersection.east.straight    = eastStraight;
    if(intersectionMode.east.left == automatic) intersection.east.left        = eastLeft;
    if(intersectionMode.east.right == automatic) intersection.east.right       = eastRight;
    if(intersectionMode.east.pedestrian2 == automatic) intersection.east.pedestrian2  = eastPedestrian2;
	
    if(intersectionMode.west.pedestrian1 == automatic) intersection.west.pedestrian1  = westPedestrian1;
    if(intersectionMode.west.straight == automatic) intersection.west.straight    = westStraight;
    if(intersectionMode.west.left == automatic) intersection.west.left        = westLeft;
    if(intersectionMode.west.right == automatic) intersection.west.right       = westRight;
    if(intersectionMode.west.pedestrian2 == automatic) intersection.west.pedestrian2  = westPedestrian2;

    // Uncomment incase of debug
    //    printf("Function %d\n",intersection.seqState);
    //    intersection.seqState = 3;
    //    printf("Function %d\n",intersection.seqState);
}

void setLight(char buffer[256]){
	int *ptr;
	ptr = &intersection.north.pedestrian1;

	int *ptrMode;
	ptrMode = &intersectionMode.north.pedestrian1;

	switch((int)buffer[0]){
		case N:
			ptr += 0;
			ptrMode += 0;
			break;
		case S:
			ptr += 5;
			ptrMode += 5;
			break;
		case E:
			ptr += 10;
			ptrMode += 10;
			break;
		case W:
			ptr += 15;
			ptrMode += 15;
			break;
		default:
			printf("direction not defined");
			break;
	}

	switch((int)buffer[1]){
		case P:
			ptr += 0;
			ptrMode += 0;
			break;
		case L:
			ptr += 1;
			ptrMode += 1;
			break;
		case S:
			ptr += 2;
			ptrMode += 2;
			break;
		case R:
			ptr += 3;
			ptrMode += 3;
			break;
		default:
			printf("light not defined");
			break;
	}

	switch((int)buffer[2]){
		case R:
			if((int)buffer[1] == P){
				*ptrMode = manual;
				*ptr = red;
				ptr += 4;

				ptrMode += 4;
				*ptrMode = manual;
				*ptr = red;
			} else {
				*ptrMode = manual;
				*ptr = red;
			}
			break;
		case Y:
			if((int)buffer[1] == P){
				*ptrMode = manual;
				*ptr = yellow;
				ptr += 4;
				ptrMode += 4;
				*ptrMode = manual;
				*ptr = yellow;
			} else {
				*ptrMode = manual;
				*ptr = yellow;
			}
			break;
		case G:
			if((int)buffer[1] == P){
				*ptrMode = manual;
				*ptr = green;
				ptr += 4;
				ptrMode += 4;
				*ptrMode = manual;
				*ptr = green;
			} else {
				*ptrMode = manual;
				*ptr = green;
			}
			break;
		case O:
			if((int)buffer[1] == P){
				*ptrMode = manual;
				*ptr = off;
				ptr += 4;
				ptrMode += 4;
				*ptrMode = manual;
				*ptr = off;
			} else {
				*ptrMode = manual;
				*ptr = off;
			}
			break;
		case A:
			if((int)buffer[1] == P){
				*ptrMode = automatic;
				ptrMode += 4;
				*ptrMode = automatic;
			} else {
				*ptrMode = automatic;
			}
			break;
		default:
			printf("value not defined");
			break;
	}
}


