/*
 * TL_Threads.c
 *
 *  Created on: 29Sep.,2019
 *      Author: LukeT, TimD
 */
#include "TL_I1_Threads.h"
#include "TL_I1_Functions.h"

// defines for hardware "gpioController()"
#define PIO_SIZE            0x0000001F
#define PIO_SIZE_Switches   0x0000000F
#define LEDs_BASE           0xFF200000  //(LEDs - 8 bits wide - output only)
#define GPIO_1_BASE         0xFF200020  //(GPIO_1 - JP7 header - 16 bits wide - Input only)
#define GPIO_0_BASE         0xFF200040  //(GPIO_0 - JP1 header - 16 bits wide - Output only)
#define SWITCHES_BASE       0xFF200060  //(Switches - 4 bits wide - Inputs only)

// Thread IDs
pthread_t daySequenceID, nightSequenceID;

void *extCommunication(void *data)
{
	inputData *td=(inputData*)data;

	char *buffer = td->buffer;

	printf("%s\n",buffer);

    // some code that talks with other nodes
    // Send status of LEDs/State
    // receive commands to change  LED | Day | Night | train start | train finish | Boom gate fail


	return NULL;
};

void *pulseTimerSetup(void){
	chid = ChannelCreate(0); // Create a communications channel

	event.sigev_notify = SIGEV_PULSE;

	// create a connection back to ourselves for the timer to send the pulse on
	event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, chid, _NTO_SIDE_CHANNEL, 0);
	if (event.sigev_coid == -1)
	{
	   printf(stderr, "couldn't ConnectAttach to self!\n");
	   perror(NULL);
	   exit(EXIT_FAILURE);
	}
	//event.sigev_priority = getprio(0);  // this function is depreciated in QNX 700
	struct sched_param th_param;
	pthread_getschedparam(pthread_self(), NULL, &th_param);
	event.sigev_priority = th_param.sched_curpriority;    // old QNX660 version getprio(0);

	event.sigev_code = MY_PULSE_CODE;

	// create the timer, binding it to the event
	if (timer_create(CLOCK_REALTIME, &event, &timer_id) == -1)
	{
	   printf (stderr, "couldn't create a timer, errno %d\n", errno);
	   perror (NULL);
	   exit (EXIT_FAILURE);
	}

	// setup the timer (1.5s initial delay value, 1.5s reload interval)
	itime.it_value.tv_sec = 2;			  // 1 second
	itime.it_value.tv_nsec = 500000000;    // 500 million nsecs = .5 secs
	itime.it_interval.tv_sec = 2;          // 1 second
	itime.it_interval.tv_nsec = 500000000; // 500 million nsecs = .5 secs

	// and start the timer!
	timer_settime(timer_id, 0, &itime, NULL);

	return NULL;
}

void gpioSetup(void)
{
    // Maybe the setup code from the gpioController() thread below can be moved to it's own function?
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
    uintptr_t gpio_1_inputs = 0;
    uintptr_t Switches_inputs = 0;

    // Map outputs
    gpio_LEDs = mmap_device_io(PIO_SIZE, LEDs_BASE);
    gpio_0_outputs = mmap_device_io(PIO_SIZE, GPIO_0_BASE);
    // Map inputs
    gpio_1_inputs = mmap_device_io(PIO_SIZE, GPIO_1_BASE);
    Switches_inputs = mmap_device_io(PIO_SIZE_Switches, SWITCHES_BASE);

    //Read from inputs
    tempVal = in32(Switches_inputs); // Read from DE10 4 slide switches
    tempVal = in32(gpio_1_inputs);   // Read from DE10 first 16 pins of GPIO_1
    //Write to outputs
    out32(gpio_LEDs, tempVal);       // Write to DE10 LEDs
    out32(gpio_0_outputs, tempVal);  // Write to DE10 first 16 pins of GPIO_0

    // Write outputs to WS2812B LEDs on GPIO_0 // Everything here is still in testing
    out32(gpio_0_outputs, 0);
    sleep(4);

    printf("Initialisation finished\n");

    int *pointer = &intersection.north.pedestrian1;
    int i;

//    for (i=0;i<1000;i++)
//    {
//        out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 2); out32(gpio_0_outputs, 0);
//    }

    while (1){
        pointer = &intersection.north.pedestrian1;
        sleep(1);
        for(i = 0; i < 5; i++)
        {
            switch(*pointer)
            {
                case 0:
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
                    break;
                case 1:
                    printf("YELLOW ");
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);  // Green  ‭10111111‬
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);

                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);  // RED
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
                    break;
                case 2:
                    printf("GREEN ");
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);   // GREEN
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);
                    out32(gpio_0_outputs, 1); out32(gpio_0_outputs, 3); out32(gpio_0_outputs, 0);

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
                    break;
                case 3:
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
                     break;

            }
            pointer++;
        }
        printf("\n");
    }
    return 0;
};

