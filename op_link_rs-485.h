/*!
 @file		op_link_rs-485.h
 @brief		

 @version	0.1
 @note
 @todo

 @date		August 14th 2012
 @author	Laurence DV
*/

#ifndef _OP_LINK_RS_485_H
#define _OP_LINK_RS_485_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Lib
#include <peripheral/pic32_uart.h>
#include <peripheral/pic32_timer.h>
#include <op_general.h>
#include <stdlib.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/bitmanip_megaxone.h>
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// Data Direction
#define OP_RS485_DIR_TX			1
#define OP_RS485_DIR_RX			0

// Status LED
#define OP_RS485_LED_STAT_OFF		0
#define OP_RS485_LED_STAT_ON		0x1
#define OP_RS485_LED_STAT_BLINK		0x2
#define OP_RS485_LED_STAT_BLINK_SLOW	0x3
#define OP_RS485_LED_STAT_BLINK_FAST	0x4

// RS-485 Reserved Address
#define OP_RS485_ADD_BROADCAST		0
#define OP_RS485_ADD_MASTER		1

// Timing
#define OP_RS485_TIME_WAIT_INIT		300000			//Time to wait a sync slot at init

// Packet
#define OP_RS485_PKT_DELIMITER		0xEE			//Value of the packet delimiter
#define OP_RS485_PKT_NO_HDR		0
#define OP_RS485_PKT_HDR_FOUND		1
#define OP_RS485_PKT_HDR_COMPLETE	2
#define OP_RS485_PKT_COMPLETE		3
// ############################################## //


// ################# Data Type ################## //
// -- RS-485 Packet -- //
// Packet Command
typedef enum
{
	RScmdSync = 0,
	RScmdAskSlot,
	RScmdGiveSlot,
	RScmdRemSlot,
	RScmdData
}tOpRS485PacketCommand;

// Packet Header
typedef union
{
	U8 all[4];
	struct
	{
		U8 delimiter;
		U8 byteNb;
		U8 destination;
		tOpRS485PacketCommand command;
	};
}tOpRS485PacketHeader;

// Packet
typedef struct
{
	
	tOpRS485PacketHeader header;
	U8 * dataPtr;
	U8 dataSize;
}tOpRS485Packet;

// Packet Reception Control
typedef union
{
	U32 all;
	struct
	{
		U8 packetState;					//State of the reassembly of the packet
		U8 byteTotal;					//Number of byte expected
		U8 byteReceived;				//Number of byte received until now
		U8 :8;
	};
}tOpRS485PacketControl;
// ------------------- //

// Link State
typedef enum
{
	RSSdetect = 0,
	RSSelection,
	RSSmaster,
	RSSslave
}tOpRS485LinkState;

// Link SubState
typedef enum
{
	RSSinit = 0,
	RSSidle,
	RSSactive
}tOpRS485LinkSubState;

// Slot control
typedef union
{
	U16 all;
	struct
	{
		U8 frame;
		U8 slot;
	};
}tOpRS485Slot;

// Control
typedef union
{
	U32 all[6];
	struct
	{
		tOpRS485LinkState linkState;			//General State of the RS-485 Link
		tOpRS485LinkSubState linkSubState;		//Sub State of the RS-485 Link
		U8 timerID;					//Hardware Timer port assigned to this RS-485 wing
		U8 uartID;					//Hardware Uart port assigned to this RS-485 wing
		U8 utilityCnt;					//Utility counter value
		U8 slotNb;					//Number of time slot assigned to this OpIUM node
		tOpRS485Slot * slotControl;			//Control reg for the different slot assigned to this node
		tOpRS485PacketControl packetControl;		//Control reg for the current transiting packet
		U8 currentFrame;				//Current RS-485 time frame
		U8 currentSlot;					//Current RS-485 time slot
		U8 rsAddress;					//RS-485 bus address assigned to this RS-485 wing
		U8 statusLedState;				//Current state of the RS-485 Status LED
		U8 statusLedSoftCntID;				//Soft counter ID assigned to the RS-485 Status LED
		U8 terminatorState;				//Current state of the RS-485 Bus terminator
		U8 dataDirection;				//Current data direction of the RS-485 transceiver
		U8 comWingID;					//COM Wing ID of this RS-485 wing
	};
}tOpRS485Control;
// ############################################## //


// ################# Prototypes ################# //
// ==== Interrupt Functions ==== //
/**
* \fn		void opRS485TimerISR(void * controlReg)
* @brief	Timer ISR for the RS-485 COM Wing
* @note
* @arg		void * controlReg				Pointer to the control Reg
* @return	nothing
*/
void opRS485TimerISR(void * controlReg);

/**
* \fn		void opRS485UartISR(void * controlReg)
* @brief	UART ISR for the RS-485 COM Wing
* @note
* @arg		void * controlReg				Pointer to the control Reg
* @return	nothing
*/
void opRS485UartISR(void * controlReg);
// ============================= //


// ==== Control Functions ==== //
/**
* \fn		void * opRS485Create(U8 comWingID)
* @brief	Create the control reg for a RS-485 Wing and initialise the peripheral
* @note
* @arg		U8 comWingID					ID of the initialising COM Wing
* @return	void * tempOpRS485ControlReg			Pointer to the control Reg
*/
void * opRS485Create(U8 comWingID);

/**
* \fn		U8 opRS485Destroy(tOpRS485Control * controlToDestroy)
* @brief	Deallocated and stop everything related to a RS-485 COM Wing
* @note		This can fail! Try-and-Pray!
* @arg		void * controlReg				Pointer to the control Reg
* @return	nothing
*/
void opRS485Destroy(void * controlReg);

/**
* \fn		U8 opRS485Control (void * controlReg)
* @brief
* @note
* @arg		U8void * controlReg				Pointer to the control Reg
* @return	U8 errorCode					STD Error Code
*/
U8 opRS485Engine(void * controlReg);

/**
* \fn		void opRS485SetTerm(void * controlReg, U8 termState)
* @brief	Set the state of the RS-485 120? terminator
* @note
* @arg		Uvoid * controlReg				Pointer to the control Reg
* @arg		U8 termState					State of the terminator
* @return	nothing
*/
void opRS485SetTerm(void * controlReg, U8 termState);

/**
* \fn		U8 opRS485GetTerm(void * controlReg)
* @brief	Return the actual state of the RS-485 120? terminator
* @note
* @arg		void * controlReg				Pointer to the control Reg
* @return	U8 termState					State of the terminator
*/
U8 opRS485GetTerm(void * controlReg);

/**
* \fn		void opRS485SetStatusLed(void * controlReg, U8 ledState)
* @brief	Set the state of the Status LED
* @note
* @arg		void * controlReg				Pointer to the control Reg
* @arg		U8 ledState					State of the LED
* @return	nothing
*/
void opRS485SetStatusLed(void * controlReg, U8 ledState);

/**
* \fn		U8 opRS485GetStatusLed(void * controlReg)
* @brief	Return the actual state of the Status LED
* @note
* @arg		void * controlReg				Pointer to the control Reg
* @return	U8 ledState					State of the LED
*/
U8 opRS485GetStatusLed(void * controlReg);

tOpRS485Packet * opRS485CreatePacket(U8 dataSize);

void opRS485DestroyPacket(tOpRS485Packet * packetPtr);
// =========================== //


// ==== Data Functions ==== //
/**
* \fn		U8 opRS485Parse(void * controlReg);
* @brief
* @note
* @arg		void * controlReg				ID of the selected COM Wing
* @return	U8 errorCode					STD Error Code
*/
U8 opRS485Parse(void * controlReg);
// ======================== //
// ############################################## //

#endif
