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
// == COM Wings Access == //
#if COM_WING_NB == 1
//Peripheral HW Port
const tADCInput comIDan[1] = {COM0_ID_AN};
const U8 comTimerID[1] = {COM0_TIMER_ID};
const U8 comUartID[1] = {COM0_UART_ID};
const U8 comSPIID[1] = {COM0_SPI_ID};

//Interrupt IRQ
const tIntIRQ COM_TIMER_INT[1] = {COM0_TIMER_INT_IRQ};
const tIntIRQ COM_UART_INT[1] = {COM0_UART_INT_IRQ};
const tIntIRQ COM_SPI_INT[1] = {COM0_SPI_INT_IRQ};
const tIntIRQ COM_IRQ0_INT[1] = {COM0_IRQ0_INT_IRQ};
const tIntIRQ COM_IRQ1_INT[1] = {COM0_IRQ1_INT_IRQ};

#elif COM_WING_NB == 2
//Peripheral HW Port
const tADCInput COM_ID_AN[2] = {COM0_ID_AN,COM1_ID_AN};
const U8 COM_TIMER_ID[2] = {COM0_TIMER_ID,COM1_TIMER_ID};
const U8 COM_UART_ID[2] = {COM0_UART_ID,COM1_UART_ID};
const U8 COM_SPI_ID[2] = {COM0_SPI_ID,COM1_SPI_ID};

//Interrupt IRQ
const tIntIRQ COM_TIMER_INT[2] = {COM0_TIMER_INT_IRQ,COM1_TIMER_INT_IRQ};
const tIntIRQ COM_UART_INT[2] = {COM0_UART_INT_IRQ,COM1_UART_INT_IRQ};
const tIntIRQ COM_SPI_INT[2] = {COM0_SPI_INT_IRQ,COM1_SPI_INT_IRQ};
const tIntIRQ COM_IRQ0_INT[2] = {COM0_IRQ0_INT_IRQ,COM1_IRQ0_INT_IRQ};
const tIntIRQ COM_IRQ1_INT[2] = {COM0_IRQ1_INT_IRQ,COM1_IRQ1_INT_IRQ};

#elif COM_WING_NB == 4
//Peripheral HW Port
const tADCInput comIDan[4] = {COM0_ID_AN,COM1_ID_AN,COM2_ID_AN,COM3_ID_AN};
const U8 comTimerID[4] = {COM0_TIMER_ID,COM1_TIMER_ID,COM2_TIMER_ID,COM3_TIMER_ID};
const U8 comUartID[4] = {COM0_UART_ID,COM1_UART_ID,COM2_UART_ID,COM3_UART_ID};
const U8 comSPIID[4] = {COM0_SPI_ID,COM1_SPI_ID,COM2_SPI_ID,COM3_SPI_ID};

//Interrupt IRQ
const tIntIRQ COM_TIMER_INT[4] = {COM0_TIMER_INT_IRQ,COM1_TIMER_INT_IRQ,COM2_TIMER_INT_IRQ,COM3_TIMER_INT_IRQ};
const tIntIRQ COM_UART_INT[4] = {COM0_UART_INT_IRQ,COM1_UART_INT_IRQ,COM2_UART_INT_IRQ,COM3_UART_INT_IRQ};
const tIntIRQ COM_SPI_INT[4] = {COM0_SPI_INT_IRQ,COM1_SPI_INT_IRQ,COM2_SPI_INT_IRQ,COM3_SPI_INT_IRQ};
const tIntIRQ COM_IRQ0_INT[4] = {COM0_IRQ0_INT_IRQ,COM1_IRQ0_INT_IRQ,COM2_IRQ0_INT_IRQ,COM3_IRQ0_INT_IRQ};
const tIntIRQ COM_IRQ1_INT[4] = {COM0_IRQ1_INT_IRQ,COM1_IRQ1_INT_IRQ,COM2_IRQ1_INT_IRQ,COM3_IRQ1_INT_IRQ};
#endif
// ====================== //


// == EXT Wings Access == //
#if EXT_WING_NB == 1
//Peripheral HW Port
const U8 EXT_UART_ID[1] = {EXT0_UART_ID};

//Interrupt IRQ
const tIntIRQ EXT_UART_INT[1] = {EXT0_UART_INT_IRQ};
const tIntIRQ EXT_IRQ_INT[1] = {EXT0_IRQ_INT_IRQ};
#elif EXT_WING_NB == 2
//Peripheral HW Port
const U8 EXT_UART_ID[2] = {EXT0_UART_ID,EXT1_UART_ID};

