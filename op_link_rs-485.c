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
extern U8 comTimerID[];
extern U8 comUartID[];

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
tOpRS485Control * opRS485Init(U8 comWingID)
{
	tOpRS485Control * tempOpRS485ControlReg;

	// -- Allocate Control -- //
	tempOpRS485ControlReg = (tOpRS485Control*)malloc(sizeof(tOpRS485Control));
	if (tempOpRS485ControlReg != NULL)
	{
		//Count the allocated ram
		heapAvailable -= sizeof(tOpRS485Control);

		// -- Allocate the slot control -- //
		tempOpRS485ControlReg->slotControl = (tOpRS485Slot*)malloc(sizeof(tOpRS485Slot));
		if (tempOpRS485ControlReg->slotControl != NULL)
		{
			//Count the allocated ram
			heapAvailable -= sizeof(tOpRS485Slot);

			// -- Assign Hardware -- //
			tempOpRS485ControlReg->timerID = comTimerID[comWingID];									// TODO
			tempOpRS485ControlReg->uartID = comUartID[comWingID];
			// --------------------- //

			// -- Initialise Control -- //
			tempOpRS485ControlReg->currentFrame = 0;
			tempOpRS485ControlReg->currentSlot = 0;
			tempOpRS485ControlReg->linkState = detect;
			tempOpRS485ControlReg->linkSubState = RSSinit;
			tempOpRS485ControlReg->slotNb = 0;
			tempOpRS485ControlReg->utilityCnt = 0;
			// ------------------------ //

			opRS485ControlReg[comWingID] = tempOpRS485ControlReg;	//Save the Control Reg
		}
		else
		{
			// -- Something went Wrong -- //
			opRS485Destroy(tempOpRS485ControlReg);			//Destroy the control Reg
			tempOpRS485ControlReg = NULL;				//Output a NULL pointer
			// -------------------------- //
		}
		// ------------------------------- //

		// -- Init Hardware -- //
		if (uartInit(tempOpRS485ControlReg->uartID,UART_TX_INT_TSR_EMPTY|UART_RX_INT_DATA_READY|UART_MODE_8N1) != STD_EC_SUCCESS)
			Nop();							// TODO report the error
		if (uartSetBaudRate(tempOpRS485ControlReg->uartID, COM_UART_LOW_SPEED) != STD_EC_SUCCESS)
			Nop();							// TODO report the error
		if (timerInit(tempOpRS485ControlReg->timerID,TMR_CS_PBCLK|TMR_16BIT|TMR_DIV_1) != STD_EC_SUCCESS)
			Nop();							// TODO report the error
		// ------------------- //
	}
	// ---------------------- //

	return tempOpRS485ControlReg;
}

/**
* \fn		U8 opRS485Destroy(tOpRS485Control * controlToDestroy)
* @brief	Destroy all memory associated with a RS-485 COM Wing
* @note		This can fail! Try-and-Pray!
* @arg		tOpRS485Control * controlToDestroy		Pointer to the control reg to be destroyed
* @return	nothing
*/
void opRS485Destroy(tOpRS485Control * controlToDestroy)
{
	// -- Destroy all memory allocated -- //
	free(controlToDestroy->slotControl) ;
	free(controlToDestroy);
	heapAvailable += (sizeof(tOpRS485Control)+sizeof(tOpRS485Slot));
	// ---------------------------------- //
}

/**
* \fn		U8 opRS485Control (U8 comWingID)
* @brief
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 errorCode					STD Error Code
*/
U8 opRS485Control(U8 comWingID)
{

}

/**
* \fn		U8 opRS485Engine (U8 comWingID)
* @brief
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 errorCode					STD Error Code
*/
U8 opRS485Engine(U8 comWingID)
{

}
// ############################################## //


// ############# Tranfer Functions ############## //

// ############################################## //
