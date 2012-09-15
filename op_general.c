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
const U8 comTimerID[1] = {COM0_TIMER_ID};
const U8 comUartID[1] = {COM0_UART_ID};
const U8 comSPIID[1] = {COM0_SPI_ID};
#elif COM_WING_NB == 2
const tADCInput comIDan[2] = {COM0_ID_AN,COM1_ID_AN};
const U8 comTimerID[2] = {COM0_TIMER_ID,COM1_TIMER_ID};
const U8 comUartID[2] = {COM0_UART_ID,COM1_UART_ID};
const U8 comSPIID[2] = {COM0_SPI_ID,COM1_SPI_ID};
#elif COM_WING_NB == 4
const tADCInput comIDan[4] = {COM0_ID_AN,COM1_ID_AN,COM2_ID_AN,COM3_ID_AN};
const U8 comTimerID[4] = {COM0_TIMER_ID,COM1_TIMER_ID,COM2_TIMER_ID,COM3_TIMER_ID};
const U8 comUartID[4] = {COM0_UART_ID,COM1_UART_ID,COM2_UART_ID,COM3_UART_ID};
const U8 comSPIID[4] = {COM0_SPI_ID,COM1_SPI_ID,COM2_SPI_ID,COM3_SPI_ID};
#endif
// ############################################## //


// ################## Variables ################# //
extern U32 heapAvailable;
tCOMWingControl COMWingControl[COM_WING_NB];				//General Control

// -- COM Detection Engine -- //
U16 comDetectResult[COM_WING_NB][COM_WING_DETECT_RESULT_NB];		//ADC result buffer
U8 comDetectSoftCntID = SOFT_CNT_MAX;					//ID of the assigned soft Counter
U8 comDetectSoftCntFlag;						//Flag for the soft Counter
U8 comDetectADCDoneFlag = ADC_CONV_DONE;				//Flag for the completion of the ADC conversions
U16 comDetectPeriod = COM_WING_DETECT_PERIOD;				//number of sysTick between presence detection
U8 comDetectComID = COM_WING_0;						//COM Wing actually being detected
tFSMState comDetectEngineState = init;					//State of detection engine
// -------------------------- //
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
* \fn		U8 comWingIdentify(U8 comWingID, U16 IDData)
* @brief	Scan and detect present COM Wings
* @note
* @arg		U8 comWingID				ID of the wing targeted
* @return	U8 errorCode				STD Error Code
*/
U8 comWingIdentify(U8 comWingID, U16 IDData)
{
	tCOMWingType wingType;						//the new type of wing

	// Format the data correctly
	IDData >>= 4;

	// -- Type assignation -- //
	switch (IDData)
	{
		case COM_WING_VALUE_NRF: wingType = CWTnrf;		break;
		case COM_WING_VALUE_RS485: wingType = CWTrs485;		break;
		case COM_WING_VALUE_SPI: wingType = CWTspi;		break;
		case COM_WING_VALUE_BLUETOOTH: wingType = CWTbluetooth;	break;
		case COM_WING_VALUE_ETHERNET: wingType = CWTethernet;	break;
		case COM_WING_VALUE_LOL: wingType = CWTlol;		break;
		default: wingType = CWTunknown;
	}
	// ---------------------- //

	// -- Check for a type change -- //
	if (wingType != COMWingControl[comWingID].type)
	{
		COMWingControl[comWingID].type = wingType;		//Save the type
		// -- Initialise the wing -- //
		if (COMWingControl[comWingID].type == CWTunknown)
			COMWingControl[comWingID].state = CWSundetected;//Reset to undetected state
		else
			COMWingControl[comWingID].state = CWSassign;	//A new wing as been detected
		// ------------------------ //
	}
	// ----------------------------- //

	return STD_EC_SUCCESS;
}