//Interrupt IRQ
const tIntIRQ EXT_UART_INT[2] = {EXT0_UART_INT_IRQ,EXT1_UART_INT_IRQ};
const tIntIRQ EXT_IRQ_INT[2] = {EXT0_IRQ_INT_IRQ,EXT1_IRQ_INT_IRQ};
#endif
// ====================== //
// ############################################## //


// ################## Variables ################# //
extern U32 heapAvailable;
tCOMWingControl COMWingControl[COM_WING_NB];				//General Control

// -- COM Detection Engine -- //
U16 comDetectResult[COM_WING_NB][COM_WING_DETECT_RESULT_NB];		//ADC result buffer
U8 comDetectSoftCntID = SOFT_CNT_MAX;					//ID of the assigned soft Counter
U32 comDetectSoftCntFlag;						//Flag for the soft Counter
U8 comDetectADCDoneFlag = ADC_CONV_DONE;				//Flag for the completion of the ADC conversions
U16 comDetectPeriod = COM_WING_DETECT_PERIOD;				//number of sysTick between presence detection
U8 comDetectComID = COM_WING_0;						//COM Wing actually being detected
tFSMState comDetectEngineState = init;					//State of detection engine
// -------------------------- //
// ############################################## //


// ############### Board Functions ############## //
/**
* \fn		void opInitStatusLed(void)
* @brief	Start the necessary pheripheral to use the Status LED on the node Board
* @note		The PWM module use the Timer 2 without interrupt
* @arg		nothing
* @return	nothing
*/
void opInitStatusLed(void)
{
	timerInit(TIMER_2,0);
	ocSetConfig(LED_R_OC_ID,OC_MODE_PWM|OC_TIMER_2);
	ocSetConfig(LED_G_OC_ID,OC_MODE_PWM|OC_TIMER_2);
	ocSetConfig(LED_B_OC_ID,OC_MODE_PWM|OC_TIMER_2);
	pwmSetPeriod(LED_R_OC_ID,1000);
	pwmSetPeriod(LED_G_OC_ID,1000);
	pwmSetPeriod(LED_B_OC_ID,1000);
	pwmSetDuty(LED_R_OC_ID, 0xFF, 0xFF);
	pwmSetDuty(LED_G_OC_ID, 0xFF, 0xFF);
	pwmSetDuty(LED_B_OC_ID, 0xFF, 0xFF);
	ocStart(LED_R_OC_ID);
	ocStart(LED_G_OC_ID);
	ocStart(LED_B_OC_ID);
}

/**
* \fn		void opSetStatusLed(U8 red, U8 green, U8 blue)
* @brief	Set the color of the Status LED
* @note		The colors value are set between 0 and 0xFF
* @arg		U8 red					Intensity value of the red
* @arg		U8 green				Intensity value of the green
* @arg		U8 blue					Intensity value of the blue
* @return	nothing
*/
void opSetStatusLed(U8 red, U8 green, U8 blue)
{
	pwmSetDuty(LED_R_OC_ID, red, 0xFF);
	pwmSetDuty(LED_G_OC_ID, green, 0xFF);
	pwmSetDuty(LED_B_OC_ID, blue, 0xFF);
}
// ############################################## //


