#include <stdlib.h>
#include <stdio.h>
#include "stdint.h"
#include <fcntl.h>
#include <devctl.h>
#include <hw/i2c.h>
#include <errno.h>
#include <unistd.h>
#include <hw/inout.h>      // for in32() and out32();
#include <sys/mman.h>      // for mmap_device_io();
#include <stdint.h>        // for unit32 types
#include <sys/neutrino.h>  // for ThreadCtl( _NTO_TCTL_IO_PRIV , NULL)
#include <sched.h>
#include <sys/procmgr.h>
#include <string.h>
#include "LCD_keypad.h"


#include <sys/dispatch.h>
#define BUF_SIZE 256
#define ATTACH_POINT_1 "/net/cycloneVg01e/dev/name/local/TLC_2" //traffic light 1
#define ATTACH_POINT_2 "/net/cycloneV_user/dev/name/local/TLC_1" //traffic light 2
#define ATTACH_POINT_3 "/net/RMIT_BBB_v5_07/dev/name/local/TC" //train controller
name_attach_t *attach;


typedef struct _pulse msg_header_t;
typedef struct _my_data {
        msg_header_t  hdr;
        int           data;
} my_data_t;
typedef struct
{
	struct _pulse hdr; // Our real data comes after this header
    char buf[BUF_SIZE];// Message we send back to clients to tell them the messages was processed correctly.
} my_reply;




const struct sigevent* Inthandler( void* area, int id )
{
	ISR_data *p_ISR_data = (ISR_data *) area;
	InterruptMask(GPIO1_IRQ, id);
	out32(p_ISR_data->gpio1_base + GPIO_IRQSTATUS_1, SD0); //clear IRQ
	p_ISR_data->count_thread++;
	InterruptUnmask(GPIO1_IRQ, id);  // Enable a hardware interrupt
	return (&p_ISR_data->pevent);
}


pthread_mutex_t mutex1;
pthread_mutex_t mutex2_i1;
pthread_mutex_t mutex3_i2;



int synchronized = 1;
char message[256] = "";//will have to make it global variable and read write lock it
int messageready = 0;
int option1 = 0;
int input_finish = 1;
int I1statereceived = 0;
char I1statereceived_string[2] = "";

int I2statereceived = 0;
char I2statereceived_string[2] = "";



