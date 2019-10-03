//  Traffic Light Intersection 1
//  Author1: Luke Ballantyne
//  Last Edited: 28/9/2019

#include "TL_I1.h"

int main(void) {
	puts("Intersection 1 has started");

	pthread_t daySequenceID, gpioControllerID, commandLineInputID; // Storage thread IDs, used for joins ect ect
	struct intersection intersection = {{red}}; // Create intersection struct of type intersection

	pthread_create(&commandLineInputID, NULL, commandLineInputThread, NULL);

	int i;
	for(i = 0; i < 14; i++)
	{
	    printf("State = %d\n",intersection.seqState);
        pthread_create (&gpioControllerID, NULL, gpioController, &intersection);    // Uncomment to run on DE10, will terminate on VM
        pthread_join(gpioControllerID,NULL);                                        // Uncomment to run on DE10, will terminate on VM

	    pthread_create (&daySequenceID, NULL, daySequence, &intersection);
	    pthread_join(daySequenceID,NULL);
        printf("State = %d\n",intersection.seqState);
       // sleep(1);
	}

    puts("\nIntersection 1 has finished");
    return 0;
}



