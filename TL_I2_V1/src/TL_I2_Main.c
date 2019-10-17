//  Traffic Light Intersection 1
//  Author1: Luke Ballantyne
//  Last Edited: 28/9/2019

#include "TL_I2_Functions.h"
#include "TL_I2_Threads.h"

int main(void) {
	//init read/write lock
	pthread_rwlock_init(&GPIO_Lock,NULL);

	//Thread actiavtion
	pthread_t gpioInputID, gpioControllerID, commandLineInputID, pulseTimerSetupID, th1; // Storage thread IDs, used for joins ect ect


	pthread_create (&pulseTimerSetupID, NULL, pulseTimerSetup, NULL);    // Uncomment to run on DE10, will terminate on VM
	pthread_create (&gpioControllerID, NULL, gpioController, NULL);    // Uncomment to run on DE10, will terminate on VM
	pthread_create (&gpioInputID, NULL, gpioInput, NULL);

    pthread_create(&th1, NULL, ServerReceive, NULL);
	pthread_create(&commandLineInputID, NULL, commandLineInputThread, NULL);

	pthread_join(commandLineInputID, NULL);
    puts("\nIntersection 1 has finished");
    return 0;
}
