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




//*************************************
//*************************************
//********** DRIVER REVISION **********
//*************************************
//*************************************
//
//V1.00
//- Original release
//
//V1.01
//- Updated to simplify I2C defines and changed to Microchip PIC18 & PIC32 example defines
//
//V1.02
//- Defines updated to generic I2C


//##### USING THE DRIVER #####
//Include this header file in any .c files within your project from which you wish to use it's functions.
/*
SIGNED_WORD temperature_x0_5c;

	//----- READ THE TEMPERATURE -----
	if (!read_temperature (&temperature_x0_5c))			//Temperature is x 0.5ºC
	{
		//Temperature read failed
	}
*/


//##### SETUP I2C PORT FOR PIC32 #####
/*
	//----- SETUP I2C 1 -----
	//Used for: temperature sensor
	OpenI2C1(I2C_EN, 23);
*/



//*****************************
//*****************************
//********** DEFINES **********
//*****************************
//*****************************
#ifndef TEMP_C_INIT		//(Do only once)
#define	TEMP_C_INIT


#define	TCN75_I2C_ADDRESS					0x90


//------ SELECT COMPILER ------
//(Enable one of these only)
#define	TCN75_USING_PIC18
//#define	TCN75_USING_PIC24
//#define	TCN75_USING_PIC32


#ifdef TCN75_USING_PIC18
//##################
//##### PIC 18 #####
//##################

#include <i2c.h>

#define	TCN75_I2C_START_I2C					StartI2C1					//Generate bus start condition
#define	TCN75_I2C_START_IN_PROGRESS_BIT		SSP1CON2bits.SEN			//Bit indicating start is still in progress
#define	TCN75_I2C_RESTART_I2C				RestartI2C1					//Generate bus restart condition
#define	TCN75_I2C_RESTART_IN_PROGRESS_BIT	SSP1CON2bits.RSEN			//Bit indicating re-start is still in progress
#define	TCN75_I2C_STOP_I2C					StopI2C1					//Generate bus stop condition
#define	TCN75_I2C_STOP_IN_PROGRESS_BIT		SSP1CON2bits.PEN			//Bit indicating Stop is still in progress
#define	TCN75_I2C_WRITE_BYTE(a)				WriteI2C1(a)				//Write byte to I2C device
#define	TCN75_I2C_TX_IN_PROGRESS_BIT		SSP1STATbits.R_W			//Bit indicating transmit byte is still in progress
#define	TCN75_I2C_ACK_NOT_RECEIVED_BIT		SSP1CON2bits.ACKSTAT		//Bit that is high when ACK was not received
//#define	TCN75_I2C_READ_BYTE_START									//Read byte from I2C device function (optional)
#define	TCN75_I2C_READ_BYTE					ReadI2C1()					//Read byte from I2C device function / result byte of TCN75_I2C_READ_FUNCTION_START
#define TCN75_I2C_ACK						AckI2C1						//Generate bus ACK condition
#define TCN75_I2C_NOT_ACK					NotAckI2C1					//Generate bus Not ACK condition
#define	TCN75_I2C_ACK_IN_PROGRESS_BIT		SSP1CON2bits.ACKEN			//Bit indicating ACK is still in progress
#define	TCN75_I2C_IDLE_I2C					IdleI2C1					//Test if I2C1 module is idle (wait until it is ready for next operation)

#endif //#ifdef TCN75_USING_PIC18


