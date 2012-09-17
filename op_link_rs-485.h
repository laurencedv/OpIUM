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
// Pin macro
#define OP_RS485_PIN_TERM		_IO0
#define OP_RS485_PIN_DIR		_IO1
#define OP_RS485_PIN_LED_STAT		_IO2

// Status LED
#define OP_RS485_LED_STAT_OFF		0
#define OP_RS485_LED_STAT_ON		0x1
#define OP_RS485_LED_STAT_BLINK		0x2
#define OP_RS485_LED_STAT_BLINK_SLOW	0x3
#define OP_RS485_LED_STAT_BLINK_FAST	0x4
// ############################################## //


// ################# Data Type ################## //
// Link State
typedef enum
{
	detect = 0,
	election,
	master,
	slave
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
	U32 all[4];
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
	};
}tOpRS485Control;
// ############################################## //


// ################# Prototypes ################# //
// ==== Control Functions ==== //
/**
* \fn		U8 opRS485Init(U8 comWingID)
* @brief
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 errorCode					STD Error Code
*/
tOpRS485Control * opRS485Init(U8 comWingID);

/**
* \fn		U8 opRS485Destroy(tOpRS485Control * controlToDestroy)
* @brief	Destroy all memory associated with a RS-485 COM Wing
* @note		This can fail! Try-and-Pray!
* @arg		tOpRS485Control * controlToDestroy		Pointer to the control reg to be destroyed
* @return	nothing
*/
void opRS485Destroy(tOpRS485Control * controlToDestroy);

/**
* \fn		U8 opRS485Control (U8 comWingID)
* @brief
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 errorCode					STD Error Code
*/
U8 opRS485Control (U8 comWingID);

/**
* \fn		U8 opRS485Engine (U8 comWingID)
* @brief
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 errorCode					STD Error Code
*/
U8 opRS485Engine (U8 comWingID);

/**
* \fn		void opRS485SetTerm(U8 comWingID, U8 termState)
* @brief	Set the state of the RS-485 120? terminator
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @arg		U8 termState					State of the terminator
* @return	nothing
*/
void opRS485SetTerm(U8 comWingID, U8 termState);

/**
* \fn		U8 opRS485GetTerm(U8 comWingID)
* @brief	Return the actual state of the RS-485 120? terminator
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 termState					State of the terminator
*/
U8 opRS485GetTerm(U8 comWingID);

/**
* \fn		void opRS485SetStatusLed(U8 comWingID, U8 ledState)
* @brief	Set the state of the Status LED
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @arg		U8 ledState					State of the LED
* @return	nothing
*/
void opRS485SetStatusLed(U8 comWingID, U8 ledState);

/**
* \fn		U8 opRS485GetStatusLed(U8 comWingID)
* @brief	Return the actual state of the Status LED
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 ledState					State of the LED
*/
U8 opRS485GetStatusLed(U8 comWingID);
// =========================== //
// ############################################## //

#endif