void *LCD_A_options (void *data)
{
	LCD_connect *td = (LCD_connect*) data;
	uint8_t	LCDdataA[15] = {};

	while(1)
	{
		if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
		// write some Text to the LCD screen
		SetCursor(td->fd, td->Address,0,0); // set cursor on LCD to first position first line
		sprintf(LCDdataA,"1,2            ");
		I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdataA[0], sizeof(LCDdataA));		// write new data to I2C
		if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions


		while(input_finish)
		{
			usleep(100);
		}

		pthread_mutex_lock(&mutex1);
		input_finish = 1;
		pthread_mutex_unlock(&mutex1);


		if(option1 <= 2)
		{
			sprintf(message,"%d",option1);
			printf("Currently constructed message =%s\n", message);
			fflush(stdout);
			if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
			// write some Text to the LCD screen
			SetCursor(td->fd, td->Address,0,0); // set cursor on LCD to first position first line
			sprintf(LCDdataA,"N,S,E,W,D,N");
			I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdataA[0], sizeof(LCDdataA));		// write new data to I2C
			if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other function


			pthread_mutex_lock(&mutex1);
			option1 = 0;
			pthread_mutex_unlock(&mutex1);


			while(input_finish)
			{
				usleep(100);
			}
			pthread_mutex_lock(&mutex1);
			input_finish = 1;
			pthread_mutex_unlock(&mutex1);

			switch(option1)
			{
				case 1:
				strcat(message,"N");
				printf("Currently constructed message =%s\n", message);
				break;
				case 2:
				strcat(message,"S");
				printf("Currently constructed message =%s\n", message);
				break;
				case 3:
				strcat(message,"E");
				printf("Currently constructed message =%s\n", message);
				break;
				case 4:
				strcat(message,"W");
				printf("Currently constructed message =%s\n", message);
				break;
				case 5:
				strcat(message,"D");
				printf("Currently constructed message =%s\n", message);
				break;
				case 6:
				strcat(message,"N");
				printf("Currently constructed message =%s\n", message);
				break;
				default:
				break;
			}

			if(option1 <= 4) //N,S,E,W selected
			{
				if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
				// write some Text to the LCD screen
				SetCursor(td->fd, td->Address,0,0); // set cursor on LCD to first position first line
				sprintf(LCDdataA,"S,L,R,P     ");
				I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdataA[0], sizeof(LCDdataA));		// write new data to I2C
				if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other function
				fflush(stdout);


				pthread_mutex_lock(&mutex1);
				option1 = 0;
				pthread_mutex_unlock(&mutex1);


				while(input_finish)
				{
					usleep(100);
				}
				pthread_mutex_lock(&mutex1);
				input_finish = 1;
				pthread_mutex_unlock(&mutex1);


				if(option1 <= 4) //SLRP
				{
					switch(option1)
					{
						case 1:
						strcat(message, "S");
						printf("Currently constructed message =%s\n", message);
						break;
						case 2:
						strcat(message,"L");
						printf("Currently constructed message =%s\n", message);
						break;
						case 3:
						strcat(message,"R");
						printf("Currently constructed message =%s\n", message);
						break;
						case 4:
						strcat(message,"P");
						printf("Currently constructed message =%s\n", message);
						break;
						default:
						break;
					}
					if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
					// write some Text to the LCD screen
					SetCursor(td->fd, td->Address,0,0); // set cursor on LCD to first position first line
					sprintf(LCDdataA,"R,Y,G,O,A      ");
					I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdataA[0], sizeof(LCDdataA));		// write new data to I2C
					if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other function



					pthread_mutex_lock(&mutex1);
					option1 = 0;
					pthread_mutex_unlock(&mutex1);


					while(input_finish)
					{
						usleep(100);
					}
					pthread_mutex_lock(&mutex1);
					input_finish = 1;
					pthread_mutex_unlock(&mutex1);


					if(option1 <= 5) //R,Y,G,O,A
					{
						switch(option1)
						{
							case 1:
								strcat(message,"R");
								printf("Currently constructed message =%s\n", message);
								puts(message);
								break;
							case 2:
								strcat(message,"Y");
								printf("Currently constructed message =%s\n", message);
								break;
							case 3:
								strcat(message,"G");
								printf("Currently constructed message =%s\n", message);
								break;
							case 4:
								strcat(message,"O");
								printf("Currently constructed message =%s\n", message);
								break;
							case 5:
								strcat(message,"A");
								printf("Currently constructed message =%s\n", message);
								break;
							default:
								break;
						}
						if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
						// write some Text to the LCD screen
						SetCursor(td->fd, td->Address,0,0); // set cursor on LCD to first position first line
						sprintf(LCDdataA,"%s:Y,N", message);
						I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdataA[0], sizeof(LCDdataA));		// write new data to I2C
						if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other function

						pthread_mutex_lock(&mutex1);
						option1 = 0;
						pthread_mutex_unlock(&mutex1);


						while(input_finish)
						{
							usleep(100);
						}
						pthread_mutex_lock(&mutex1);
						input_finish = 1;
						pthread_mutex_unlock(&mutex1);

						if(option1 == 1)
						{
//							pthread_mutex_lock(&mutex1);
//							input_finish = 1;
//							pthread_mutex_unlock(&mutex1);
							pthread_mutex_lock(&mutex1);
							messageready = 1;
							pthread_mutex_unlock(&mutex1);
							printf("Sending Message = %s\n", message);
							fflush(stdout);
						}
						else
						{
							printf("Message canceled\n");
							fflush(stdout);
						}
					}
					else //NOT RYGOA
					{
						printf("Select RYGOA only\n");
					}
				}
				else //NOT SLRP
				{
					printf("Select SLRP only\n");
				}
			}//end of north south east west option


			else if(option1 <= 6) //night time or day time operation
			{
				if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
				// write some Text to the LCD screen
				SetCursor(td->fd, td->Address,0,0); // set cursor on LCD to first position first line
				sprintf(LCDdataA,"%s:Y,N       ", message);
				I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdataA[0], sizeof(LCDdataA));		// write new data to I2C
				if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other function

				pthread_mutex_lock(&mutex1);
				option1 = 0;
				pthread_mutex_unlock(&mutex1);


				while(input_finish)
				{
					usleep(100);
				}

				pthread_mutex_lock(&mutex1);
				input_finish = 1;
				pthread_mutex_unlock(&mutex1);

				if(option1 == 1)
				{
					pthread_mutex_lock(&mutex1);
					messageready = 1;
					pthread_mutex_unlock(&mutex1);
					printf("Message sending\n");
					fflush(stdout);
				}
				else
				{
					printf("Message canceled\n");
					fflush(stdout);
				}
			}
			else
			{
				printf("Select only N,S,E,W,D,N\n");
			}
		}
		else
		{
			printf("Select only 1,2\n");
		}
	}//end while loop

	//never reach here
	return 0;

}

