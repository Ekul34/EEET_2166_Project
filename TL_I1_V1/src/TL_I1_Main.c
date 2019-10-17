//  Traffic Light Intersection 1
//  Author1: Luke Ballantyne
//  Last Edited: 28/9/2019

#include "TL_I1_Functions.h"
#include "TL_I1_Threads.h"

int main(void) {
	//init read/write lock
	pthread_rwlock_init(&GPIO_Lock,NULL);

	//Thread actiavtion
	pthread_t gpioControllerID, commandLineInputID, pulseTimerSetupID, th1; // Storage thread IDs, used for joins ect ect

	//comms send thread init
	void *retval;
	pthread_attr_t th1_attr;
	struct sched_param th1_param;
	pthread_attr_init (&th1_attr);
	pthread_attr_setschedpolicy (&th1_attr, SCHED_RR);
	th1_param.sched_priority = 5;
	pthread_attr_setschedparam (&th1_attr, &th1_param);
	pthread_attr_setinheritsched (&th1_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setstacksize (&th1_attr, 8000);

	pthread_create (&pulseTimerSetupID, NULL, pulseTimerSetup, NULL);    // Uncomment to run on DE10, will terminate on VM
	pthread_create (&gpioControllerID, NULL, gpioController, NULL);    // Uncomment to run on DE10, will terminate on VM

    pthread_create(&th1, NULL, ServerReceive, NULL);
	//pthread_create(&commandLineInputID, NULL, commandLineInputThread, NULL);

    pthread_join(th1,&retval);

	pthread_join(commandLineInputID, NULL);
    puts("\nIntersection 1 has finished");
    return 0;
}
