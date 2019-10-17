/*
 * TL_I1_Threads.h
 *
 *  Created on: 17Oct.,2019
 *      Author: tsdav
 */

#ifndef TL_I1_THREADS_H_
#define TL_I1_THREADS_H_

#include "TL_I1.h"

#define MY_PULSE_CODE   _PULSE_CODE_MINAVAIL

///pulse timer setup
struct sigevent         event;
struct itimerspec       itime;
timer_t                 timer_id;
int                     chid;
int                     rcvid;
timer            		msg;

/// For Threads
void extCommunication(void);
void *gpioController(void);
void *daySequence(void);
void *nightSequence(void);
void *commandLineInputThread(void);

//pulse timer setup thread
void *pulseTimerSetup(void);
void *ServerReceive(void);

#endif /* TL_I1_THREADS_H_ */
