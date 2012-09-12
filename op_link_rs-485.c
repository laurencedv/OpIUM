/*!
 @file		op_link_rs-485.c
 @brief		

 @version	0.1
 @note		
 @todo		

 @date		August 14th 2012
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "op_link_rs-485.h"
// ############################################## //


// ################## Variables ################# //
extern U32 heapAvailable;

// ==== Control ==== //
tOpRS485Control * opRS485ControlReg[COM_WING_NB];
// ================= //
// ############################################## //


// ############## Control Functions ############# //
/**
* \fn		U8 opRS485Init(U8 comWingID)
* @brief	
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 errorCode					STD Error Code
*/
U8 opRS485Init(U8 comWingID)
{
	tOpRS485Control * tempOpRS485ControlReg;
	// -- Allocate Control -- //
	tempOpRS485ControlReg = (tOpRS485Control*)malloc(sizeof(tOpRS485Control));
	if (tempOpRS485ControlReg != NULL)
	{
		// -- Allocate the slot control -- //
		tempOpRS485ControlReg->slotControl = (tOpRS485Slot*)malloc(sizeof(tOpRS485Slot));
		if (tempOpRS485ControlReg->slotControl != NULL)
		{
			//Count the allocated ram
			heapAvailable -= (sizeof(tOpRS485Control)+sizeof(tOpRS485Slot));

			// -- Assign Hardware -- //
			tempOpRS485ControlReg->timerID = 0;									// TODO
			tempOpRS485ControlReg->uartID = 0;
			// --------------------- //

			// -- Initialise Control -- //
			tempOpRS485ControlReg->currentFrame = 0;
			tempOpRS485ControlReg->currentSlot = 0;
			tempOpRS485ControlReg->linkState = detect;
			tempOpRS485ControlReg->linkSubState = init;
			tempOpRS485ControlReg->slotNb = 0;
			tempOpRS485ControlReg->utilityCnt = 0;
			// ------------------------ //

			opRS485ControlReg[comWingID] = tempOpRS485ControlReg;	//Save the Control Reg

			return STD_EC_SUCCESS;
		}
		else
			free(tempOpRS485ControlReg);			//Free up the Control
		// ------------------------------- //
	}
	// ---------------------- //

	return STD_EC_FAIL;
}

/**
* \fn		U8 opRS485Control (U8 comWingID)
* @brief
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 errorCode					STD Error Code
*/
U8 opRS485Control (U8 comWingID)
{

}

/**
* \fn		U8 opRS485Engine (U8 comWingID)
* @brief
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 errorCode					STD Error Code
*/
U8 opRS485Engine (U8 comWingID)
{

}
// ############################################## //


// ############# Tranfer Functions ############## //

// ############################################## //
