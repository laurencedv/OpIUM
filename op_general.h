/*!
 @file		op_general.h
 @brief		General control of the OpIUM protocol

 @version	0.1
 @note
 @todo		

 @date		April 19th 2012
 @author	Laurence DV
*/


#ifndef _OP_GENERAL_H
#define _OP_GENERAL_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Layers
#include <op_link_rs-485.h>

// Lib
#include <soft/pic32_realtime.h>
#include <peripheral/pic32_adc.h>
#include <peripheral/pic32_spi.h>
#include <peripheral/pic32_uart.h>
#include <peripheral/pic32_timer.h>
#include <peripheral/pic32_output_compare.h>
#include <peripheral/pic32_interrupt.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //
#define OPIUM_SYSTICK_VALUE		1000			//Value of a sysTick (in µs) for the Real-Time system

// COM wings
#define COM_WING_0			0
#define COM_WING_1			1
#define COM_WING_2			2
#define COM_WING_3			3

// COM wings ID value	(ADC value >>4)
#define COM_WING_VALUE_NRF		0x05
#define COM_WING_VALUE_RS485		0x3A
#define COM_WING_VALUE_SPI		0x00
#define COM_WING_VALUE_BLUETOOTH	0x20
#define COM_WING_VALUE_ETHERNET		0x33
#define COM_WING_VALUE_LOL		0x3F

// COM wings detection
#define COM_WING_DETECT_PERIOD		100
#define COM_WING_DETECT_PERIOD_AT_RUN	1000
#define COM_WING_DETECT_RESULT_NB	10

// COM Peripherals Speed
#define COM_UART_LOW_SPEED		100000			//COM's UART Baud Rate at low speed
#define COM_UART_MEDIUM_SPEED		1000000			//COM's UART Baud Rate at medium speed
#define COM_UART_HIGH_SPEED		10000000		//COM's UART Baud Rate at high speed

#define COM_SPI_LOW_SPEED		1000000			//COM's SPI bit Rate at low speed
#define COM_SPI_MEDIUM_SPEED		10000000		//COM's SPI bit Rate at medium speed (max for nRF24L01+)
#define COM_SPI_HIGH_SPEED		20000000		//COM's SPI bit Rate at high speed (max for µOpioid)
// ############################################## //


// ################# Data Type ################## //
// COM wing state
typedef enum
{
	CWSundetected = 0,
	CWSassign,
	CWSinit,
	CWSidle,
	CWSbusy,
	CWSerror,
	CWSdisconnect
}tCOMWingState;

// COM wing type
typedef enum
{
	CWTunknown = 0,
	CWTnrf,
	CWTrs485,
	CWTspi,
	CWTbluetooth,
	CWTethernet,
	CWTlol
}tCOMWingType;

// COM wing control
typedef union
{
	U32 all[5];
	struct
	{
		tCOMWingType type;				//Type of the COM wing connected
		tCOMWingType newType;				//Type of the COM wing newly detected
		tCOMWingState state;				//General state of the COM wing
		void * controlReg;				//Control reg for the specific type of COM wing
		void * (*comWingInit)(U8 comWingID);		//Init function
		void * (*comWingDestroy)(U8 comWingID);		//Destroy function
		U8 (*comWingControl)(U8 comWingID);		//Control function
		U8 (*comWingEngine)(U8 comWingID);		//Engine function
	};
}tCOMWingControl;

// Packet Type
typedef enum
{
	OPPTnodeCommand = 0x00,
	OPPTnodeDiagnostic = 0x01,
	OPPTnodeFirmwareUpdate = 0x02,
	OPPTdata = 0x10,
	OPPTnetCommand = 0x40,
	OPPTnetDiagnostic = 0x41
}tOpPacketType;

// Packet Header structure
typedef union
{
	U32 all[5];
	struct
	{
		U8 delimiter;
		struct
		{
			U8 retryCnt:4;
			U8 :1;
			U8 ack:1;
			U8 addressSize:2;
		}status;
		U8 destination[6];
		U8 source[6];
		U16 packetID;
		struct
		{
			U8 responseFlag:1;
			tOpPacketType packetType;
		};
		struct
		{
			U8 option:6;
			U8 payloadSizeMsb:2;
		};
		U8 payloadSize;
	};
}tOpPacketHeader;
// ############################################## //


// ################# Prototypes ################# //
// ==== Buffer Functions ===== //

// =========================== //

// ==== Board Functions ====== //
/**
* \fn		void opInitStatusLed(void)
* @brief	Start the necessary pheripheral to use the Status LED on the node Board
* @note		The PWM module use the Timer 2 without interrupt
* @arg		nothing
* @return	nothing
*/
void opInitStatusLed(void);

/**
* \fn		void opSetStatusLed(U8 red, U8 green, U8 blue)
* @brief	Set the color of the Status LED
* @note		The colors value are set between 0 and 0xFF
* @arg		U8 red					Intensity value of the red
* @arg		U8 green				Intensity value of the green
* @arg		U8 blue					Intensity value of the blue
* @return	nothing
*/
void opSetStatusLed(U8 red, U8 green, U8 blue);
// =========================== //

// ==== Control Functions ==== //
/**
* \fn		U8 comWingIdentify(U8 comWingID, U16 IDData)
* @brief	Scan and detect present COM Wings
* @note
* @arg		U8 comWingID				ID of the wing targeted
* @return	U8 errorCode				STD Error Code
*/
U8 comWingIdentify(U8 comWingID, U16 IDData);

/**
* \fn		U8 comWingAssign(U8 comWingID)
* @brief	Assign the correct function according to the type of COM wing
* @note
* @arg		U8 comWingID				ID of COM wing to be assigned
* @return	U8 errorCode				STD Error Code
*/
U8 comWingAssign(U8 comWingID);

/**
* \fn		U8 comWingDetectionEngine(void)
* @brief	Engine to detect and identify COM Wings
* @note		This function must be in the infinite loop of the main
* @arg		nothing
* @return	U8 errorCode				STD Error Code
*/
U8 comWingDetectionEngine(void);

/**
* \fn		U8 comWingEngine(U8 comWingID)
* @brief	Engine to operate a COM Wing
* @note		This function must be in the infinite loop of the main
* @arg		U8 comWingID				ID of the wings selected
* @return	U8 errorCode				STD Error Code
*/
U8 comWingEngine(U8 comWingID);
// =========================== //
// ############################################## //

#endif