void *daySequence(void)
{
    while(1)
    {
		// wait for message/pulse
	   rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);

	   // determine who the message came from
	   if (rcvid == 0) // this process
	   {
		   // received a pulse, now check "code" field...
		   if (msg.pulse.code == MY_PULSE_CODE) // we got a pulse
           {
        //sleep(2);
				switch(intersection.seqState)
				{       //           North                                                         South
						//           East                                                          West
						//           Pedestrian1  Left     Straight     Right    Pedestrian2       Pedestrian1  Left      Straight     Right    Pedestrian2
						// Safe start
						case initial:
							setState(red,         red,     red,         red,     red,              red,         red,      red,         red,     red,
									 red,         red,     red,         red,     red,              red,         red,      red,         red,     red); intersection.seqState = day1;  break;
						// NS start
						case day1:
							setState(red,         red,     red,         green,   red,              red,         red,      red,         green,   red,
									 red,         green,   red,         red,     red,              red,         green,    red,         red,     red); intersection.seqState = day2;  break;
						case day2:
							setState(red,         red,     red,         yellow,  red,              red,         red,      red,         yellow,  red,
									 red,         yellow,  red,         red,     red,              red,         yellow,   red,         red,     red); intersection.seqState = day3;  break;
						case day3:
							setState(red,         red,     red,         off,     red,              red,         red,      red,         off,     red,
									 green,       red,     red,         red,     green,            green,       red,      red,         red,     green); intersection.seqState = day4;  break;
						case day4:
							setState(red,         green,   green,       off,     red,              red,         green,    green,       off,     red,
									 green,       red,     red,         red,     green,            green,       red,      red,         red,     green); intersection.seqState = day5;  break;
						case day5:
							setState(red,         yellow,  yellow,      off,     red,              red,         yellow,   yellow,      off,     red,
									 flashing,    red,     red,         red,     flashing,         flashing,    red,      red,         red,     flashing); intersection.seqState = day6;  break;
						case day6:
							setState(red,         red,     red,         red,     red,              red,         red,      red,         red,     red,
									 red,         red,     red,         red,     red,              red,         red,      red,         red,     red); intersection.seqState = day7;  break;
						// EW start
						case day7:
							setState(red,         green,   red,         red,     red,              red,         green,    red,         red,     red,
									 red,         red,     red,         green,   red,              red,         red,      red,         green,   red); intersection.seqState = day8;  break;
						case day8:
							setState(red,         yellow,  red,         red,     red,              red,         yellow,   red,         red,     red,
									 red,         red,     red,         yellow,  red,              red,         red,      red,         yellow,  red); intersection.seqState = day9;  break;
						case day9:
							setState(green,       red,     red,         red,     green,            green,       red,      red,         red,     green,
									 red,         red,     red,         off,     red,              red,         red,      red,         off,     red); intersection.seqState = day10; break;
						case day10:
							setState(green,       red,     red,         red,     green,            green,       red,      red,         red,     green,
									 red,         green,   green,       off,     red,              red,         green,    green,       off,     red); intersection.seqState = day11; break;
						case day11:
							setState(green,       red,     red,         red,     green,            green,       red,      red,         red,     green,
									 red,         green,   green,       off,     red,              red,         green,    green,       off,     red); intersection.seqState = day12; break;
						case day12:
							setState(red,         red,     red,         red,     red,              red,         red,      red,         red,     red,
									 red,         red,     red,         red,     red,              red,         red,      red,         red,     red); intersection.seqState = day1; break;
						default: intersection.seqState = initial;
					}
			}
	   }
    }
    return 0;
};

