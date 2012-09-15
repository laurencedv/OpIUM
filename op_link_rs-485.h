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
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //

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
* @note		This can fail! watch the return value as it will tell you if it was successful
* @arg		tOpRS485Control * controlToDestroy		Pointer to the control reg to be destroyed
* @return	U8 errorCode					STD Error Code
*/
U8 opRS485Destroy(tOpRS485Control * controlToDestroy);

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
// =========================== //
// ############################################## //

#endif
