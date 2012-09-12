/*!
 @file		op_general.c
 @brief		General control of the OpIUM protocol

 @version	0.1
 @note		
 @todo		

 @date		April 19th 2012
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "op_general.h"
// ############################################## //


// ################## Variables ################# //
extern U32 heapAvailable;
tCOMWingControl COMWingControlReg[COM_WING_NB];

// ############################################## //


// ############# Functions Pointers ############# //
/**
* \fn		void comWingControl[COM_WING_NB](void)
* @brief	Control function pointer for each of the COM wing
* @note
* @arg		void
* @return	U8 errorCode					STD Error Code
*/
//void (*comWingControlPtr)[COM_WING_NB] (void);
// ############################################## //


// ############# Control Functions ############## //
/**
* \fn		U8 comWingScan(U8 comWingID)
* @brief	Scan and detect present COM Wings
* @note
* @arg		U8 comWingID					ID of the wing targeted
* @return	U8 errorCode					STD Error Code
*/
U8 comWingScan(U8 comWingID)
{
	tCOMWingType wingType;					//the new type of wing
	U8 readValue = 0;

	// -- Detect Presence -- //
	// ADC Read

	readValue >>= 4;

	// Type assignation
	switch (readValue)
	{
		case COM_WING_VALUE_NRF: wingType = nrf;				break;
		case COM_WING_VALUE_RS485: wingType = rs485;			break;
		case COM_WING_VALUE_SPI: wingType = spi;				break;
		case COM_WING_VALUE_BLUETOOTH: wingType = bluetooth;	break;
		case COM_WING_VALUE_ETHERNET: wingType = ethernet;		break;
		case COM_WING_VALUE_LOL: wingType = lol;				break;
		default: wingType = unknown;
	}
	// --------------------- //

	// -- Check for a type change -- //
	if (wingType != COMWingControlReg[comWingID].type)
	{
		// -- Initialise the wing -- //
		if (COMWingControlReg[comWingID].type != unknown)
		{
			COMWingControlReg[comWingID].state = detected;					//A new wing as been detected
		}
		// ------------------------ //
	}
	// ----------------------------- //

	return STD_EC_SUCCESS;
}

/**
* \fn		U8 nrfInit(void)
* @brief
* @note
* @arg		U8
* @return	U8 errorCode					STD Error Code
*/
U8 comWingAssign(U8 comWingID)
{
	tCOMWingControl * tempCOMWingControlReg = &(COMWingControlReg[comWingID]);

	if (tempCOMWingControlReg->state != undetected)
	{
		// -- Assign ISR and Control -- //
		switch (tempCOMWingControlReg->type)
		{
			case nrf:
			{
				tempCOMWingControlReg->comWingInit = NULL;
				tempCOMWingControlReg->comWingControl = NULL;
				tempCOMWingControlReg->comWingEngine = NULL;
				break;
			}
			case rs485:
			{
				tempCOMWingControlReg->comWingInit = &opRS485Init;
				tempCOMWingControlReg->comWingControl = &opRS485Control;
				tempCOMWingControlReg->comWingEngine = &opRS485Engine;
				break;
			}
			case spi:
			{
				tempCOMWingControlReg->comWingInit = NULL;
				tempCOMWingControlReg->comWingControl = NULL;
				tempCOMWingControlReg->comWingEngine = NULL;
				break;
			}
			case bluetooth:
			{
				tempCOMWingControlReg->comWingInit = NULL;
				tempCOMWingControlReg->comWingControl = NULL;
				tempCOMWingControlReg->comWingEngine = NULL;
				break;
			}
			case ethernet:
			{
				tempCOMWingControlReg->comWingInit = NULL;
				tempCOMWingControlReg->comWingControl = NULL;
				tempCOMWingControlReg->comWingEngine = NULL;
				break;
			}
			case lol:
			{
				tempCOMWingControlReg->comWingInit = NULL;
				tempCOMWingControlReg->comWingControl = NULL;
				tempCOMWingControlReg->comWingEngine = NULL;
				break;
			}
			default:	tempCOMWingControlReg->state = error;
		}
		// ---------------------------- //

		// -- Set the State -- //
		tempCOMWingControlReg->state = assigned;
		// ------------------- //

		return STD_EC_SUCCESS;
	}

	return STD_EC_FAIL;
}

/**
* \fn		U8 nrfInit(void)
* @brief
* @note
* @arg		U8
* @return	U8 errorCode					STD Error Code
*/
U8 comWingEngine(U8 comWingID)
{
	switch (COMWingControlReg[comWingID].state)
	{
		case undetected:
		{
			// Scan for the presence of a wing
			break;
		}
		case detected:
		{
			// Assign the correct function for this type of wing
			break;
		}
		case assigned:
		{
			// Initialise the specific control reg
			break;
		}
		case idle:
		{
			// Idle action

			break;
		}
		case busy:
		{
			// Wait for idle
			break;
		}
		case error:
		{
			// Handle error
			break;
		}
		default: COMWingControlReg[comWingID].state = error;
	}

	return STD_EC_SUCCESS;
}

// ############################################## //
