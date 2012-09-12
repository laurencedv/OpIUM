/*!
 @file		op_link_layer.h
 @brief		Layer 1 - Link of the OpIUM protocol

 @version	0.1
 @note
 @todo

 @date		April 18th 2012
 @author	Laurence DV
*/


#ifndef _OP_LINK_LAYER_H
#define _OP_LINK_LAYER_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Lib
#include <protocol/opium/op_general.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //



// ############################################## //


// ################# Data Type ################## //


// Link Layer control
typedef struct
{
	tCOMWingState	linkState;
	tCOMWingType	linkType;
	
	U32 * linkBufferIn;
	U32 * linkBufferOut;

}tLinkControl;
// ############################################## //


// ################# Prototypes ################# //
// ==== Control Functions ==== //

// =========================== //
// ############################################## //

#endif
