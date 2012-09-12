/*!
 @file		op_session_layer.h
 @brief		Layer 3 - Session of the OpIUM protocol

 @version	0.1
 @note
 @todo

 @date		April 19th 2012
 @author	Laurence DV
*/


#ifndef _OP_SESSION_LAYER_H
#define _OP_SESSION_LAYER_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Lib

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //

// ############################################## //


// ################# Data Type ################## //
// Special Session header
typedef union
{
	U32 all;
	struct
	{
		U8 sessionID;
		U8 status;
	};

}tOpSessionHeader;
// ############################################## //


// ################# Prototypes ################# //
// ==== Control Functions ==== //

// =========================== //
// ############################################## //

#endif
