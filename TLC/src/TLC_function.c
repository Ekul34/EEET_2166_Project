/*
 * TLC_function.c
 *
 *  Created on: Oct 9, 2019
 *  Last edited: Oct 15, 2019
 *      Author: Nikhshay Singh Kukreja
 */


#include "LCD_keypad.h"

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
		printf("Key pressed could not be determined - %lu\n", word);
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



void printhelp()
{
	printf("\n\n\n\n..............This is a help menu................\n");
	printf("To select an option select number then press 16 to enter\n");
	printf("Keypad 13 is logout\n");
	printf("When :Y,N comes, press 1 for yes and others for no\n");
	printf("Username is admin\n");
	printf("Password is password\n");
	printf("...................end help menu................\n\n\n\n");

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
