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
extern U8 COM_TIMER_ID[];
extern U8 COM_UART_ID[];
extern tIntIRQ COM_TIMER_INT[];
extern tIntIRQ COM_UART_INT[];

// ==== Control ==== //
tOpRS485Control * opRS485ControlReg[COM_WING_NB] = {NULL};
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
			tempOpRS485ControlReg->timerID = COM_TIMER_ID[comWingID];									// TODO
			tempOpRS485ControlReg->uartID = COM_UART_ID[comWingID];
			// --------------------- //

			// -- Initialise Control -- //
			tempOpRS485ControlReg->currentFrame = 0;
			tempOpRS485ControlReg->currentSlot = 0;
			tempOpRS485ControlReg->linkState = RSSdetect;
			tempOpRS485ControlReg->linkSubState = RSSinit;
			tempOpRS485ControlReg->slotNb = 0;
			tempOpRS485ControlReg->utilityCnt = 0;
			// ------------------------ //

			opRS485ControlReg[comWingID] = tempOpRS485ControlReg;	//Save the Control Reg
		}
		else
		{
			// -- Something went Wrong -- //
			//opRS485Destroy(tempOpRS485ControlReg);			//Destroy the control Reg
			tempOpRS485ControlReg = NULL;				//Output a NULL pointer
			// -------------------------- //
		}
		// ------------------------------- //

		// -- Init the IO -- //
		// TODO set all the IO
		// ----------------- //

		// -- Display the status -- //
		opRS485SetStatusLed(comWingID, OP_RS485_LED_STAT_BLINK_FAST);	//Status LED will flash rapidly to indicate when are searching for a network
		// ------------------------ //

		// -- Init Hardware -- //
		if (uartInit(tempOpRS485ControlReg->uartID,UART_TX_INT_TSR_EMPTY|UART_RX_INT_DATA_READY|UART_MODE_8N1) != STD_EC_SUCCESS)
			Nop();							// TODO report the error
		if (uartSetBaudRate(tempOpRS485ControlReg->uartID, COM_UART_LOW_SPEED) != STD_EC_SUCCESS)
			Nop();							// TODO report the error
		if (timerInit(tempOpRS485ControlReg->timerID,TMR_CS_PBCLK|TMR_16BIT|TMR_DIV_1) != STD_EC_SUCCESS)
			Nop();							// TODO report the error
		// ------------------- //

		// -- Init the interrupts -- //
		intSetPriority(COM_TIMER_INT[comWingID],5,0);
		intSetPriority(COM_UART_INT[comWingID],5,3);
		intInit(COM_TIMER_INT[comWingID]);
		intInit(COM_UART_INT[comWingID]);
		// ------------------------- //
	}
	// ---------------------- //

	return tempOpRS485ControlReg;
}