#ifdef TCN75_USING_PIC24
//##################
//##### PIC 24 #####
//##################
#define	TCN75_I2C_START_I2C					I2C2CONbits.SEN = 1										//Generate bus start condition
#define	TCN75_I2C_START_IN_PROGRESS_BIT		I2C2CONbits.SEN											//Bit indicating start is still in progress
#define	TCN75_I2C_RESTART_I2C				I2C2CONbits.RSEN = 1									//Generate bus restart condition
#define	TCN75_I2C_RESTART_IN_PROGRESS_BIT	I2C2CONbits.RSEN										//Bit indicating re-start is still in progress
#define	TCN75_I2C_STOP_I2C					I2C2CONbits.PEN = 1										//Generate bus stop condition
#define	TCN75_I2C_STOP_IN_PROGRESS_BIT		I2C2CONbits.PEN											//Bit indicating Stop is still in progress
#define	TCN75_I2C_WRITE_BYTE(a)				I2C2TRN = a												//Write byte to I2C device
#define	TCN75_I2C_TX_IN_PROGRESS_BIT		I2C2STATbits.TRSTAT										//Bit indicating transmit byte is still in progress
#define	TCN75_I2C_ACK_NOT_RECEIVED_BIT		I2C2STATbits.ACKSTAT									//Bit that is high when ACK was not received
#define	TCN75_I2C_READ_BYTE_START			I2C2CONbits.RCEN = 1; while(I2C2STATbits.RBF == 0) ;	//Read byte from I2C device function (optional)
#define	TCN75_I2C_READ_BYTE					I2C2RCV													//Read byte from I2C device function / result byte of TCN75_I2C_READ_FUNCTION_START
#define TCN75_I2C_ACK						I2C2CONbits.ACKDT = 0; I2C2CONbits.ACKEN = 1			//Generate bus ACK condition
#define TCN75_I2C_NOT_ACK					I2C2CONbits.ACKDT = 1; I2C2CONbits.ACKEN = 1			//Generate bus Not ACK condition
#define	TCN75_I2C_ACK_IN_PROGRESS_BIT		I2C2CONbits.ACKEN										//Bit indicating ACK is still in progress
#define	TCN75_I2C_IDLE_I2C					while ((I2C2CON & 0x001F) | (I2C2STATbits.R_W))			//Test if I2C1 module is idle (wait until it is ready for next operation)

#endif //#ifdef TCN75_USING_PIC24


#ifdef TCN75_USING_PIC32
//##################
//##### PIC 32 #####
//##################
#define	TCN75_I2C_START_I2C					StartI2C1					//Generate bus start condition
#define	TCN75_I2C_START_IN_PROGRESS_BIT		I2C1CONbits.SEN				//Bit indicating start is still in progress
#define	TCN75_I2C_RESTART_I2C				RestartI2C1					//Generate bus restart condition
#define	TCN75_I2C_RESTART_IN_PROGRESS_BIT	I2C1CONbits.RSEN			//Bit indicating re-start is still in progress
#define	TCN75_I2C_STOP_I2C					StopI2C1					//Generate bus stop condition
#define	TCN75_I2C_STOP_IN_PROGRESS_BIT		I2C1CONbits.PEN				//Bit indicating Stop is still in progress
#define	TCN75_I2C_WRITE_BYTE(a)				MasterWriteI2C1(a)			//Write byte to I2C device
#define	TCN75_I2C_TX_IN_PROGRESS_BIT		I2C1STATbits.TRSTAT			//Bit indicating transmit byte is still in progress
#define	TCN75_I2C_ACK_NOT_RECEIVED_BIT		I2C1STATbits.ACKSTAT		//Bit that is high when ACK was not received
//#define	TCN75_I2C_READ_BYTE_START									//Read byte from I2C device function (optional)
#define	TCN75_I2C_READ_BYTE					MasterReadI2C1()			//Read byte from I2C device function / result byte of TCN75_I2C_READ_FUNCTION_START
#define TCN75_I2C_ACK						AckI2C1						//Generate bus ACK condition
#define TCN75_I2C_NOT_ACK					NotAckI2C1					//Generate bus Not ACK condition
#define	TCN75_I2C_ACK_IN_PROGRESS_BIT		I2C1CONbits.ACKEN			//Bit indicating ACK is still in progress
#define	TCN75_I2C_IDLE_I2C					IdleI2C1					//Test if I2C1 module is idle (wait until it is ready for next operation)

#endif //#ifdef TCN75_USING_PIC32


#endif //#ifndef TEMP_C_INIT



//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef TEMP_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------



//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
BYTE read_temperature (SIGNED_WORD *temperature);


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern BYTE read_temperature (SIGNED_WORD *temperature);


#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef TEMP_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------



//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)



#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------



#endif







