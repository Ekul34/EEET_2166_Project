
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

//#include <cstring.h>

#define AM335X_CONTROL_MODULE_BASE   (uint64_t) 0x44E10000
#define AM335X_CONTROL_MODULE_SIZE   (size_t)   0x00001448
#define AM335X_GPIO_SIZE             (uint64_t) 0x00001000
#define AM335X_GPIO1_BASE            (size_t)   0x4804C000

#define LED0          (1<<21)   // GPIO1_21
#define LED1          (1<<22)   // GPIO1_22
#define LED2          (1<<23)   // GPIO1_23
#define LED3          (1<<24)   // GPIO1_24

#define SD0 (1<<28)  // SD0 is connected to GPIO1_28
#define SCL (1<<16)  // SCL is connected to GPIO1_16


#define GPIO_OE        0x134
#define GPIO_DATAIN    0x138
#define GPIO_DATAOUT   0x13C

#define GPIO_IRQSTATUS_SET_1 0x38   // enable interrupt generation
#define GPIO_IRQWAKEN_1      0x48   // Wakeup Enable for Interrupt Line
#define GPIO_FALLINGDETECT   0x14C  // set falling edge trigger
#define GPIO_CLEARDATAOUT    0x190  // clear data out Register
#define GPIO_IRQSTATUS_1     0x30   // clear any prior IRQs

#define GPIO1_IRQ 99  // TRG page 465 list the IRQs for the am335x


#define P9_12_pinConfig 0x878 //  conf_gpmc_ben1 (TRM pp 1364) for GPIO1_28,  P9_12

// GPMC_A1_Configuration
#define PIN_MODE_0   0x00
#define PIN_MODE_1   0x01
#define PIN_MODE_2   0x02
#define PIN_MODE_3   0x03
#define PIN_MODE_4   0x04
#define PIN_MODE_5   0x05
#define PIN_MODE_6   0x06
#define PIN_MODE_7   0x07

// PIN MUX Configuration strut values  (page 1420 from TRM)
#define PU_ENABLE    0x00
#define PU_DISABLE   0x01
#define PU_PULL_UP   0x01
#define PU_PULL_DOWN 0x00
#define RECV_ENABLE  0x01
#define RECV_DISABLE 0x00
#define SLEW_FAST    0x00
#define SLEW_SLOW    0x01

typedef union _CONF_MODULE_PIN_STRUCT   // See TRM Page 1420
{
  unsigned int d32;
  struct {    // name: field size
           unsigned int conf_mmode : 3;       // LSB
           unsigned int conf_puden : 1;
           unsigned int conf_putypesel : 1;
           unsigned int conf_rxactive : 1;
           unsigned int conf_slewctrl : 1;
           unsigned int conf_res_1 : 13;      // reserved
           unsigned int conf_res_2 : 12;      // reserved MSB
         } b;
} _CONF_MODULE_PIN;

#define DATA_SEND 0x40  // sets the Rs value high
#define Co_Ctrl   0x00  // mode to tell LCD we are sending a single command


// Function prototypes
int main(int argc, char *argv[]);
int  I2cWrite_(int fd, uint8_t Address, uint8_t mode, uint8_t *pBuffer, uint32_t NbData);
void SetCursor(int fd, uint8_t LCDi2cAdd, uint8_t row, uint8_t column);
void Initialise_LCD (int fd, _Uint32t LCDi2cAdd);
void strobe_SCL(uintptr_t gpio_port_add);
void delaySCL();
uint32_t KeypadReadIObit(uintptr_t gpio_base, uint32_t BitsToRead);
int DecodeKeyValue(uint32_t word);

typedef struct
{
	int count_thread;
	uintptr_t gpio1_base;
	struct sigevent pevent; // remember to fill in "event" structure in main
}ISR_data;

// create global struct to share data between threads
ISR_data ISR_area_data;

const struct sigevent* Inthandler( void* area, int id )
{
	// 	"Do not call any functions in ISR that call kernerl - including printf()
	//struct sigevent *pevent = (struct sigevent *) area;
	ISR_data *p_ISR_data = (ISR_data *) area;

	InterruptMask(GPIO1_IRQ, id);  // Disable all hardware interrupt

	// must do this in the ISR  (else stack over flow and system will crash
	out32(p_ISR_data->gpio1_base + GPIO_IRQSTATUS_1, SD0); //clear IRQ

	// do this to tell us how many times this handler gets called
	p_ISR_data->count_thread++;
	// got IRQ.
	// work out what it came from

    InterruptUnmask(GPIO1_IRQ, id);  // Enable a hardware interrupt

    // return a pointer to an event structure (preinitialized
    // by main) that contains SIGEV_INTR as its notification type.
    // This causes the InterruptWait in "int_thread" to unblock.
	return (&p_ISR_data->pevent);
}


