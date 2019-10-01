/*
 * TL_Threads.c
 *
 *  Created on: 29Sep.,2019
 *      Author: LukeT, TimD
 */
#include "TL_I1.h"
#include <unistd.h> // Used for sleep()

// Includes for hardware "gpioController()"
#include <stdlib.h>
#include <stdio.h>
#include <hw/inout.h>      // for in32() and out32();
#include <sys/mman.h>      // for mmap_device_io();
#include <sys/neutrino.h>  // for ThreadCtl( _NTO_TCTL_IO_PRIV , NULL);
#include <stdint.h>        // for unit32 types
#include <string.h>
#include <sys/procmgr.h>

// defines for hardware "gpioController()"
#define PIO_SIZE            0x0000001F
#define PIO_SIZE_Switches   0x0000000F
#define LEDs_BASE           0xFF200000  //(LEDs - 8 bits wide - output only)
#define GPIO_1_BASE         0xFF200020  //(GPIO_1 - JP7 header - 16 bits wide - Input only)
#define GPIO_0_BASE         0xFF200040  //(GPIO_0 - JP1 header - 16 bits wide - Output only)
#define SWITCHES_BASE       0xFF200060  //(Switches - 4 bits wide - Inputs only)


void extCommunication(void)
{
    // some code that talks with other nodes
    // Send status of LEDs/State
    // receive commands to change  LED | Day | Night | train start | train finish | Boom gate fail
};

void gpioSetup(void)
{
    // Maybe the setup code from the gpioController() thread below can be moved to it's own function?
}

void gpioController(void) // This function will crash if not on real DE10 hardware
{
    // code that controls the LED GPIO pin based on the current values in the struct
    // code that that reads car and pedestrian (maybe in a different thread?)

    int tempVal; // temp store any values read from gpio_0 or Switches (placeholder)

    // GPIO global pointers
    uintptr_t gpio_LEDs = 0;
    uintptr_t gpio_0_outputs = 0;
    uintptr_t gpio_1_inputs = 0;
    uintptr_t Switches_inputs = 0;

    // Map outputs
    gpio_LEDs = mmap_device_io(PIO_SIZE, LEDs_BASE);
    gpio_0_outputs = mmap_device_io(PIO_SIZE, GPIO_0_BASE);
    // Map inputs
    gpio_1_inputs = mmap_device_io(PIO_SIZE, GPIO_1_BASE);
    Switches_inputs = mmap_device_io(PIO_SIZE_Switches, SWITCHES_BASE);

    //Read from inputs
    tempVal = in32(Switches_inputs); // Read from DE10 4 slide switches
    tempVal = in32(gpio_1_inputs);   // Read from DE10 first 16 pins of GPIO_1
    //Write to outputs
    out32(gpio_LEDs, tempVal);       // Write to DE10 LEDs
    out32(gpio_0_outputs, tempVal);  // Write to DE10 first 16 pins of GPIO_0

    // Write outputs to WS2812B LEDs on GPIO_0 // Everything here is still in testing
    out32(gpio_0_outputs, 0);
    sleep(4);

//    int i, j;
//    for(i=0;i<4;i++)
//    {
//        for(j=0;j<4;j++)
//        {
////            printf("%d\t",*prt);
////            prt++;
//        }
//        puts("\n");
//    }

    //setLED(Ptr->north.right);

//    int i = 0;
//    for(i=0;i<16;i++)
//    {
//        printf("%d\t",*prt);
//        prt++;
//    }

//    while (1){
////        out32(gpio_0_outputs, 0);
////        sleep(1);
//        for (i = 0; i < 3; i++)
//        {
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
//
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//            out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//        }
//      for (i = 0; i < 10000; i++)
//      {
//          out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//      }
//    }
};

