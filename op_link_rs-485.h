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
// ############################################## //


// ################# Data Type ################## //
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
	RSSactive,
	RSSwait
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
	U32 all[5];
	struct
	{
		tOpRS485LinkState linkState;
		tOpRS485LinkSubState linkSubState;
		U8 timerID;
		U8 uartID;
		U8 utilityCnt;
		U8 slotNb;
		tOpRS485Slot * slotControl;
		U8 currentFrame;
		U8 currentSlot;
		U8 statusLedState;
		U8 statusLedSoftCntID;
		U8 terminatorState;
		U8 dataDirection;
		U8 rsAddress;
		U8 comWingID;
	};
}tOpRS485Control;

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
	U32 all;
	struct
	{
		U8 delimiter;
		U8 byteNb;
		U8 destination;
		tOpRS485PacketCommand command;
	};
}tOpRS485PacketHeader;
// ------------------- //
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
* \fn		tOpRS485Control * opRS485Create(U8 comWingID)
* @brief	Create the control reg for a RS-485 Wing and initialise the peripheral
* @note
* @arg		void * controlReg				Pointer to the control Reg
* @return	tOpRS485Control * tempOpRS485ControlReg		Pointer to the control Reg
*/
tOpRS485Control * opRS485Create(U8 comWingID);

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