void *nightSequence(void)
{
    while(1)
    {
    	// wait for message/pulse
    	   rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);

    	   // determine who the message came from
    	   if (rcvid == 0) // this process
    	   {
    		   // received a pulse, now check "code" field...
    		   if (msg.pulse.code == MY_PULSE_CODE) // we got a pulse
               {
				//sleep(2);
				switch(intersection.seqState)
					{   //           North                                                 South
						//           East                                                  West
						//           Pedestrian1  Left     Straight  Right    Pedestrian2  Pedestrian1  Left     Straight   Right    Pedestrian2
						// Safe start
						case initial:
							setState(red,         red,     red,      red,     red,        red,          red,     red,       red,     red,
									 red,         red,     red,      red,     red,        red,          red,     red,       red,     red     );  intersection.seqState = night4;  break;
						// NS start
						case night4:
							setState(red,         green,   green,    off,     red,        red,          green,   green,     off,     red,
									 green,       red,     red,      red,     green,      green,        red,     red,       red,     green   );  intersection.seqState = night5;  break;
						case night5:
							setState(red,         yellow,  yellow,   off,     red,        red,          yellow,  yellow,    off,     red,
									 flashing,    red,     red,      red,     flashing,   flashing,     red,     red,       red,     flashing); intersection.seqState = night6;  break;
						case night6:
							setState(red,         red,     red,      red,     red,        red,          red,     red,       red,     red,
									 red,         red,     red,      red,     red,        red,          red,     red,       red,     red     );  intersection.seqState = night10;  break;
						// EW start
						case night10:
							setState(green,       red,     red,      red,     green,      green,        red,     red,       red,     green,
									 red,         green,   green,    off,     red,        red,          green,   green,     off,     red     );  intersection.seqState = night11; break;
						case night11:
							setState(green,       red,     red,      red,     green,      green,        red,     red,       red,     green,
									 red,         green,   green,    off,     red,        red,          green,   green,     off,     red     );  intersection.seqState = night12; break;
						case night12:
							setState(red,         red,     red,      red,     red,        red,          red,     red,       red,     red,
									 red,         red,     red,      red,     red,        red,          red,     red,       red,     red     );  intersection.seqState = night4; break;
						default: intersection.seqState = initial;
					}
               }
    	   }
    }
    return 0;
};

void* commandLineInputThread(void){
    char buffer[256];

    bool isBoomGateClosed = false;

    while(true){
    	scanf("%s",buffer);//

    	printf("%c,",buffer[0]);
		printf("%c,",buffer[1]);
		printf("%c\n",buffer[2]);

		switch((int)buffer[0]){
			case Q:
				printf("Quitting thread\n");
				pthread_cancel(pthread_self()); //kills itself
				break;
			case D:
				printf("Starting day sequence\n");
				pthread_cancel(&nightSequenceID);
				pthread_create(&daySequenceID, NULL, daySequence, NULL);
				break;
			case N:
				if((int)buffer[1] != 0){
					setLight(buffer);
				} else {
					printf("Starting night sequence\n");
					pthread_cancel(&daySequenceID);
					pthread_create(&nightSequenceID, NULL, daySequence, NULL);
				}

				break;
			case B:
				if(isBoomGateClosed){
					printf("Boom Gate Opened\n");
					unblockSouth();
				} else {
					printf("Boom Gate Closed\n");
					blockSouth();
				}
				break;
			case S:
				setLight(buffer);
				break;
			case E:
				setLight(buffer);
				break;
			case W:
				setLight(buffer);
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
               MsgReply(rcvid, intersection.seqState, NULL, 0 );
               continue;
           }

           if (msg.hdr.type > _IO_BASE && msg.hdr.type <= _IO_MAX ) {
               MsgError( rcvid, ENOSYS );
               continue;
           }
		   sprintf(replymsg.buf, "%d", intersection.seqState);

           //printf("Server received %s \n", ReceiveBuff);

           MsgReply(rcvid, EOK, &replymsg, sizeof(replymsg));

           /***COMMANDS*/
           	bool isBoomGateClosed = false;

			switch((int)ReceiveBuff[0]){
				case Q:
					printf("Quitting thread\n");
					pthread_cancel(pthread_self()); //kills itself
					break;
				case D:
					printf("Starting day sequence\n");
					pthread_cancel(&nightSequenceID);
					pthread_create(&daySequenceID, NULL, daySequence, NULL);
					break;
				case N:
					if((int)ReceiveBuff[1] != 0){
						setLight(ReceiveBuff);
					} else {
						printf("Starting night sequence\n");
						pthread_cancel(&daySequenceID);
						pthread_create(&nightSequenceID, NULL, daySequence, NULL);
					}

					break;
				case B:
					if(isBoomGateClosed){
						printf("Boom Gate Opened\n");
						unblockSouth();
					} else {
						printf("Boom Gate Closed\n");
						blockSouth();
					}
					break;
				case S:
					setLight(ReceiveBuff);
					break;
				case E:
					setLight(ReceiveBuff);
					break;
				case W:
					setLight(ReceiveBuff);
				default:
					//printf("Input not a command\n");
					break;
			}

       }

       /* Remove the name from the space */
       name_detach(attach, 0);

       return EXIT_SUCCESS;
};
