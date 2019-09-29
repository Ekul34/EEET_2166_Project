/*
 * TL_Threads.c
 *
 *  Created on: 29Sep.,2019
 *      Author: LukeT, TimD
 */
#include "TL_I1.h"


void extCommunication(void)
{
    // some code that talks with other nodes
};

void gpioController(void)
{
    // some code that controls the LEDs
    // some code that that reads car and pedestrian (maybe in a different thread?)
}



void daySequence(void)
{
    switch(intersection.seqState)
    {   //           North                                       South
        //           East                                        West
        //           Straight    Left    Right    Pedestrian     Straight    Left    Right    Pedestrian
        // Safe start
        case initial:
            setState(red,        red,    red,     red,           red,        red,    red,     red,
                     red,        red,    red,     red,           red,        red,    red,     red);      intersection.seqState = day1;  break;
        // NS start
        case day1:
            setState(red,        red,    green,   red,           red,        red,    green,   red,
                     red,        green,  red,     red,           red,        green,  red,     red);      intersection.seqState = day2;  break;
        case day2:
            setState(red,        red,    yellow,  red,           red,        red,    yellow,  red,
                     red,        yellow, red,     red,           red,        yellow, red,     red);      intersection.seqState = day3;  break;
        case day3:
            setState(red,        red,    off,     red,           red,        red,    off,     red,
                     red,        red,    red,     green,         red,        red,    red,     green);    intersection.seqState = day4;  break;
        case day4:
            setState(green,      green,  off,     red,           green,      green,  off,     red,
                     red,        red,    red,     green,         red,        red,    red,     green);    intersection.seqState = day5;  break;
        case day5:
            setState(yellow,     yellow, off,     red,           yellow,     yellow, off,     red,
                     red,        red,    red,     flashing,      red,        red,    red,     flashing); intersection.seqState = day6;  break;
        case day6:
            setState(red,        red,    red,     red,           red,        red,    red,     red,
                     red,        red,    red,     red,           red,        red,    red,     red);      intersection.seqState = day7;  break;
        // EW start
        case day7:
            setState(red,        green,  red,     red,           red,        green,  red,     red,
                     red,        red,    green,   red,           red,        red,    green,   red);      intersection.seqState = day8;  break;
        case day8:
            setState(red,        yellow, red,     red,           red,        yellow, red,     red,
                     red,        red,    yellow,  red,           red,        red,    yellow,  red);      intersection.seqState = day9;  break;
        case day9:
            setState(red,        red,    red,     green,         red,        red,    red,     green,
                     red,        red,    off,     red,           red,        red,    off,     red);      intersection.seqState = day10; break;
        case day10:
            setState(red,        red,    red,     green,         red,        red,    red,     green,
                     green,      green,  off,     red,           green,      green,  off,     red);      intersection.seqState = day11; break;
        case day11:
            setState(red,        red,    red,     green,         red,        red,    red,     green,
                     green,      green,  off,     red,           green,      green,  off,     red);      intersection.seqState = day12; break;
        case day12:
            setState(red,        red,    red,     red,           red,        red,    red,     red,
                     red,        red,    red,     red,           red,        red,    red,     red);      intersection.seqState = day1; break;
        default:      intersection.seqState = initial;
    }
};

void nightSequence(void)
{
    switch(intersection.seqState)
    {   //           North                                       South
        //           East                                        West
        //           Straight    Left    Right    Pedestrian     Straight    Left    Right    Pedestrian
        // Safe start
        case initial:
            setState(red,        red,    red,     red,           red,        red,    red,     red,
                     red,        red,    red,     red,           red,        red,    red,     red);      intersection.seqState = night4;  break;
        // NS start
        case night4:
            setState(green,      green,  off,     red,           green,      green,  off,     red,
                     red,        red,    red,     green,         red,        red,    red,     green);    intersection.seqState = night5;  break;
        case night5:
            setState(yellow,     yellow, off,     red,           yellow,     yellow, off,     red,
                     red,        red,    red,     flashing,      red,        red,    red,     flashing); intersection.seqState = night6;  break;
        case night6:
            setState(red,        red,    red,     red,           red,        red,    red,     red,
                     red,        red,    red,     red,           red,        red,    red,     red);      intersection.seqState = night10;  break;
        // EW start
        case night10:
            setState(red,        red,    red,     green,         red,        red,    red,     green,
                     green,      green,  off,     red,           green,      green,  off,     red);      intersection.seqState = night11; break;
        case night11:
            setState(red,        red,    red,     green,         red,        red,    red,     green,
                     green,      green,  off,     red,           green,      green,  off,     red);      intersection.seqState = night12; break;
        case night12:
            setState(red,        red,    red,     red,           red,        red,    red,     red,
                     red,        red,    red,     red,           red,        red,    red,     red);      intersection.seqState = night4; break;
        default:      intersection.seqState = initial;
    }
};