void *LCD_B_I1_states (void *data)
{

	LCD_connect *td = (LCD_connect*) data;
	uint8_t	LCDdata[5] = {};
	if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
	// write some Text to the LCD screen
	SetCursor(td->fd, td->Address,0,15); // set cursor on LCD to first position first line
	sprintf(LCDdata,"I1=  ");
	I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdata[0], sizeof(LCDdata));		// write new data to I2C
	if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions
	while(1)
	{
		while(I1statereceived)
		{
			if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
			// write some Text to the LCD screen
			SetCursor(td->fd, td->Address,0,15); // set cursor on LCD to first position first line
			sprintf(LCDdata,"I1=%s",I1statereceived_string);
			I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdata[0], sizeof(LCDdata));		// write new data to I2C
			if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions
			pthread_mutex_lock(&mutex2_i1);
			I1statereceived = 0;
			pthread_mutex_unlock(&mutex2_i1);
		}
		usleep(100);
	}
	return 0;
}

void *LCD_D_I2_states (void *data)
{
	LCD_connect *td = (LCD_connect*) data;

	uint8_t	LCDdata[15] = {};
	if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
	// write some Text to the LCD screen
	SetCursor(td->fd, td->Address,1,0); // set cursor on LCD to first position first line
	sprintf(LCDdata,"Please Log In  ");
	I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdata[0], sizeof(LCDdata));		// write new data to I2C
	if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions
	LCDdata[5];

	while(1)
	{


		while(I2statereceived)
		{
			if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
			// write some Text to the LCD screen
			SetCursor(td->fd, td->Address,1,15); // set cursor on LCD to first position first line
			sprintf(LCDdata,"I2=%s",I2statereceived_string);
			I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdata[0], sizeof(LCDdata));		// write new data to I2C
			if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions
			pthread_mutex_lock(&mutex3_i2);
			I2statereceived = 0;
			pthread_mutex_unlock(&mutex3_i2);
		}

		usleep(100);
	}

	return 0;
}


