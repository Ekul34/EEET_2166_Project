//  Traffic Light Intersection 1
//  Author1: Luke Ballantyne
//  Last Edited: 28/9/2019

#include <stdio.h>
#include <stdlib.h>
#include "TL_I1.h"

int main(void) {
	puts("Intersection 1 is starting");

	printf("%d\n", intersection.north.straight);

	daySequence();


    printf("%d\n", intersection.north.straight);
    printf("%d\n", intersection.seqState);

    return 0;
}