typedef struct
{
	int fd;
	uint8_t Address;
	uint8_t mode;
	pthread_mutex_t mutex;

} LCD_connect;

int synchronized = 1;
char message[256] = "";//will have to make it global variable and read write lock it
int messageready = 0;
int option1 = 0;
int input_finish = 1;


void *A_Options (void *data)
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

		//wait for input
		//int option1;
		//scanf("%d",&option1); //replace with scank
		//option1 = scank(gpio1_base);

		while(input_finish)
		{
			usleep(100);
		}
		input_finish = 1;

		if(option1 == 1 || option1 == 2)
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
			option1 = 0;
			while(input_finish)
			{
				usleep(100);
			}
			input_finish = 1;

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

			if(option1 == 1 || option1 == 2 || option1 == 3 || option1 == 4)
			{
				if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
				// write some Text to the LCD screen
				SetCursor(td->fd, td->Address,0,0); // set cursor on LCD to first position first line
				sprintf(LCDdataA,"S,L,R,P     ");
				I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdataA[0], sizeof(LCDdataA));		// write new data to I2C
				if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other function
				fflush(stdout);


				option1 = 0;
				while(input_finish)
				{
					usleep(100);
				}
				input_finish = 1;

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
					break;//need to change for error checking

				}
				if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
				// write some Text to the LCD screen
				SetCursor(td->fd, td->Address,0,0); // set cursor on LCD to first position first line
				sprintf(LCDdataA,"R,Y,G,O,A      ");
				I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdataA[0], sizeof(LCDdataA));		// write new data to I2C
				if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other function

				option1 = 0;
				while(input_finish)
				{
					usleep(100);
				}
				input_finish = 1;

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
						strcat(message,"F");
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


				option1 = 0;
				while(input_finish)
				{
					usleep(100);
				}
				input_finish = 1;

				if(option1 == 1)
				{
					messageready = 1;
					printf("Sending Message = %s\n", message);
					fflush(stdout);
				}
				else if(option1 == 2)
				{
					printf("Message canceled\n");
					fflush(stdout);
				}
				else
				{
					printf("Message canceled\n");
					fflush(stdout);
				}
			}//end of north south east west

			else if(option1 == 5 || option1 == 6) //night time or day time operation
			{
				if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
				// write some Text to the LCD screen
				SetCursor(td->fd, td->Address,0,0); // set cursor on LCD to first position first line
				sprintf(LCDdataA,"kdsjfnk       ");
				I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdataA[0], sizeof(LCDdataA));		// write new data to I2C
				if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other function

				option1 = 0;
				while(input_finish)
				{
					usleep(100);
				}
				input_finish = 1;

				if(option1 == 1)
				{
					int messageready = 1;
					printf("Message sent\n");
					fflush(stdout);
				}
				else
				{
					printf("Message canceled\n");
					fflush(stdout);
				}
			}
			else break;
		}
		else;
	}
	return 0;

}

void *B_I1_state_display (void *data)
{
	//use top left 5 bit to display the state

	LCD_connect *td = (LCD_connect*) data;
	uint8_t	LCDdata[5] = {};
	int i = 0;
	while(1)
	{
		//only update if state updates
		usleep(1000000); // 1 seconds

		//dummy state data from 1 - 99

		i++;
		if(i == 99)
		{
			i = 0;
		}

		if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
		// write some Text to the LCD screen
		SetCursor(td->fd, td->Address,0,15); // set cursor on LCD to first position first line
		sprintf(LCDdata,"I1=%d",i);
		I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdata[0], sizeof(LCDdata));		// write new data to I2C
		if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions


	}

	return 0;
}

void *D_I2_state_display (void *data)
{
	LCD_connect *td = (LCD_connect*) data;
	uint8_t	LCDdata[5] = {};

	int i = 99;
	while(1)
	{
		//only update if state updates
		usleep(1000000); // 1 seconds

		//dummy state data from 1 - 99


		if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
		// write some Text to the LCD screen
		SetCursor(td->fd, td->Address,1,15); // set cursor on LCD to first position first line
		sprintf(LCDdata,"I2=%d",i);
		I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdata[0], sizeof(LCDdata));		// write new data to I2C
		if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions

		i--;
		if(i == 0)
		{
			i = 99;
		}
	}
}