void *LCD_C_keypad (void *data)
{
	uintptr_t control_module = mmap_device_io(AM335X_CONTROL_MODULE_SIZE,
	AM335X_CONTROL_MODULE_BASE);
	uintptr_t gpio1_base = mmap_device_io(AM335X_GPIO_SIZE , AM335X_GPIO1_BASE);
	if( (control_module)&&(gpio1_base) )// if mapped then
	{
	ThreadCtl( _NTO_TCTL_IO_PRIV , 1);// Request I/O privileges;
	volatile uint32_t val = 0;
	// set DDR for LEDs to output and GPIO_28 to input
	val = in32(gpio1_base + GPIO_OE); // read in current setup for GPIO1 port
	val |= 1<<28; // set IO_BIT_28 high (1=input, 0=output)
	out32(gpio1_base + GPIO_OE, val); // write value to input enable for data pins
	val &= ~(LED0|LED1|LED2|LED3); // write value to output enable
	out32(gpio1_base + GPIO_OE, val); // write value to output enable for LED pins
	in32s(&val, 1, control_module + P9_12_pinConfig );
	printf("Original pinmux configuration for GPIO1_28 = %#010x\n", val);
	// set up pin mux for the pins we are going to use (see page 1354 of TRM)
	volatile _CONF_MODULE_PIN pinConfigGPMC; // Pin configuration strut
	pinConfigGPMC.d32 = 0;
	// Pin MUX register default setup for input (GPIO input, disable pull up/down - Mode 7)
	pinConfigGPMC.b.conf_slewctrl = SLEW_SLOW; // Select between faster or slower slew rate
	pinConfigGPMC.b.conf_rxactive = RECV_ENABLE; // Input enable value for the PAD
	pinConfigGPMC.b.conf_putypesel= PU_PULL_UP; // Pad pullup/pulldown type selection
	pinConfigGPMC.b.conf_puden = PU_ENABLE; // Pad pullup/pulldown enable
	pinConfigGPMC.b.conf_mmode = PIN_MODE_7; // Pad functional signal mux select 0 - 7
	// Write to PinMux registers for the GPIO1_28
	out32(control_module + P9_12_pinConfig, pinConfigGPMC.d32);
	in32s(&val, 1, control_module + P9_12_pinConfig); // Read it back
	printf("New configuration register for GPIO1_28 = %#010x\n", val);
	int i = 0;

	//decoding
	val = in32(gpio1_base + GPIO_OE);
	val &= ~SCL; // 0 for output
	out32(gpio1_base + GPIO_OE, val); // write value to output enable for data pins


	val = in32(gpio1_base + GPIO_DATAOUT);
	val |= SCL; // Set Clock Line High as per TTP229-BSF datasheet
	out32(gpio1_base + GPIO_DATAOUT, val); // for 16-Key active-Low timing diagram


	//interupt
	// Setup IRQ for SD0 pin ( see TRM page 4871 for register list)
	out32(gpio1_base + GPIO_IRQSTATUS_SET_1, SD0);// Write 1 to GPIO_IRQSTATUS_SET_1
	out32(gpio1_base + GPIO_IRQWAKEN_1, SD0); // Write 1 to GPIO_IRQWAKEN_1
	out32(gpio1_base + GPIO_FALLINGDETECT, SD0); // set falling edge
	out32(gpio1_base + GPIO_CLEARDATAOUT, SD0); // clear GPIO_CLEARDATAOUT
	out32(gpio1_base + GPIO_IRQSTATUS_1, SD0); // clear any prior IRQs
	struct sigevent event; // fill in "event" structure
	memset(&event, 0, sizeof(event));
	event.sigev_notify = SIGEV_INTR; // Setup for external interrupt
	int id = 0; // Attach interrupt Event to IRQ for GPIO1B (upper 16 bits of port)
	id = InterruptAttachEvent (GPIO1_IRQ, &event, _NTO_INTR_FLAGS_TRK_MSK);

	LCD_connect *td = (LCD_connect*) data;
	uint8_t	LCDdata[15] = {};
	if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
	// write some Text to the LCD screen
	SetCursor(td->fd, td->Address,1,0); // set cursor on LCD to first position first line
	sprintf(LCDdata,"Logged in     ");
	I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdata[0], sizeof(LCDdata));		// write new data to I2C
	if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions



	// Main code starts here


	while(1) // dummy for loop that detects if a key press event has occurred
	{


		//interupt
		InterruptWait( 0, NULL ); // block this thread until an interrupt occurs
		InterruptDisable();
		//interupt end


		volatile uint32_t word = 0;
		// confirm that SD0 is still low (that is a valid Key press event has occurred)
		val = KeypadReadIObit(gpio1_base, SD0); // read SD0 (means data is ready)
		if(val == 0) // start reading key value form the keypad
		{
			word = 0; // clear word variable
			delaySCL(); // wait a short period of time before reading the data Tw (10 us)
			for(i=0;i<16;i++) // get data from SD0 (16 bits)
			{
				strobe_SCL(gpio1_base); // strobe the SCL line so we can read in data bit
				val = KeypadReadIObit(gpio1_base, SD0); // read in data bit
				val = ~val & 0x01; // invert bit and mask out everything but the LSB
				//printf("val[%u]=%u, ",i, val); // debug code
				word = word | (val<<i); // add data bit to word in unique position (build word up bit by bit)
			}
			//printf("word=%u\n",word); // debug code

			int keypad_value = DecodeKeyValue(word);

			//fflush(stdout);
			if(keypad_value == 13)
			{

				if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
				// write some Text to the LCD screen
				SetCursor(td->fd, td->Address,1,0); // set cursor on LCD to first position first line
				sprintf(LCDdata,"Log in again   ");
				I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdata[0], sizeof(LCDdata));		// write new data to I2C
				if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions
				return 0;
			}
			if(keypad_value == 12)
			{
				printhelp();
				if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
				// write some Text to the LCD screen
				SetCursor(td->fd, td->Address,1,0); // set cursor on LCD to first position first line
				sprintf(LCDdata,"Help on console");
				I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdata[0], sizeof(LCDdata));		// write new data to I2C
				if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions
			}
			else if(keypad_value < 16)
			{
				if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
				// write some Text to the LCD screen
				SetCursor(td->fd, td->Address,1,0); // set cursor on LCD to first position first line
				sprintf(LCDdata,"=>%d           ",keypad_value);
				I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdata[0], sizeof(LCDdata));		// write new data to I2C
				if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions

				pthread_mutex_lock(&mutex1);
				option1 = keypad_value;
				pthread_mutex_unlock(&mutex1);
			 }
			 else if(keypad_value == 16)
			 {
				if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
				// write some Text to the LCD screen
				SetCursor(td->fd, td->Address,1,0); // set cursor on LCD to first position first line
				sprintf(LCDdata,"=>             ");
				I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdata[0], sizeof(LCDdata));		// write new data to I2C
				if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions

				pthread_mutex_lock(&mutex1);
				input_finish = 0;
				pthread_mutex_unlock(&mutex1);
			 }


		}

		//interupt
		out32(gpio1_base + GPIO_IRQSTATUS_1, SD0); //clear IRQ
		InterruptUnmask(GPIO1_IRQ, id);
		InterruptEnable();
		//interupt end

	}
	// will never get here
	munmap_device_io(control_module, AM335X_CONTROL_MODULE_SIZE);
	return 0;
	}

}



