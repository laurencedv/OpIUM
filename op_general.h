/*!
 @file		op_general.h
 @brief		General control of the OpIUM protocol

 @version	0.1
 @note
 @todo		

 @date		April 19th 2012
 @author	Laurence DV
*/


#ifndef _OP_GENERAL_H
#define _OP_GENERAL_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Layers

// Lib
#include <protocol/opium/op_link_rs-485.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// COM wings
#define COM_WING_1	0
#define COM_WING_2	1
#define COM_WING_3	2
#define COM_WING_4	3

// COM wings ID value	(ADC value >>4)
#define COM_WING_VALUE_NRF			0x05
#define COM_WING_VALUE_RS485		0x3A
#define COM_WING_VALUE_SPI			0x00
#define COM_WING_VALUE_BLUETOOTH	0x20
#define COM_WING_VALUE_ETHERNET		0x33
#define COM_WING_VALUE_LOL			0x3F
// ############################################## //


// ################# Data Type ################## //
// COM wing state
typedef enum
{
	undetected = 0,
	detected,
	assigned,
	idle,
	busy,
	error,
}tCOMWingState;

// COM wing type
typedef enum
{
	unknown = 0,
	nrf,
	rs485,
	spi,
	bluetooth,
	ethernet,
	lol
}tCOMWingType;

// COM wing control
typedef union
{
	U32 all[3];
	struct
	{
		tCOMWingType type;
		tCOMWingState state;
		void * controlReg;
		U8 (*comWingInit)(U8 comWingID);
		U8 (*comWingControl)(U8 comWingID);
		U8 (*comWingEngine)(U8 comWingID);
	};
}tCOMWingControl;

// Packet Type
typedef enum
{
	nodeCommand = 0x00,
	nodeDiagnostic = 0x01,
	nodeFirmwareUpdate = 0x02,
	data = 0x10,
	netCommand = 0x40,
	netDiagnostic = 0x41
}tOpPacketType;

// Packet Header structure
typedef union
{
	U32 all[5];
	struct
	{
		U8 delimiter;
		struct
		{
			U8 retryCnt:4;
			U8 :1;
			U8 ack:1;
			U8 addressSize:2;
		}status;
		U8 destination[6];
		U8 source[6];
		U16 packetID;
		struct
		{
			U8 responseFlag:1;
			tOpPacketType packetType;
		};
		struct
		{
			U8 option:6;
			U8 payloadSizeMsb:2;
		};
		U8 payloadSize;
	};
}tOpPacketHeader;
// ############################################## //


// ################# Prototypes ################# //
// ==== Buffer Functions ==== //

// ========================== //

// ==== Control Functions ==== //

// =========================== //
// ############################################## //

#endif
