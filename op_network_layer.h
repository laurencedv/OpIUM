/*!
 @file		op_network_layer.h
 @brief		Layer 2 - Network of the OpIUM protocol

 @version	0.1
 @note
 @todo

 @date		April 19th 2012
 @author	Laurence DV
*/


#ifndef _OP_NETWORK_LAYER_H
#define _OP_NETWORK_LAYER_H 1

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
typedef union
{
	U32 all;
	struct
	{
		U8 mode;
		
	};
}tOpNetworkHeader;

// ############################################## //


// ################# Prototypes ################# //
// ==== Control Functions ==== //

// =========================== //
// ############################################## //

#endif
