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


// ################## Defines ################### //
//COM Wing ID AN address
#if COM_WING_NB == 1
const tADCInput comIDan[1] = {COM0_ID_AN};
#elif COM_WING_NB == 2
const tADCInput comIDan[2] = {COM0_ID_AN,COM1_ID_AN};
#elif COM_WING_NB == 4
const tADCInput comIDan[4] = {COM0_ID_AN,COM1_ID_AN,COM2_ID_AN,COM3_ID_AN};
#endif
// ############################################## //


// ################## Variables ################# //
extern U32 heapAvailable;
tCOMWingControl COMWingControl[COM_WING_NB];				//General Control
U16 comDetectResult[COM_WING_NB][COM_WING_DETECT_RESULT_NB];		//ADC result buffer
U8 comDetectSoftCntID = SOFT_CNT_MAX;					//ID of the assigned soft Counter
U8 comDetectSoftCntFlag;						//Flag for the soft Counter
U16 comDetectPeriod = COM_WING_DETECT_PERIOD;				//number of sysTick between presence detection
U8 comDetectComID = COM_WING_0;						//COM Wing actually being detected
tFSMState comDetectEngineState = init;					//State of detection engine
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
	tCOMWingType wingType;						//the new type of wing
	U8 readValue = 0;

	// -- Detect Presence -- //
	// ADC Read

	readValue >>= 4;

	// Type assignation
	switch (readValue)
	{
		case COM_WING_VALUE_NRF: wingType = CWTnrf;		break;
		case COM_WING_VALUE_RS485: wingType = CWTrs485;		break;
		case COM_WING_VALUE_SPI: wingType = CWTspi;		break;
		case COM_WING_VALUE_BLUETOOTH: wingType = CWTbluetooth;	break;
		case COM_WING_VALUE_ETHERNET: wingType = CWTethernet;	break;
		case COM_WING_VALUE_LOL: wingType = CWTlol;		break;
		default: wingType = CWTunknown;
	}
	// --------------------- //

	// -- Check for a type change -- //
	if (wingType != COMWingControl[comWingID].type)
	{
		// -- Initialise the wing -- //
		if (COMWingControl[comWingID].type != CWTunknown)
		{
			COMWingControl[comWingID].state = CWSdetected;	//A new wing as been detected
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
	tCOMWingControl * tempCOMWingControlReg = &(COMWingControl[comWingID]);

	if (tempCOMWingControlReg->state != CWSundetected)
	{
		// -- Assign ISR and Control -- //
		switch (tempCOMWingControlReg->type)
		{
			case CWTnrf:
			{
				tempCOMWingControlReg->comWingInit = NULL;
				tempCOMWingControlReg->comWingControl = NULL;
				tempCOMWingControlReg->comWingEngine = NULL;
				break;
			}
			case CWTrs485:
			{
				tempCOMWingControlReg->comWingInit = &opRS485Init;
				tempCOMWingControlReg->comWingControl = &opRS485Control;
				tempCOMWingControlReg->comWingEngine = &opRS485Engine;
				break;
			}
			case CWTspi:
			{
				tempCOMWingControlReg->comWingInit = NULL;
				tempCOMWingControlReg->comWingControl = NULL;
				tempCOMWingControlReg->comWingEngine = NULL;
				break;
			}
			case CWTbluetooth:
			{
				tempCOMWingControlReg->comWingInit = NULL;
				tempCOMWingControlReg->comWingControl = NULL;
				tempCOMWingControlReg->comWingEngine = NULL;
				break;
			}
			case CWTethernet:
			{
				tempCOMWingControlReg->comWingInit = NULL;
				tempCOMWingControlReg->comWingControl = NULL;
				tempCOMWingControlReg->comWingEngine = NULL;
				break;
			}
			case CWTlol:
			{
				tempCOMWingControlReg->comWingInit = NULL;
				tempCOMWingControlReg->comWingControl = NULL;
				tempCOMWingControlReg->comWingEngine = NULL;
				break;
			}
			default:	tempCOMWingControlReg->state = CWSerror;
		}
		// ---------------------------- //

		// -- Set the State -- //
		tempCOMWingControlReg->state = CWSassigned;
		// ------------------- //

		return STD_EC_SUCCESS;
	}

	return STD_EC_FAIL;
}

U8 comDetectionEngine(void)
{
	switch (comDetectEngineState)
	{
		case init:
		{
			U8 wu0;

			// -- init variables -- //
			comDetectSoftCntFlag = 0;
			for (wu0 = 0; wu0 < COM_WING_DETECT_RESULT_NB; wu0++)
				comDetectResult[comDetectComID][wu0] = 0;
			// -------------------- //

			// -- init the soft Counter for detection -- //
			if (comDetectSoftCntID != SOFT_CNT_MAX)			//Soft Counter as already been assigned
				softCntRelease(comDetectSoftCntID);		//Release it
			comDetectSoftCntID = softCntInit(comDetectPeriod, &comDetectSoftCntFlag, 0x01, SOFT_CNT_RELOAD_EN+SOFT_CNT_TARGET_EN);
			if (comDetectSoftCntID == STD_EC_OVERFLOW)
				return STD_EC_NOTFOUND;
			// ----------------------------------------- //

			// -- Start the soft Counter -- //
			softCntStart(comDetectSoftCntID);
			// ---------------------------- //

			comDetectEngineState = busy;
		}
		case busy:
		{
			// -- Wait for the flag -- //
			if (comDetectSoftCntFlag == 0x01)
			{
				// -- Convert 10 time the ID pin -- //
				adcConvert(ADC_1, comIDan[comDetectComID], COM_WING_DETECT_RESULT_NB, &comDetectResult[comDetectComID][0]);
				comDetectComID++;
				// -------------------------------- //

				comDetectSoftCntFlag = 0;			//Clear the flag
			}
			// ----------------------- //
		}
		default: comDetectEngineState = init;	break;
	}
	return STD_EC_SUCCESS;
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
	tCOMWingControl * workPtr = &(COMWingControl[comWingID]);

	

	switch (workPtr->state)
	{
		case CWSinit:
		{
			// -- init variables -- //
			workPtr->softCntFlag = 0;
			workPtr->detectPeriod = COM_WING_DETECT_PERIOD;
			workPtr->type = CWTunknown;
			// -------------------- //

			// -- init the soft Counter for detection -- //
			workPtr->softCntID = softCntInit(workPtr->detectPeriod, &(workPtr->softCntFlag), 0x01, SOFT_CNT_RELOAD_EN+SOFT_CNT_TARGET_EN);
			if (workPtr->softCntID == STD_EC_OVERFLOW)
				return STD_EC_NOTFOUND;
			// ----------------------------------------- //

			// -- Start the soft Counter -- //
			softCntStart(workPtr->softCntID);
			// ---------------------------- //

			workPtr->state = CWSundetected;
			break;
		}
		case CWSundetected:
		{


			// Scan for the presence of a wing
			break;
		}
		case CWSdetected:
		{
			// Assign the correct function for this type of wing
			break;
		}
		case CWSassigned:
		{
			// Initialise the specific control reg
			break;
		}
		case CWSidle:
		{
			// Idle action

			break;
		}
		case CWSbusy:
		{
			// Wait for idle
			break;
		}
		case CWSerror:
		{
			// Handle error
			break;
		}
		default: COMWingControl[comWingID].state = CWSerror;
	}

	return STD_EC_SUCCESS;
}

// ############################################## //
