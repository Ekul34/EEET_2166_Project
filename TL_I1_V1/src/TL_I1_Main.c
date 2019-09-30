//  Traffic Light Intersection 1
//  Author1: Luke Ballantyne
//  Last Edited: 28/9/2019

#include <stdio.h>
#include <stdlib.h>
#include "TL_I1.h"

int main(void) {
	puts("Intersection 1 has started");

	printf("State\tState\tnS\tnR\teS\teR\n");
    for (int i=0; i < 14; i++)      /// Test some outputs of day state machine
    {
        printf("%d\t",  intersection.seqState);
        daySequence();
        printf("%d\t",  intersection.seqState);
        printf("%d\t",  intersection.north.straight);
        printf("%d\t",  intersection.north.right);
        printf("%d\t",  intersection.east.straight);
        printf("%d\t\n",intersection.east.right);
    }

    printf("------------------------------------------\n");
    printf("State\tState\tnS\tnR\teS\teR\n");
    intersection.seqState = 0; // Change state back to 0 before moving into night sequence

    for (int i=0; i < 14; i++)      /// Test some outputs of night state machine
    {
        printf("%d\t",  intersection.seqState);
        nightSequence();
        printf("%d\t",  intersection.seqState);
        printf("%d\t",  intersection.north.straight);
        printf("%d\t",  intersection.north.right);
        printf("%d\t",  intersection.east.straight);
        printf("%d\t\n",intersection.east.right);
    }

    //gpioController(); // Uncomment to run hardware // will crash if not running on DE10


    puts("Intersection 1 has finished");
    return 0;
}