void *messagesending_to_intersection(void *Not_used)
{
	my_data_t msg1;
	my_reply reply1;
	int server_coid_i1;
	char messagetosendi1[256] = "";

	if ((server_coid_i1 = name_open(ATTACH_POINT_1, 0)) == -1) //change to while later
	{
		printf("Cant attach to TLC_1 first time\n");
		sleep(1);
	} //connect to intersection 1


	msg1.hdr.type = 0x00;
	msg1.hdr.subtype = 0x00;


	my_data_t msg2;
	my_reply reply2;
	int server_coid_i2;
	char messagetosendi2[256] = "";


	if ((server_coid_i2 = name_open(ATTACH_POINT_2, 0)) == -1)
	{
		printf("Cant attach to TLC_2 first time\n");
		sleep(1);
	} //connect to intersection 2

	msg2.hdr.type = 0x00;
	msg2.hdr.subtype = 0x00;



	my_data_t msg3;
	my_reply reply3;
	int server_coid_tc;
	char messagetosendtc[256] = "";


	if ((server_coid_tc = name_open(ATTACH_POINT_3, 0)) == -1)
	{
		printf("Cant attach to TC first time\n");
		sleep(1);
	} //connect to train intersection

	msg3.hdr.type = 0x00;
	msg3.hdr.subtype = 0x00;
	char receivedfromtc[256] = ""; //should send that message to I1

	while(1)
	{
		//ping to tc start
		sprintf(messagetosendtc, "");
		if (MsgSend(server_coid_tc, &messagetosendtc, sizeof(messagetosendtc), &reply3, sizeof(reply3)) == -1)
		{
			//printf("train controller send return -1\n");
			ConnectDetach(server_coid_tc);
			if ((server_coid_tc = name_open(ATTACH_POINT_3, 0)) == -1)
			{
				printf("Cant ping train controller, try again\n");
			}
		}
		else
		{
			strcpy(receivedfromtc, reply3.buf);
			//printf("receivedfromtc ->%s\n", receivedfromtc);
			usleep(100);

		}//ping to tc done



		//ping i1
		strcpy(messagetosendi1, receivedfromtc); //instad of empty, send message received from tc
		//printf("TLC sending to i1 %s \n", messagetosendi1);
		if (MsgSend(server_coid_i1, &messagetosendi1, sizeof(messagetosendi1), &reply1, sizeof(reply1)) == -1)
		{

			pthread_mutex_lock(&mutex2_i1);
			strcpy(I1statereceived_string, "XX");
			I1statereceived = 1;
			pthread_mutex_unlock(&mutex2_i1);
			printf("Cant ping traffic intersection 1, try again\n");
			sleep(2);

			//ConnectDetach(server_coid_i1);
			printf("%d",server_coid_i1);
//			if ((server_coid_i1 = name_open(ATTACH_POINT_1, 0)) == -1)
//			{
//				printf("Cant ping traffic intersection 1, try again\n");
//				usleep(50000);
//			}

		}
		else
		{
			printf("   -->Reply is: '%s'\n", reply1.buf);

			pthread_mutex_lock(&mutex2_i1);
			strcpy(I1statereceived_string, reply1.buf);
			I1statereceived = 1;
			printf("Received from i1 %s\n", I1statereceived_string);
			pthread_mutex_unlock(&mutex2_i1);
			usleep(100);

		}//ping to i1 done



		//ping to i2 start
		strcpy(messagetosendi2, receivedfromtc); //instad of empty, send message received from tc
		//printf("Client sending %s \n", messagetosendi1);
		if (MsgSend(server_coid_i2, &messagetosendi2, sizeof(messagetosendi2), &reply2, sizeof(reply2)) == -1)
		{
			pthread_mutex_lock(&mutex3_i2);
			strcpy(I2statereceived_string, "XX");
			I2statereceived = 1;
			pthread_mutex_unlock(&mutex3_i2);
			ConnectDetach(server_coid_i2);
			printf("Cant ping traffic intersection 2, try again\n");
			usleep(100);

//			server_coid_i2 = name_open(ATTACH_POINT_2, 0);
//			if (server_coid_i2 == -1)
//			{
//				printf("Cant ping traffic intersection 2, try again\n");
//				usleep(100);
//			}
		}
		else
		{
			//printf("   -->Reply is: '%s'\n", reply.buf);

			pthread_mutex_lock(&mutex3_i2);
			strcpy(I2statereceived_string, reply2.buf);
			I2statereceived = 1;
			pthread_mutex_unlock(&mutex3_i2);
			usleep(100);
		}





		if(messageready)
		{
			if(message[0] == '1')
			{
				memcpy(messagetosendi1, &message[1], 255);
				if (MsgSend(server_coid_i1, &messagetosendi1, sizeof(messagetosendi1), &reply1, sizeof(reply1)) == -1);
				printf("Message sent to i1 %s\n", messagetosendi1);
				fflush(stdout);
				pthread_mutex_lock(&mutex1);
				messageready = 0;
				pthread_mutex_unlock(&mutex1);
				usleep(100);
			}
			else if(message[0] == '2')
			{
				memcpy(messagetosendi2, &message[1], 255);
				if (MsgSend(server_coid_i2, &messagetosendi2, sizeof(messagetosendi2), &reply2, sizeof(reply2)) == -1);
				printf("Message sent to i2 %s\n", messagetosendi2);
				fflush(stdout);
				pthread_mutex_lock(&mutex1);
				messageready = 0;
				pthread_mutex_unlock(&mutex1);
				usleep(100);
			}
		}

		//sleep(1);

	}
	return 0;
}