void *C_keypad_input (void *data)
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
		// Main code starts here


		for(;;) // dummy for loop that detects if a key press event has occurred
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
					return 0;
				}
				else if(keypad_value < 16)
				{
					LCD_connect *td = (LCD_connect*) data;
					uint8_t	LCDdata[15] = {};
					if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
					// write some Text to the LCD screen
					SetCursor(td->fd, td->Address,1,0); // set cursor on LCD to first position first line
					sprintf(LCDdata,"=>%d             ",keypad_value);
					I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdata[0], sizeof(LCDdata));		// write new data to I2C
					if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions
					option1 = keypad_value;
				 }
				 else if(keypad_value == 16)
				 {
					 input_finish = 0;
					 LCD_connect *td = (LCD_connect*) data;
					uint8_t	LCDdata[15] = {};
					if(synchronized) pthread_mutex_lock(&td->mutex);     //lock the function to make sure the variables are protected
					// write some Text to the LCD screen
					SetCursor(td->fd, td->Address,1,0); // set cursor on LCD to first position first line
					sprintf(LCDdata,"               ");
					I2cWrite_(td->fd, td->Address, DATA_SEND, &LCDdata[0], sizeof(LCDdata));		// write new data to I2C
					if(synchronized) pthread_mutex_unlock(&td->mutex);	//unlock the functions to release the variables for use by other functions
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
		}

}

int main(int argc, char *argv[])
{
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
	pthread_t  th1, th2, th4, th5;
	pthread_mutex_lock(&td.mutex);		//lock the function to make sure the variables are protected
		td.fd     = file;
		td.Address= LCDi2cAdd;
	td.mode   = DATA_SEND;
    pthread_mutex_unlock(&td.mutex);	//unlock the functions to release the variables for use by other functions

	pthread_create (&th1, NULL, A_Options, &td);
	pthread_create (&th2, NULL, B_I1_state_display, &td);
	pthread_create (&th4, NULL, D_I2_state_display, &td);
	pthread_create (&th5, NULL, C_keypad_input, &td);

	pthread_join (th5, NULL); // wait for fastest thread to finish


	//Destroy the mutex
	pthread_mutex_destroy(&td.mutex);

	printf("\nMin terminating........");
	return EXIT_SUCCESS;
}


// Writes to I2C
int  I2cWrite_(int fd, uint8_t Address, uint8_t mode, uint8_t *pBuffer, uint32_t NbData)
{
	i2c_send_t hdr;
    iov_t sv[2];
    int status, i;

    uint8_t LCDpacket[21] = {};  // limited to 21 characters  (1 control bit + 20 bytes)

    // set the mode for the write (control or data)
    LCDpacket[0] = mode;  // set the mode (data or control)

	// copy data to send to send buffer (after the mode bit)
	for (i=0;i<NbData+1;i++)
		LCDpacket[i+1] = *pBuffer++;

    hdr.slave.addr = Address;
    hdr.slave.fmt = I2C_ADDRFMT_7BIT;
    hdr.len = NbData + 1;  // 1 extra for control (mode) bit
    hdr.stop = 1;

    SETIOV(&sv[0], &hdr, sizeof(hdr));
    SETIOV(&sv[1], &LCDpacket[0], NbData + 1); // 1 extra for control (mode) bit
      // int devctlv(int filedes, int dcmd,     int sparts, int rparts, const iov_t *sv, const iov_t *rv, int *dev_info_ptr);
    status = devctlv(fd, 		  DCMD_I2C_SEND, 2,          0,          sv,              NULL,           NULL);

    if (status != EOK)
    	printf("status = %s\n", strerror ( status ));

    return status;
}


void SetCursor(int fd, uint8_t LCDi2cAdd, uint8_t row, uint8_t column)
{
	uint8_t position = 0x80; // SET_DDRAM_CMD (control bit)
	uint8_t rowValue = 0;
	uint8_t	LCDcontrol = 0;
	if (row == 1)
		rowValue = 0x40;     // memory location offset for row 1
	position = (uint8_t)(position + rowValue + column);
	LCDcontrol = position;
	I2cWrite_(fd, LCDi2cAdd, Co_Ctrl, &LCDcontrol, 1);		// write data to I2C
}


void Initialise_LCD (int fd, _Uint32t LCDi2cAdd)
{
	uint8_t	LCDcontrol = 0x00;

	//   Initialise the LCD display via the I2C bus
	LCDcontrol = 0x38;  // data byte for FUNC_SET_TBL1
	I2cWrite_(fd, LCDi2cAdd, Co_Ctrl, &LCDcontrol, 1);		// write data to I2C

	LCDcontrol = 0x39; // data byte for FUNC_SET_TBL2
	I2cWrite_(fd, LCDi2cAdd, Co_Ctrl, &LCDcontrol, 1);		// write data to I2C

	LCDcontrol = 0x14; // data byte for Internal OSC frequency
	I2cWrite_(fd, LCDi2cAdd, Co_Ctrl, &LCDcontrol, 1);		// write data to I2C

	LCDcontrol = 0x79; // data byte for contrast setting
	I2cWrite_(fd, LCDi2cAdd, Co_Ctrl, &LCDcontrol, 1);		// write data to I2C

	LCDcontrol = 0x50; // data byte for Power/ICON control Contrast set
	I2cWrite_(fd, LCDi2cAdd, Co_Ctrl, &LCDcontrol, 1);		// write data to I2C

	LCDcontrol = 0x6C; // data byte for Follower control
	I2cWrite_(fd, LCDi2cAdd, Co_Ctrl, &LCDcontrol, 1);		// write data to I2C

	LCDcontrol = 0x0C; // data byte for Display ON
	I2cWrite_(fd, LCDi2cAdd, Co_Ctrl, &LCDcontrol, 1);		// write data to I2C

	LCDcontrol = 0x01; // data byte for Clear display
	I2cWrite_(fd, LCDi2cAdd, Co_Ctrl, &LCDcontrol, 1);		// write data to I2C
}



