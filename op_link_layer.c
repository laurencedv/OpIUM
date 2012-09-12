/*!
 @file		op_link_layer.c
 @brief		Layer 1 - Link of the OpIUM protocol

 @version	0.1
 @note		
 @todo		

 @date		April 18th 2012
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "op_link_layer.h"
// ############################################## //


// ################## Variables ################# //
extern U32 heapAvailable;

// Link Layer buffers
tLinkControl linkControl[COM_WING_NB];


// ############################################## //


// ############# Internal Functions ############# //

// ############################################## //


// ############### LINK Functions ############### //
// ==== Buffer Functions ==== //
/**
* \fn		U8 opLinkPacketBufCreate(U16 packetID, U16 packetSize)
* @brief	Create a buffer for a new OpIUM packet in the heap
* @note
* @arg		U16 packetID				Packet ID of the new packet
* @arg		U16 packetSize				Total size of the new packet buffer
* @return	U8 errorCode				STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 opLinkPacketBufCreate(U16 packetID, U16 packetSize)
{
	// -- Compute the total size -- //
	// ---------------------------- //

	// -- Allocate the buffer -- //
	// ------------------------- //
}

/**
* \fn		U8 opLinkPacketBufRemove(U16 packetID)
* @brief	Remove a buffer of an OpIUM packet from the heap
* @note
* @arg		U16 packetID				Packet ID to be removed
* @return	U8 errorCode				STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 opLinkPacketBufRemove(U16 packetID)
{

}
// ========================== //

// ==== Control Functions ==== //
//Detect/identify COM wing

//Startup COM wing

// =========================== //
// ############################################## //
