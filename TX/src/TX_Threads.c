/*
 * TL_Threads.c
 *
 *  Created on: 29Sep.,2019
 *      Author: LukeT, TimD
 */
#include "TX_Threads.h"
#include "TX_Functions.h"

// defines for hardware "gpioController()"
#define PIO_SIZE            0x0000001F
#define PIO_SIZE_Switches   0x0000000F
#define LEDs_BASE           0xFF200000  //(LEDs - 8 bits wide - output only)
#define GPIO_1_BASE         0xFF200020  //(GPIO_1 - JP7 header - 16 bits wide - Input only)
#define GPIO_0_BASE         0xFF200040  //(GPIO_0 - JP1 header - 16 bits wide - Output only)
#define SWITCHES_BASE       0xFF200060  //(Switches - 4 bits wide - Inputs only)

// Thread IDs
pthread_t sequenceID;

void gpioSetup(void)
{
    // Maybe the setup code from the gpioController() thread below can be moved to it's own function?
}

void *gpioInput(void)
{
	printf("Initialising inputs\n");
	// GPIO pointers
    uintptr_t gpio_1_inputs = 0;
    uintptr_t Switches_inputs = 0;

    // Map inputs
    gpio_1_inputs = mmap_device_io(PIO_SIZE, GPIO_1_BASE);
    Switches_inputs = mmap_device_io(PIO_SIZE_Switches, SWITCHES_BASE);

    //Read from inputs
    //tempVal = in32(Switches_inputs); // Read from DE10 4 slide switches
    //tempVal = in32(gpio_1_inputs);   // Read from DE10 first 16 pins of GPIO_1
    printf("Initialising inputs finished\n");
    while(1){
    	usleep(50);
    	gpio_inputVal = in32(gpio_1_inputs);   // Read from DE10 first 16 pins of GPIO_1
    	//printf("GPIO input = %d",gpio_inputVal);
    }
}

void *gpioController(void) // This function will crash if not on real DE10 hardware
{
    printf("Initialising\n");
    // code that controls the LED GPIO pin based on the current values in the struct
    // code that that reads car and pedestrian (maybe in a different thread?)

    int tempVal; // temp store any values read from gpio_0 or Switches (placeholder)

    // GPIO pointers
    uintptr_t gpio_LEDs = 0;
    uintptr_t gpio_0_outputs = 0;


    // Map outputs
    gpio_LEDs = mmap_device_io(PIO_SIZE, LEDs_BASE);
    gpio_0_outputs = mmap_device_io(PIO_SIZE, GPIO_0_BASE);

    //Write to outputs
    out32(gpio_LEDs, tempVal);       // Write to DE10 LEDs
    out32(gpio_0_outputs, tempVal);  // Write to DE10 first 16 pins of GPIO_0

    // Write outputs to WS2812B LEDs on GPIO_0 // Everything here is still in testing
    out32(gpio_0_outputs, 0);
    sleep(4);

    printf("Initialisation finished\n");

    while (1){
        sleep(1);

        	if(crossing.trainLight == true)
        	{
                printf("RED ");
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);   // Green
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);

                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);   // RED
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);

                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);   // BLUE
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
        	}
        	else
        	{
        		printf("OFF ");
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);   // GREEN
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);

				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);   // RED
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);

				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);   // BLUE
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
        	}

        	if(crossing.boomGate == true)
        	{
                printf("RED ");
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);   // Green
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);

                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);   // RED
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);

                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);   // BLUE
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
                out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
        	}
        	else
        	{
        		printf("OFF ");
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);   // GREEN
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);

				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);   // RED
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);

				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);   // BLUE
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
				 out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
        	}
        printf("\n");
    }
    return 0;
};

void *sequence(void)
{
    while(1)
    {
		switch(crossing.seqState)
		{
				//setState(bool isTrainLightOn, bool isboomGateClosed);
				// Safe start
				case bGClosed:
					setState(false, true);
					while(gpio_inputVal == 65535){};
					while(gpio_inputVal == 65534){};
					crossing.seqState = bGOpen;
					break;
				// NS start
				case bGOpen:
					setState(true, false);
					while(gpio_inputVal == 65535){};
					while(gpio_inputVal == 65534){};
					crossing.seqState = bGClosed;
					break;
				/*case error:
					setState(true, true);
					crossing.seqState = bGClosed;
					break;*/

				default: crossing.seqState = bGClosed;
		}
    }
    return 0;
};

void* commandLineInputThread(void){
    char buffer[256];

    bool isError = false;

    while(true){
    	scanf("%s",buffer);//

		switch((int)buffer[0]){
			case Q:
				printf("Quitting thread\n");
				pthread_cancel(pthread_self()); //kills itself
				break;
			case S:
				printf("Starting sequence\n");
				pthread_create(&sequenceID, NULL, sequence, NULL);
				break;
			case E:
				if(!isError){
					crossingMode.crossing = manual;
					crossing.boomGate = true;
					crossing.trainLight = true;
					isError = true;
				} else {
					crossingMode.crossing = automatic;
					crossing.boomGate = false;
					crossing.trainLight = true;
					isError = false;
				}

				break;
			default:
				printf("Input not a command\n");
				break;
		}
    }
};

pthread_t commandLineInputID;

void *ServerReceive(void) {

       name_attach_t *attach;
       char ReceiveBuff[BUF_SIZE]="";
       my_data_t msg;
       int rcvid = 0;

       if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL) {
           printf("Couldnt attach to receive from client \n");
           return 0;
       }

		my_reply replymsg; 			// replymsg structure for sending back to client
		replymsg.hdr.type = 0x01;
		replymsg.hdr.subtype = 0x00;

       while (1) {
           rcvid = MsgReceive(attach->chid, &ReceiveBuff, sizeof(ReceiveBuff), NULL);

           if (rcvid == -1) {/* Error condition, exit */
               break;
           }

           if (rcvid == 0) {/* Pulse received */
               switch (msg.hdr.code) {
               case _PULSE_CODE_DISCONNECT:

                   ConnectDetach(msg.hdr.scoid);
                   break;
               case _PULSE_CODE_UNBLOCK:

                   break;
               default:
                   printf("Controller Disconnected!\n");

                   break;
               }
               continue;
           }

           if (msg.hdr.type == _IO_CONNECT ) {
               MsgReply(rcvid, crossing.seqState, NULL, 0 );
               continue;
           }

           if (msg.hdr.type > _IO_BASE && msg.hdr.type <= _IO_MAX ) {
               MsgError( rcvid, ENOSYS );
               continue;
           }
		   sprintf(replymsg.buf, "%d", crossing.seqState);

           //printf("Server received %s \n", ReceiveBuff);

           MsgReply(rcvid, EOK, &replymsg, sizeof(replymsg));

           /***COMMANDS*/
           	switch((int)ReceiveBuff[0]){
				case Q:
					printf("Quitting thread\n");
					pthread_cancel(pthread_self()); //kills itself
					break;
				case S:
					printf("Starting sequence\n");
					pthread_create(&sequenceID, NULL, sequence, NULL);
					break;
				case E:
					crossingMode.crossing = manual;
					setState(true, true);
					break;
				default:
					printf("Input not a command\n");
					break;
			}
       }

       /* Remove the name from the space */
       name_detach(attach, 0);

       return EXIT_SUCCESS;
};