/**
* \fn		U8 comWingAssign(U8 comWingID)
* @brief	Assign the correct function according to the type of COM wing
* @note
* @arg		U8 comWingID				ID of COM wing to be assigned
* @return	U8 errorCode				STD Error Code
*/
U8 comWingAssign(U8 comWingID)
{
	tCOMWingControl * workPtr = &(COMWingControl[comWingID]);

	if (workPtr->state != CWSundetected)
	{
		// -- Assign ISR and Control -- //
		switch (workPtr->type)
		{
			case CWTnrf:
			{
				workPtr->comWingInit = NULL;
				workPtr->comWingControl = NULL;
				workPtr->comWingEngine = NULL;
				break;
			}
			case CWTrs485:
			{
				workPtr->comWingInit = &opRS485Init;
				workPtr->comWingControl = &opRS485Control;
				workPtr->comWingEngine = &opRS485Engine;
				break;
			}
			case CWTspi:
			{
				workPtr->comWingInit = NULL;
				workPtr->comWingControl = NULL;
				workPtr->comWingEngine = NULL;
				break;
			}
			case CWTbluetooth:
			{
				workPtr->comWingInit = NULL;
				workPtr->comWingControl = NULL;
				workPtr->comWingEngine = NULL;
				break;
			}
			case CWTethernet:
			{
				workPtr->comWingInit = NULL;
				workPtr->comWingControl = NULL;
				workPtr->comWingEngine = NULL;
				break;
			}
			case CWTlol:
			{
				workPtr->comWingInit = NULL;
				workPtr->comWingControl = NULL;
				workPtr->comWingEngine = NULL;
				break;
			}
			default:	workPtr->state = CWSerror;
		}
		// ---------------------------- //

		// -- Set the State -- //
		workPtr->state = CWSinit;
		// ------------------- //

		return STD_EC_SUCCESS;
	}

	return STD_EC_FAIL;
}

/**
* \fn		U8 comWingDetectionEngine(void)
* @brief	Engine to detect and identify COM Wings
* @note		This function must be in the infinite loop of the main
* @arg		nothing
* @return	U8 errorCode				STD Error Code
*/
U8 comWingDetectionEngine(void)
{
	U8 wu0;
	U32 tempResult = 0;

	switch (comDetectEngineState)
	{
		case init:
		{
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

			comDetectEngineState = idle;
			break;
		}
		case idle:
		{
			// -- Wait for the flag -- //
			if (comDetectSoftCntFlag == 0x01)
			{
				// -- Convert 10 time the ID pin -- //
				adcConvert(ADC_1, comIDan[comDetectComID], COM_WING_DETECT_RESULT_NB, &comDetectResult[comDetectComID][0], &comDetectADCDoneFlag);
				// -------------------------------- //

				comDetectSoftCntFlag = 0;			//Clear the flag
				comDetectEngineState = busy;
			}
			// ----------------------- //
			break;
		}
		case busy:
		{
			if (comDetectADCDoneFlag == ADC_CONV_DONE)
			{
				// -- Check if the pin is floating -- //

				// ---------------------------------- //

				// -- Round the result -- //
				for (wu0 = 0; wu0 < COM_WING_DETECT_RESULT_NB; wu0++)
					tempResult += comDetectResult[comDetectComID][wu0];
				tempResult /= COM_WING_DETECT_RESULT_NB;
				// ---------------------- //

				// -- Identify the correct type -- //
				comWingIdentify(comDetectComID, tempResult);
				// ------------------------------- //

				// -- Select the next COM Wing -- //
				comDetectComID++;
				if (comDetectComID >= COM_WING_NB)
					comDetectComID = 0;
				// ------------------------------ //

				comDetectEngineState = idle;			//Wait for the counter to underRun
			}
			break;
		}
		default: comDetectEngineState = init;	break;
	}
	return STD_EC_SUCCESS;
}

/**
* \fn		U8 comWingEngine(U8 comWingID)
* @brief	Engine to operate a COM Wing
* @note		This function must be in the infinite loop of the main
* @arg		U8 comWingID				ID of the wings selected
* @return	U8 errorCode				STD Error Code
*/
U8 comWingEngine(U8 comWingID)
{
	tCOMWingControl * workPtr = &(COMWingControl[comWingID]);	

	switch (workPtr->state)
	{
		//* -- No Wing -- *//
		case CWSundetected:	workPtr->type = CWTunknown;	break;
		//* -- Assign --- *//
		case CWSassign:
		{
			// -- Assign variables and function -- //
			comWingAssign(comWingID);
			// ----------------------------------- //

			workPtr->state = CWSinit;
			break;
		}
		//* -- Init ----- *//
		case CWSinit:
		{
			// -- Init and save the control Reg -- //
			workPtr->controlReg = workPtr->comWingInit(comWingID);
			// ----------------------------------- //
			
			workPtr->state = CWSidle;
			break;
		}
		//* -- Idle ----- *//
		case CWSidle:
		{
			// Idle action

			break;
		}
		//* -- Busy ----- *//
		case CWSbusy:
		{
			// Wait for idle
			break;
		}
		//* -- Error ---- *//
		case CWSerror:
		{
			// Handle error
			break;
		}
		default: workPtr->state = CWSundetected;
		//* ------------- *//
	}

	return STD_EC_SUCCESS;
}

// ############################################## //
