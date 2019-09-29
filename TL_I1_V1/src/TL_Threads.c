/*
 * TL_Threads.c
 *
 *  Created on: 29Sep.,2019
 *      Author: LukeT
 */
#include "TL_I1.h"



void daySequence(void)
{

    switch(intersection.seqState)

    {
        case initial: setState(red,   red,   red,   red,      red,   red,   red,   red,
                               red,   red,   red,   red,      red,   red,   red,   red);
        break;
        case 1:     setState(yellow,yellow,yellow,yellow,   yellow,yellow,yellow,yellow,
                               yellow,yellow,yellow,yellow,   yellow,yellow,yellow,yellow);
        break;
        case 2:     setState(yellow,yellow,yellow,yellow,   yellow,yellow,yellow,yellow,
                               yellow,yellow,yellow,yellow,   yellow,yellow,yellow,yellow);
        break;
        default:      intersection.seqState = initial;
    }
};

void nightSequence(void)
{
    switch(intersection.seqState)
    {

        case initial:
        break;
    }

};

