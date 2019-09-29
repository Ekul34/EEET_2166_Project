/*
 * TL_I1.h
 *
 *  Created on: 29Sep.,2019
 *      Author: LukeT
 */

#ifndef TL_I1_H_
#define TL_I1_H_

enum lightState     {red, yellow, green, off, disabled};
enum sequenceState  {initial, foo, bar};


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
}intersection;

void setState(int northStraight, int northLeft,int northRight, int northPedestrian, int southStraight, int southLeft, int southRight, int southPedestrian, int eastStraight,  int eastLeft,  int eastRight,  int eastPedestrian,int westStraight,  int westLeft,  int westRight,  int westPedestrian);

void daySequence(void);

void nightSequence(void);

#endif /* TL_I1_H_ */