int main(int argc, char *argv[])
{
	pthread_mutex_init(&mutex1,NULL);
	pthread_mutex_init(&mutex2_i1,NULL);
	pthread_mutex_init(&mutex3_i2,NULL);


	int file;
	int error;
	volatile uint8_t LCDi2cAdd = 0x3C;
	_Uint32t speed = 10000; // nice and slow (will work with 200000)
	LCD_connect td;
	// Create the mutex
	pthread_mutex_init(&td.mutex,NULL);		// pass NULL as the attr parameter to use the default attributes for the mutex

	// Open I2C resource and set it up
	if ((file = open("/dev/i2c1",O_RDWR)) < 0)	  // OPEN I2C1
		printf("Error while opening Device File.!!\n");
	else
		printf("I2C1 Opened Successfully\n");

	error = devctl(file,DCMD_I2C_SET_BUS_SPEED,&(speed),sizeof(speed),NULL);  // Set Bus speed
	if (error)
	{
		fprintf(stderr, "Error setting the bus speed: %d\n",strerror ( error ));
		exit(EXIT_FAILURE);
	}
	else
		printf("Bus speed set = %d\n", speed);

	Initialise_LCD(file, LCDi2cAdd);

	usleep(1);



	// launch threads
	pthread_t  th1, th2, th4, th5, th6;
	pthread_mutex_lock(&td.mutex);		//lock the function to make sure the variables are protected
		td.fd     = file;
		td.Address= LCDi2cAdd;
	td.mode   = DATA_SEND;
    pthread_mutex_unlock(&td.mutex);	//unlock the functions to release the variables for use by other functions
    void *retval;


	pthread_create (&th1, NULL, LCD_A_options, &td);
	pthread_create (&th2, NULL, LCD_B_I1_states, &td);
	pthread_create (&th4, NULL, messagesending_to_intersection, NULL);
	pthread_create (&th6, NULL, LCD_D_I2_states, &td);
	//pthread_create (&th6, NULL, ServerReceive, &td);

	//thread th5 is the keypad which is only created after successful log in
	while(1)
	{
		int tries=0;
		int left=0;
		while(1)
		{
			char username[255] = "";
			char correctuser[255]="admin";
			printf(".......Please enter login details.......\n");
			printf("\nUser name: ");
			fflush(stdout);
			scanf("%s",&username);

			int ret = strcmp(username,correctuser);

			if(ret == 0) //wtf???????? password works but == doent work for this
			{
				break;
			}
			else
			{
				printf("Incorrect user name, please try again");
			}
		}//end username while loop
		while(1)
		{
			char passwordd[255]="";
			char correctpass[255]="password";
			printf("Please Enter Password here: ");
			fflush(stdout);
			if(tries>=10)
			{
				printf("Too many log in attempts, Portal is now locked!\n");
				//if you can use timers run a timer then exit to put in username again
				return 0;
			}
			else;

			scanf("%s",&passwordd);
			int ret=strcmp(passwordd,correctpass);
			//printf("debug: ret= %d\n",ret);
			if(ret == 0)
			{
				printf("User logged in\n");
				pthread_create (&th5, NULL, LCD_C_keypad, &td);
				break;
			}
			else
			{
				tries++;
				left=10-tries;
				printf("Password incorrect, you have %d tries left\n",left);
			}
		}//end password while loop

		pthread_join (th1, NULL); // wait for log out thread to finish

		pthread_join (th2, NULL);
		pthread_join (th4, NULL);
		pthread_join (th5, NULL);
		pthread_join (th6, NULL);

	}//end big while loop





	//Destroy the mutex
	pthread_mutex_destroy(&td.mutex);
	pthread_mutex_destroy(&mutex1);

	printf("\nMain terminating........");
	return EXIT_SUCCESS;
}

