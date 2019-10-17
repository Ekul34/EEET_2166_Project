#include "TX_Functions.h"
#include "TX_Threads.h"

int main(void) {
	//init read/write lock
	pthread_rwlock_init(&GPIO_Lock,NULL);

	//Thread actiavtion
	pthread_t gpioInputID, gpioControllerID, commandLineInputID, serverReceiveID, sequenceID; // Storage thread IDs, used for joins ect ect

	pthread_create (&gpioControllerID, NULL, gpioController, NULL);    // Uncomment to run on DE10, will terminate on VM
	pthread_create (&gpioInputID, NULL, gpioInput, NULL);    // Uncomment to run on DE10, will terminate on VM

    pthread_create(&serverReceiveID, NULL, ServerReceive, NULL);
	//pthread_create(&commandLineInputID, NULL, commandLineInputThread, NULL);
	pthread_create(&sequenceID, NULL, sequence, NULL);


    pthread_join(serverReceiveID,NULL);

	//pthread_join(commandLineInputID, NULL);
    puts("\nIntersection 1 has finished");
    return 0;
}