/**
* \fn		U8 opRS485Destroy(tOpRS485Control * controlToDestroy)
* @brief	Deallocated and stop everything related to a RS-485 COM Wing
* @note		This can fail! Try-and-Pray!
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	nothing
*/
void opRS485Destroy(U8 comWingID)
{
	tOpRS485Control * controlToDestroy = opRS485ControlReg[comWingID];

	// -- Disable interrupts -- //
	intSetState(COM_TIMER_INT[comWingID],DISABLE);
	intSetState(COM_UART_INT[comWingID],DISABLE);
	// ------------------------ //

	// -- Stop Hardware -- //
	softCntRelease(controlToDestroy->statusLedSoftCntID);
	timerStop(controlToDestroy->timerID);
	// TODO uartStop()	when it will exist...
	// ------------------- //

	// -- Reset IO -- //
	opRS485SetStatusLed(comWingID, OP_RS485_LED_STAT_OFF);
	// TODO reset all the other IO
	// -------------- //

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
U8 opRS485Engine(U8 comWingID)
{
	tOpRS485Control * workPtr = opRS485ControlReg[comWingID];
	switch (workPtr->linkState)
	{
		//* -- Detect Network ------ *//
		case RSSdetect:
		{
			// -- Initialise -- //
			if (workPtr->linkSubState == RSSinit)
			{
				// Wait for a sync frame
				timerSetOverflow(workPtr->timerID,OP_RS485_TIME_WAIT_INIT);
				timerStart(workPtr->timerID);
				workPtr->linkSubState == RSSwait;
			}
			// ---------------- //
			break;
		}
		//* -- Election of Master -- *//
		case RSSelection:
		{
			break;
		}
		//* -- Master -------------- *//
		case RSSmaster:
		{
			break;
		}
		//* -- Slave --------------- *//
		case RSSslave:
		{
			break;
		}
		default:	break;
		//* ------------------------ *//
	}
	return STD_EC_SUCCESS;
}

/**
* \fn		U8 opRS485Engine (U8 comWingID)
* @brief
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 errorCode					STD Error Code
*/
U8 opRS485Parse(U8 comWingID)
{
	
	return STD_EC_SUCCESS;
}

/**
* \fn		void opRS485SetTerm(U8 comWingID, U8 termState)
* @brief	Set the state of the RS-485 120? terminator
* @note		Use ENABLE or DISABLE for $termState
* @arg		U8 comWingID					ID of the selected COM Wing
* @arg		U8 termState					State of the terminator
* @return	nothing
*/
void opRS485SetTerm(U8 comWingID, U8 termState)
{
	switch (comWingID)
	{
	#if COM_WING_NB >= 1
		case COM_WING_0: termState == 1 ? setPIN(COM0_IO0) : clearPIN(COM0_IO0);	break;
	#if COM_WING_NB >= 2
		case COM_WING_1: termState == 1 ? setPIN(COM1_IO0) : clearPIN(COM1_IO0);	break;
	#if COM_WING_NB == 4
		case COM_WING_2: termState == 1 ? setPIN(COM2_IO0) : clearPIN(COM2_IO0);	break;
		case COM_WING_3: termState == 1 ? setPIN(COM3_IO0) : clearPIN(COM3_IO0);	break;
	#endif
	#endif
	#endif
		default:	return;
	}
	opRS485ControlReg[comWingID]->terminatorState = termState;	//Save the state
}

/**
* \fn		U8 opRS485GetTerm(U8 comWingID)
* @brief	Return the actual state of the RS-485 120? terminator
* @note		The state is return as ENABLE or DISABLE
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 termState					State of the terminator
*/
U8 opRS485GetTerm(U8 comWingID)
{

	return opRS485ControlReg[comWingID]->terminatorState;
}

/**
* \fn		void opRS485SetDir(U8 comWingID, U8 dataDir)
* @brief	Set the data direction of the RS-485 transceiver
* @note		Use OP_RS485_DIR_TX or OP_RS485_DIR_RX for $dataDir
* @arg		U8 comWingID					ID of the selected COM Wing
* @arg		U8 termState					State of the terminator
* @return	nothing
*/
void opRS485SetDir(U8 comWingID, U8 dataDir)
{
	switch (comWingID)
	{
	#if COM_WING_NB >= 1
		case COM_WING_0: dataDir == OP_RS485_DIR_TX ? setPIN(COM0_IO1) : clearPIN(COM0_IO1);	break;
	#if COM_WING_NB >= 2
		case COM_WING_1: dataDir == OP_RS485_DIR_TX ? setPIN(COM1_IO1) : clearPIN(COM1_IO1);	break;
	#if COM_WING_NB == 4
		case COM_WING_2: dataDir == OP_RS485_DIR_TX ? setPIN(COM2_IO1) : clearPIN(COM2_IO1);	break;
		case COM_WING_3: dataDir == OP_RS485_DIR_TX ? setPIN(COM3_IO1) : clearPIN(COM3_IO1);	break;
	#endif
	#endif
	#endif
		default:	return;
	}
	opRS485ControlReg[comWingID]->dataDirection = dataDir;
}

/**
* \fn		U8 opRS485GetTerm(U8 comWingID)
* @brief	Return the actual direction of data of the RS-485 transceiver
* @note		The direction is return as OP_RS485_DIR_TX or OP_RS485_DIR_RX
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 dataDir					Direction of the data
*/
U8 opRS485GetDir(U8 comWingID)
{

	return opRS485ControlReg[comWingID]->dataDirection;
}

/**
* \fn		void opRS485SetStatusLed(U8 comWingID, U8 ledState)
* @brief	Set the state of the Status LED
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @arg		U8 ledState					State of the LED
* @return	nothing
*/
void opRS485SetStatusLed(U8 comWingID, U8 ledState)
{
	if (opRS485ControlReg[comWingID] != NULL)
	{
		U32 * ledPtr;
		U32 ledMask;
		U16 blinkPeriod;

		// -- Point to the correct reg -- //
		switch (comWingID)
		{
		#if COM_WING_NB >= 1
			case COM_WING_0: ledPtr = (U32*)&getInvRegPin(COM0_IO2); ledMask = COM0_IO2;	break;
		#if COM_WING_NB >= 2
			case COM_WING_1: ledPtr = (U32*)&getInvRegPin(COM1_IO2); ledMask = COM1_IO2;	break;
		#if COM_WING_NB == 4
			case COM_WING_2: ledPtr = &getInvRegPin(COM2_IO2); ledMask = COM2_IO2;	break;
			case COM_WING_3: ledPtr = &getInvRegPin(COM3_IO2); ledMask = COM3_IO2;	break;
		#endif
		#endif
		#endif
			default:	break;
		}
		// ------------------------------ //

		// -- Save the state -- //
		opRS485ControlReg[comWingID]->statusLedState = ledState;
		// -------------------- //

		// -- Start a soft Cnt -- //
		switch (ledState)
		{
			case OP_RS485_LED_STAT_BLINK:		blinkPeriod = 250;	break;
			case OP_RS485_LED_STAT_BLINK_SLOW:	blinkPeriod = 500;	break;
			case OP_RS485_LED_STAT_BLINK_FAST:	blinkPeriod = 100;	break;
			default:	ledPtr[-2+ledState] = ledMask;		return;
		}
		opRS485ControlReg[comWingID]->statusLedSoftCntID = softCntInit(blinkPeriod, ledPtr, ledMask,SOFT_CNT_RELOAD_EN+SOFT_CNT_TARGET_EN);
		softCntStart(opRS485ControlReg[comWingID]->statusLedSoftCntID);
		// ---------------------- //
	}
}

/**
* \fn		U8 opRS485GetStatusLed(U8 comWingID)
* @brief	Return the actual state of the Status LED
* @note
* @arg		U8 comWingID					ID of the selected COM Wing
* @return	U8 ledState					State of the LED
*/
U8 opRS485GetStatusLed(U8 comWingID)
{
	return opRS485ControlReg[comWingID]->statusLedState;
}
// ############################################## //


// ############# Tranfer Functions ############## //

// ############################################## //
