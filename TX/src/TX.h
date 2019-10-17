#ifndef TX_H_
#define TX_H_

//string defines for input switch statement
#define B                   66
#define Q                   81
#define D                   68
#define N                   78
#define O                   79
#define A                   65
#define T                   84
#define F                   70
#define S                   83
#define E                   69
#define W                   87
#define R                   82
#define Y                   89
#define G                   71
#define L                   76
#define P                   80


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // Used for sleep()
#include <pthread.h>
#include <time.h>
#include <sys/netmgr.h>
#include <stdbool.h>
#include <string.h>
// Includes for hardware "gpioController()"
#include <hw/inout.h>      // for in32() and out32();
#include <sys/mman.h>      // for mmap_device_io();
#include <sys/neutrino.h>  // for ThreadCtl( _NTO_TCTL_IO_PRIV , NULL);
#include <stdint.h>        // for unit32 types
#include <sys/procmgr.h>
#include <sys/dispatch.h>
#include <sys/iofunc.h>

#define BUF_SIZE 256
#define ATTACH_POINT "TX" //attach point

int gpio_inputVal;
enum mode     {automatic, manual};
enum sequenceState  {bGClosed, bGOpen, error};

struct crossing
{
    bool trainLight;
    bool boomGate;
    enum sequenceState seqState;
}crossing;

pthread_rwlock_t GPIO_Lock;

struct crossingMode
{
    enum mode crossing;
}crossingMode;

typedef union
{
	struct _pulse   pulse;
} timer;

/* All of your messages should start with this header */
typedef struct _pulse msg_header_t;

#define BUF_SIZE 256
#define ATTACH_POINT "TLC"
#include <sys/neutrino.h>

/* All of your messages should start with this header */
typedef struct _pulse msg_header_t;

/* Now your real data comes after this */
typedef struct _my_data {
        msg_header_t  hdr;
        int           data;
} my_data_t;

typedef struct
{
    struct _pulse hdr; // Our real data comes after this header
    char buf[BUF_SIZE];// Message we send back to clients to tell them the messages was processed correctly.
} my_reply;


#endif /* TX_H_ */