// ############# Control Functions ############## //
/**
* \fn		void comWingInit(U8 comWingID)
* @brief	Initialise the control Reg of a COM Wing
* @note
* @arg		U8 comWingID				ID of the wing targeted
* @return	nothing
*/
void comWingInit(U8 comWingID)
{
	COMWingControl[comWingID].newType = CWTunknown;
	COMWingControl[comWingID].type = CWTunknown;
	COMWingControl[comWingID].state = CWSundetected;
	COMWingControl[comWingID].controlReg = NULL;
}

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
		COMWingControl[comWingID].newType = wingType;		//Save the new type

		// -- Initialise the wing -- //
		if (COMWingControl[comWingID].type == CWTunknown)
			COMWingControl[comWingID].state = CWSassign;	//A new wing as been detected
		else
			COMWingControl[comWingID].state = CWSdisconnect;//The wing has changed, disconnect it
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
U8 comWingAssign(tCOMWingControl * controlPtr)
{
	if (controlPtr->state != CWSundetected)
	{
		// -- Assign ISR and Control -- //
		switch (controlPtr->type)
		{
			case CWTnrf:
			{
				controlPtr->comWingInit = NULL;
				controlPtr->comWingDestroy = NULL;
				controlPtr->comWingControl = NULL;
				controlPtr->comWingEngine = NULL;
				controlPtr->comWingDataISR = NULL;
				controlPtr->comWingTimerISR = NULL;
				break;
			}
			case CWTrs485:
			{
				controlPtr->comWingInit = &opRS485Init;
				controlPtr->comWingDestroy = &opRS485Destroy;
				controlPtr->comWingControl = &opRS485Control;
				controlPtr->comWingEngine = &opRS485Engine;
				controlPtr->comWingDataISR = NULL;
				controlPtr->comWingTimerISR = NULL;
				break;
			}
			case CWTspi:
			{
				controlPtr->comWingInit = NULL;
				controlPtr->comWingDestroy = NULL;
				controlPtr->comWingControl = NULL;
				controlPtr->comWingEngine = NULL;
				controlPtr->comWingDataISR = NULL;
				controlPtr->comWingTimerISR = NULL;
				break;
			}
			case CWTbluetooth:
			{
				controlPtr->comWingInit = NULL;
				controlPtr->comWingDestroy = NULL;
				controlPtr->comWingControl = NULL;
				controlPtr->comWingEngine = NULL;
				controlPtr->comWingDataISR = NULL;
				controlPtr->comWingTimerISR = NULL;
				break;
			}
			case CWTethernet:
			{
				controlPtr->comWingInit = NULL;
				controlPtr->comWingDestroy = NULL;
				controlPtr->comWingControl = NULL;
				controlPtr->comWingEngine = NULL;
				controlPtr->comWingDataISR = NULL;
				controlPtr->comWingTimerISR = NULL;
				break;
			}
			case CWTlol:
			{
				controlPtr->comWingInit = NULL;
				controlPtr->comWingDestroy = NULL;
				controlPtr->comWingControl = NULL;
				controlPtr->comWingEngine = NULL;
				controlPtr->comWingDataISR = NULL;
				controlPtr->comWingTimerISR = NULL;
				break;
			}
			default:	controlPtr->state = CWSerror;
		}
		// ---------------------------- //
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
				adcConvert(ADC_1, COM_ID_AN[comDetectComID], COM_WING_DETECT_RESULT_NB, &comDetectResult[comDetectComID][0], &comDetectADCDoneFlag);
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
		//* -- No Wing ----- *//
		case CWSundetected:	workPtr->type = CWTunknown;	break;
		//* -- Assign ------ *//
		case CWSassign:
		{
			// -- Assign variables and function -- //
			workPtr->type = workPtr->newType;
			comWingAssign(workPtr);
			// ----------------------------------- //

			workPtr->state = CWSinit;
			break;
		}
		//* -- Init -------- *//
		case CWSinit:
		{
			// -- Init and save the control Reg -- //
			workPtr->controlReg = workPtr->comWingInit(comWingID);
			// ----------------------------------- //
			
			workPtr->state = CWSidle;
			break;
		}
		//* -- Idle -------- *//
		case CWSidle:
		{
			// -- Treat the Control -- //
			workPtr->comWingControl(comWingID);
			// ----------------------- //

			break;
		}
		//* -- Busy -------- *//
		case CWSbusy:
		{
			// -- Treat the data -- //
			workPtr->comWingEngine(comWingID);
			// -------------------- //

			break;
		}
		//* -- Error ------- *//
		case CWSerror:
		{
			// Handle error
			
			// -- Handle disconnection -- //
			if (COMWingControl[comWingID].type == CWTunknown)
				workPtr->state = CWSundetected;
			// -------------------------- //

			break;
		}
		//* -- Disconnect -- *//
		case CWSdisconnect:
		{
			// -- Destroy the specific type -- //
			workPtr->comWingDestroy(comWingID);
			// ------------------------------- //

			// -- Assign the new type -- //
			COMWingControl[comWingID].type = COMWingControl[comWingID].newType;
			// ------------------------- //

			workPtr->state = CWSassign;

			break;
		}
		default: workPtr->state = CWSundetected;
		//* ---------------- *//
	}

	return STD_EC_SUCCESS;
}

// ############################################## //
