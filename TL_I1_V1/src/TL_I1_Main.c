//  Traffic Light Intersection 1
//  Author1: Luke Ballantyne
//  Last Edited: 28/9/2019

#include <stdio.h>
#include <stdlib.h>
#include "TL_I1.h"
#include <pthread.h>


int main(void) {
	puts("Intersection 1 has started");

	pthread_t  daySequenceID; // Storage thread IDs, used for joins ect ect
	struct intersection intersection = {{red}}; // Create intersection struct of type intersection

	printf("Main %d\n",intersection.seqState);
    pthread_create (&daySequenceID, NULL, daySequence, &intersection);
    pthread_join(daySequenceID,NULL);
    printf("Main %d\n",intersection.seqState);

//    //gpioController(); // Uncomment to run hardware // will crash if not running on DE10

    puts("\nIntersection 1 has finished");
    return 0;
}