void *daySequence(void *data)
{
    struct intersection *Ptr = (struct intersection*) data;

    switch(Ptr->seqState)
    {   //           North                                       South
        //           East                                        West
        //           Straight    Left    Right    Pedestrian     Straight    Left    Right    Pedestrian
        // Safe start
        case initial:
            setState(red,        red,    red,     red,           red,        red,    red,     red,
                     red,        red,    red,     red,           red,        red,    red,     red,   &Ptr); Ptr->seqState = day1;  break;
        // NS start
        case day1:
            setState(red,        red,    green,   red,           red,        red,    green,   red,
                     red,        green,  red,     red,           red,        green,  red,     red,   &Ptr); Ptr->seqState = day2;  break;
        case day2:
            setState(red,        red,    yellow,  red,           red,        red,    yellow,  red,
                     red,        yellow, red,     red,           red,        yellow, red,     red,   &Ptr); Ptr->seqState = day3;  break;
        case day3:
            setState(red,        red,    off,     red,           red,        red,    off,     red,
                     red,        red,    red,     green,         red,        red,    red,     green, &Ptr); Ptr->seqState = day4;  break;
        case day4:
            setState(green,      green,  off,     red,           green,      green,  off,     red,
                     red,        red,    red,     green,         red,        red,    red,     green, &Ptr); Ptr->seqState = day5;  break;
        case day5:
            setState(yellow,     yellow, off,     red,           yellow,     yellow, off,     red,
                     red,        red,    red,     flashing,      red,        red,    red,     flashing, &Ptr); Ptr->seqState = day6;  break;
        case day6:
            setState(red,        red,    red,     red,           red,        red,    red,     red,
                     red,        red,    red,     red,           red,        red,    red,     red,   &Ptr); Ptr->seqState = day7;  break;
        // EW start
        case day7:
            setState(red,        green,  red,     red,           red,        green,  red,     red,
                     red,        red,    green,   red,           red,        red,    green,   red,   &Ptr); Ptr->seqState = day8;  break;
        case day8:
            setState(red,        yellow, red,     red,           red,        yellow, red,     red,
                     red,        red,    yellow,  red,           red,        red,    yellow,  red,   &Ptr); Ptr->seqState = day9;  break;
        case day9:
            setState(red,        red,    red,     green,         red,        red,    red,     green,
                     red,        red,    off,     red,           red,        red,    off,     red,   &Ptr); Ptr->seqState = day10; break;
        case day10:
            setState(red,        red,    red,     green,         red,        red,    red,     green,
                     green,      green,  off,     red,           green,      green,  off,     red,   &Ptr); Ptr->seqState = day11; break;
        case day11:
            setState(red,        red,    red,     green,         red,        red,    red,     green,
                     green,      green,  off,     red,           green,      green,  off,     red,   &Ptr); Ptr->seqState = day12; break;
        case day12:
            setState(red,        red,    red,     red,           red,        red,    red,     red,
                     red,        red,    red,     red,           red,        red,    red,     red,   &Ptr); Ptr->seqState = day1; break;
        default: Ptr->seqState = initial;
    }

    return 0;
};

void *nightSequence(void *data)
{

    struct intersection *Ptr = (struct intersection*) data;

    switch(Ptr->seqState)
    {   //           North                                       South
        //           East                                        West
        //           Straight    Left    Right    Pedestrian     Straight    Left    Right    Pedestrian
        // Safe start
        case initial:
            setState(red,        red,    red,     red,           red,        red,    red,     red,
                     red,        red,    red,     red,           red,        red,    red,     red,   &Ptr);  Ptr->seqState = night4;  break;
        // NS start
        case night4:
            setState(green,      green,  off,     red,           green,      green,  off,     red,
                     red,        red,    red,     green,         red,        red,    red,     green, &Ptr);  Ptr->seqState = night5;  break;
        case night5:
            setState(yellow,     yellow, off,     red,           yellow,     yellow, off,     red,
                     red,        red,    red,     flashing,      red,        red,    red,     flashing, &Ptr); Ptr->seqState = night6;  break;
        case night6:
            setState(red,        red,    red,     red,           red,        red,    red,     red,
                     red,        red,    red,     red,           red,        red,    red,     red,   &Ptr);  Ptr->seqState = night10;  break;
        // EW start
        case night10:
            setState(red,        red,    red,     green,         red,        red,    red,     green,
                     green,      green,  off,     red,           green,      green,  off,     red,   &Ptr);  Ptr->seqState = night11; break;
        case night11:
            setState(red,        red,    red,     green,         red,        red,    red,     green,
                     green,      green,  off,     red,           green,      green,  off,     red,   &Ptr);  Ptr->seqState = night12; break;
        case night12:
            setState(red,        red,    red,     red,           red,        red,    red,     red,
                     red,        red,    red,     red,           red,        red,    red,     red,   &Ptr);  Ptr->seqState = night4; break;
        default: Ptr->seqState = initial;
    }
    return 0;
};

