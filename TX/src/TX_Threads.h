/*
 * TX_Threads.h
 *
 *  Created on: 17Oct.,2019
 *      Author: tsdav
 */

#ifndef TX_THREADS_H_
#define TX_THREADS_H_

#include "TX.h"

#define MY_PULSE_CODE   _PULSE_CODE_MINAVAIL

typedef struct{
	char buffer[BUF_SIZE];
} inputData;

///pulse timer setup
struct sigevent         event;
struct itimerspec       itime;
timer_t                 timer_id;
int                     chid;
int                     rcvid;
timer            		msg;

/// For Threads
void *gpioController(void);
void *daySequence(void);
void *nightSequence(void);
void *commandLineInputThread(void);

//pulse timer setup thread
void *pulseTimerSetup(void);
void *ServerReceive(void);


#endif /* TX_THREADS_H_ */