void delaySCL()  {// Small delay used to get timing correct for BBB
  volatile int i, a;
  for(i=0;i<0x1F;i++) // 0x1F results in a delay that sets F_SCL to ~480 kHz
  {   // i*1 is faster than i+1 (i+1 results in F_SCL ~454 kHz, whereas i*1 is the same as a=i)
     a = i;
  }
  // usleep(1);  //why doesn't this work? Ans: Results in a period of 4ms as
  // fastest time, which is 250Hz (This is to slow for the TTP229 chip as it
  // requires F_SCL to be between 1 kHz and 512 kHz)
}

uint32_t KeypadReadIObit(uintptr_t gpio_base, uint32_t BitsToRead)  {
   volatile uint32_t val = 0;
   val  = in32(gpio_base + GPIO_DATAIN);// value that is currently on the GPIO port

   val &= BitsToRead; // mask bit
   //val = val >> (BitsToRead % 2);
   //return val;
   if(val==BitsToRead)
	   return 1;
   else
	   return 0;
}

int DecodeKeyValue(uint32_t word)
{
	int val;
	switch(word)
	{
	case 0x01:
		printf("Key  1 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		val = 1;
		return val;
	case 0x02:
		printf("Key  2 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 2;
	case 0x04:
		printf("Key  3 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 3;
	case 0x08:
		printf("Key  4 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 4;
	case 0x10:
		printf("Key  5 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 5;
	case 0x20:
		printf("Key  6 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 6;
	case 0x40:
		printf("Key  7 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 7;
	case 0x80:
		printf("Key  8 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 8;
	case 0x100:
		printf("Key  9 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 9;
	case 0x200:
		printf("Key 10 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 10;
	case 0x400:
		printf("Key 11 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 11;
	case 0x800:
		printf("Key 12 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 12;
	case 0x1000:
		printf("Key 13 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 13;
	case 0x2000:
		printf("Key 14 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 14;
	case 0x4000:
		printf("Key 15 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		return 15;
	case 0x8000:
		printf("Key 16 pressed\n");
		//pthread_create(NULL, NULL, Flash_LED0_ex, NULL); // flash LED
		usleep(1); // do this so we only fire once
		return 16;
	case 0x00:  // key release event (do nothing)
		//return 21;
	default:
		//printf("Key pressed could not be determined - %lu\n", word);
		return 21;

	}
}

void strobe_SCL(uintptr_t gpio_port_add) {
   uint32_t PortData;
   PortData = in32(gpio_port_add + GPIO_DATAOUT);// value that is currently on the GPIO port
   PortData &= ~(SCL);
   out32(gpio_port_add + GPIO_DATAOUT, PortData);// Clock low
   delaySCL();

   PortData  = in32(gpio_port_add + GPIO_DATAOUT);// get port value
   PortData |= SCL;// Clock high
   out32(gpio_port_add + GPIO_DATAOUT, PortData);
   delaySCL();
}



//void *Flash_LED0_ex(void *notused)
//{
//	pthread_detach(pthread_self());  // no need for this thread to join
//	uintptr_t gpio1_port = mmap_device_io(AM335X_GPIO_SIZE, AM335X_GPIO1_BASE);
//
//	uintptr_t val;
//	// Write GPIO data output register
//	val  = in32(gpio1_port + GPIO_DATAOUT);
//	val |= (LED0|LED1|LED2|LED3);
//	out32(gpio1_port + GPIO_DATAOUT, val);
//
//	usleep(100000);  // 100 ms wait
//	//sched_yield();  // if used without the usleep, this line will flash the LEDS for ~4ms
//
//	val  = in32(gpio1_port + GPIO_DATAOUT);
//	val &= ~(LED0|LED1|LED2|LED3);
//	out32(gpio1_port + GPIO_DATAOUT, val);
//
//	munmap_device_io(gpio1_port, AM335X_GPIO_SIZE);
//
//}
