/*
 * TL_I1_Functions.h
 *
 *  Created on: 17Oct.,2019
 *      Author: tsdav
 */

#ifndef TL_I1_FUNCTIONS_H_
#define TL_I1_FUNCTIONS_H_

#include "TL_I1.h"

/// For functions
void setState(int northPedestrian1, int northLeft, int northStraight, int northRight, int northPedestrian2,
              int southPedestrian1, int southLeft, int southStraight, int southRight, int southPedestrian2,
              int eastPedestrian1,  int eastLeft,  int eastStraight,  int eastRight,  int eastPedestrian2,
              int westPedestrian1,  int westLeft,  int westStraight,  int westRight,  int westPedestrian2);
void blockSouth(void);
void unblockSouth(void);
void blockNorth(void);
void unblockNorth(void);
void setLight(char buffer[256]);

#endif /* TL_I1_FUNCTIONS_H_ */
