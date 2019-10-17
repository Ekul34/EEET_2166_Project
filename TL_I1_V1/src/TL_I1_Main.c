//  Traffic Light Intersection 1
//  Author1: Luke Ballantyne
//  Last Edited: 28/9/2019

#include "TL_I1_Functions.h"
#include "TL_I1_Threads.h"

int main(void) {
	puts("Intersection 1 has started");

	//Thread actiavtion
	pthread_t gpioControllerID, commandLineInputID; // Storage thread IDs, used for joins ect ect
	pthread_create (&gpioControllerID, NULL, gpioController, NULL);    // Uncomment to run on DE10, will terminate on VM
	pthread_create(&commandLineInputID, NULL, commandLineInputThread, NULL);

	pthread_join(commandLineInputID, NULL);
    puts("\nIntersection 1 has finished");
    return 0;
}
