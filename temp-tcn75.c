/*
IBEX UK LTD http://www.ibexuk.com
Electronic Product Design Specialists
RELEASED SOFTWARE

The MIT License (MIT)

Copyright (c) 2013, IBEX UK Ltd, http://ibexuk.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//Project Name:		MICROCHIP TCN75 TEMPERATURE SENSOR DRIVER



#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )
#define	TEMP_C
#include "temp-tcn75.h"






//**************************************
//**************************************
//********** READ TEMPERATURE **********
//**************************************
//**************************************
//Returns 1 if temperature was read sucessfully, 0 if not
//temperature:
//	Temperature reading x0.5ºC (0x0000 = 0ºC)
BYTE read_temperature (SIGNED_WORD *temperature)
{

	//Send Start
	TCN75_I2C_IDLE_I2C();					//Wait for I2C bus to be ready
	TCN75_I2C_START_I2C();
	while(TCN75_I2C_START_IN_PROGRESS_BIT)
		;

	//Write Slave address and with RW bit 0 for write
	TCN75_I2C_IDLE_I2C();
	TCN75_I2C_WRITE_BYTE(TCN75_I2C_ADDRESS);
	while(TCN75_I2C_TX_IN_PROGRESS_BIT)
		;
	if(TCN75_I2C_ACK_NOT_RECEIVED_BIT ==1)
		goto read_temperature_fail;
	
	//Write pointer to Temperature register
	TCN75_I2C_IDLE_I2C();
	TCN75_I2C_WRITE_BYTE(0x00);
	while(TCN75_I2C_TX_IN_PROGRESS_BIT);
	if(TCN75_I2C_ACK_NOT_RECEIVED_BIT == 1)
		goto read_temperature_fail;

	//Send Restart
	TCN75_I2C_IDLE_I2C();
	TCN75_I2C_RESTART_I2C();
	while(TCN75_I2C_RESTART_IN_PROGRESS_BIT)
		;

	//Write Slave address and with RW bit 1 for read
	TCN75_I2C_IDLE_I2C();
	TCN75_I2C_WRITE_BYTE(TCN75_I2C_ADDRESS | 0x01);
	while(TCN75_I2C_TX_IN_PROGRESS_BIT)
		;
	if(TCN75_I2C_ACK_NOT_RECEIVED_BIT == 1)
		goto read_temperature_fail;

	//Get byte 0
	TCN75_I2C_IDLE_I2C();
	#ifdef TCN75_I2C_READ_BYTE_START
		TCN75_I2C_READ_BYTE_START
	#endif
	*temperature = ((SIGNED_WORD)TCN75_I2C_READ_BYTE << 1);
	if (*temperature & 0x0100)				//If value is negative then set all the high bits
		*temperature |= 0xfe00

	//Send Ack
	TCN75_I2C_IDLE_I2C();
	TCN75_I2C_ACK();
	while(TCN75_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Get byte 1 (Least significant bit is in bit 7, rest are unused)
	TCN75_I2C_IDLE_I2C();
	#ifdef TCN75_I2C_READ_BYTE_START
		TCN75_I2C_READ_BYTE_START
	#endif
	if (TCN75_I2C_READ_BYTE & 0x80)
		*temperature |= 1;

	//Send NAK
	TCN75_I2C_IDLE_I2C();
	TCN75_I2C_NOT_ACK();
	while(TCN75_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Send Stop
	TCN75_I2C_IDLE_I2C();
	TCN75_I2C_STOP_I2C();
	while(TCN75_I2C_STOP_IN_PROGRESS_BIT)
		;

	
	return(1);



read_temperature_fail:
	//Send Stop
	TCN75_I2C_IDLE_I2C();
	TCN75_I2C_STOP_I2C();
	while(TCN75_I2C_STOP_IN_PROGRESS_BIT)
		;

	*temperature = 0;

	return(0);
}













