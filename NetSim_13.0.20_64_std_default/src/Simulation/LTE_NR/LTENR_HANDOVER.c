/************************************************************************************
* Copyright (C) 2021																*
* TETCOS, Bangalore. India															*
*																					*
* Tetcos owns the intellectual property rights in the Product and its content.		*
* The copying, redistribution, reselling or publication of any or all of the		*
* Product or its content without express prior written consent of Tetcos is			*
* prohibited. Ownership and / or any other right relating to the software and all	*
* intellectual property rights therein shall remain at all times with Tetcos.		*
*																					*
* This source code is licensed per the NetSim license agreement.					*
*																					*
* No portion of this source code may be used as the basis for a derivative work,	*
* or used, for any purpose other than its intended use per the NetSim license		*
* agreement.																		*
*																					*
* This source code and the algorithms contained within it are confidential trade	*
* secrets of TETCOS and may not be used as the basis for any other software,		*
* hardware, product or service.														*
*																					*
* Author:    Kumar Gaurav	                                                        *
*										                                            *
* ----------------------------------------------------------------------------------*/

#pragma region HEADER_FILES
#include "stdafx.h"
#include "LTENR_GNBRRC.h"
#include "LTENR_NAS.h"
#include "LTENR_PHY.h"
#include "LTENR_EPC.h"
#pragma endregion

#pragma region UE_CONTEXT_RELEASE_ACK
void LTENR_HANDOVER_UE_CONTEXT_RELEASE_ACK(ptrLTENR_HANDOVER_Hdr hdr) {
	hdr->msgType = LTENR_MSG_NAS_HANDOVER_UE_CONTEXT_RELEASE_ACK;
	ptrLTENR_NAS_UECONTEXTRELEASECOMMAND msg = calloc(1, sizeof * msg);
	msg->ueID = hdr->UEID;
	msg->outCome = HANDOVER_OUTCOME_SUCESSFUL;
	msg->cause = HANDOVER_CAUSE_RADIO_NETWORK;
	hdr->msg = msg;

	NETSIM_IPAddress srcIP = DEVICE_NWADDRESS(hdr->serveringCellID,
		fn_NetSim_LTENR_CORE_INTERFACE(hdr->serveringCellID, nGC_INTERFACE_XN));
	NETSIM_IPAddress destIP = DEVICE_NWADDRESS(hdr->targetCellID,
		fn_NetSim_LTENR_CORE_INTERFACE(hdr->targetCellID, nGC_INTERFACE_XN));

	LTENR_Handover_Send_Packet(hdr->serveringCellID, hdr->targetCellID, srcIP, destIP, hdr, sizeof * hdr,
		LTENR_MSG_NAS_HANDOVER_UE_CONTEXT_RELEASE_ACK,
		strLTENR_MSGTYPE[LTENR_MSG_NAS_HANDOVER_UE_CONTEXT_RELEASE_ACK % (MAC_PROTOCOL_LTE_NR * 100)],
		nasUEContextReleaseACK);
}

void LTENR_HANDOVER_UE_CONTEXT_RELEASE_ACK_RECV(ptrLTENR_HANDOVER_Hdr hdr) {
	ptrLTENR_NAS_UECONTEXTRELEASECOMMAND msg = hdr->msg;
	if (msg->outCome != HANDOVER_OUTCOME_SUCESSFUL) {
		fnNetSimError("UE context release outcome is unsucessfull.");
	}
}
#pragma endregion 

#pragma region UE_CONTEXT_RELEASE
void LTENR_HANDOVER_UE_CONTEXT_RELEASE(ptrLTENR_HANDOVER_Hdr hdr) {
	hdr->msgType = LTENR_MSG_NAS_HANDOVER_UE_CONTEXT_RELEASE;
	ptrLTENR_NAS_UECONTEXTRELEASEREQUEST msg = calloc(1, sizeof * msg);
	msg->cause = HANDOVER_CAUSE_RADIO_NETWORK;
	msg->gnb_UE_ID = hdr->targetCellIF;
	msg->mme_UE_ID = hdr->AMFID;
	hdr->msg = msg;

	NETSIM_IPAddress destIP = DEVICE_NWADDRESS(hdr->serveringCellID,
		fn_NetSim_LTENR_CORE_INTERFACE(hdr->serveringCellID, nGC_INTERFACE_XN));
	NETSIM_IPAddress srcIP = DEVICE_NWADDRESS(hdr->targetCellID,
		fn_NetSim_LTENR_CORE_INTERFACE(hdr->targetCellID, nGC_INTERFACE_XN));

	LTENR_Handover_Send_Packet(hdr->targetCellID, hdr->serveringCellID, srcIP, destIP, hdr, sizeof * hdr,
		LTENR_MSG_NAS_HANDOVER_UE_CONTEXT_RELEASE,
		strLTENR_MSGTYPE[LTENR_MSG_NAS_HANDOVER_UE_CONTEXT_RELEASE % (MAC_PROTOCOL_LTE_NR * 100)],
		nasUEContextRelease);
}

void LTENR_HANDOVER_UE_CONTEXT_RELEASE_RECV(ptrLTENR_HANDOVER_Hdr hdr) {
	ptrLTENR_HANDOVER_Hdr msg = calloc(1, sizeof * msg);
	memcpy(msg, hdr, sizeof * msg);
	msg->msg = NULL;
	LTENR_HANDOVER_UE_CONTEXT_RELEASE_ACK(msg);
}
#pragma endregion

#pragma region HANDOVER_RESPONSE
void LTENR_HANDOVER_REQUEST_ACK(ptrLTENR_HANDOVER_Hdr hdr) {
	ptrLTENR_NAS_HANDOVER_REQUEST_ACK msg = calloc(1, sizeof * msg);
	msg->GNB_UE_ID = hdr->serveringCellID;
	msg->MME_UE_ID = hdr->AMFID;
	hdr->msgType = LTENR_MSG_NAS_HANDOVER_REQUEST_ACK;
	msg->outCome = HANDOVER_OUTCOME_SUCESSFUL;
	hdr->msg = msg;

	NETSIM_IPAddress destIP = DEVICE_NWADDRESS(hdr->serveringCellID,
		fn_NetSim_LTENR_CORE_INTERFACE(hdr->serveringCellID, nGC_INTERFACE_XN));
	NETSIM_IPAddress srcIP = DEVICE_NWADDRESS(hdr->targetCellID,
		fn_NetSim_LTENR_CORE_INTERFACE(hdr->targetCellID, nGC_INTERFACE_XN));

	LTENR_Handover_Send_Packet(hdr->targetCellID, hdr->serveringCellID, srcIP, destIP, hdr, sizeof * hdr,
		LTENR_MSG_NAS_HANDOVER_REQUEST_ACK,
		strLTENR_MSGTYPE[LTENR_MSG_NAS_HANDOVER_REQUEST_ACK % (MAC_PROTOCOL_LTE_NR * 100)],
		nasHandoverRequestAck);
}

void LTENR_HANDOVER_REQUEST_ACK_RECV(ptrLTENR_HANDOVER_Hdr hdr) {
	ptrLTENR_HANDOVER_Hdr msg = calloc(1, sizeof * msg);
	memcpy(msg, hdr, sizeof * msg);
	msg->msg = NULL;
	fn_NetSim_LTENR_NAS_HANDOVER_COMMAND(msg);
}
#pragma endregion

#pragma region HANDOVER_REQUEST
void LTENR_HANDOVER_REQUEST(ptrLTENR_HANDOVER_Hdr hdr) {

	ptrLTENR_NAS_HANDOVER_REQUEST msg = calloc(1, sizeof * msg);
	msg->MME_UE_ID = hdr->AMFID;
	msg->type = HANDOVER_TYPE_INTRA_LTENR;
	msg->cause = HANDOVER_CAUSE_RADIO_NETWORK;
	msg->targetID = hdr->targetCellID;
	msg->lastVisitedCellID = hdr->serveringCellID;
	msg->LastVisitedCellType = CELL_TYPE_LARGE;
	hdr->msg = msg;
	hdr->msgType = LTENR_MSG_NAS_HANDOVER_REQUEST;

	NETSIM_IPAddress srcIP = DEVICE_NWADDRESS(hdr->serveringCellID,
		fn_NetSim_LTENR_CORE_INTERFACE(hdr->serveringCellID, nGC_INTERFACE_XN));
	NETSIM_IPAddress destIP = DEVICE_NWADDRESS(hdr->targetCellID,
		fn_NetSim_LTENR_CORE_INTERFACE(hdr->targetCellID, nGC_INTERFACE_XN));

	LTENR_Handover_Send_Packet(hdr->serveringCellID, hdr->targetCellID, srcIP, destIP, hdr, sizeof * hdr,
		LTENR_MSG_NAS_HANDOVER_REQUEST,
		strLTENR_MSGTYPE[LTENR_MSG_NAS_HANDOVER_REQUEST % (MAC_PROTOCOL_LTE_NR * 100)],
		nasHandoverRequest);
	
}

void LTENR_HANDOVER_REQUEST_RECV(ptrLTENR_HANDOVER_Hdr hdr) {
	ptrLTENR_HANDOVER_Hdr msg = calloc(1, sizeof * msg);
	memcpy(msg, hdr, sizeof * msg);
	msg->msg = NULL;
	LTENR_HANDOVER_REQUEST_ACK(msg);
}
#pragma endregion

#pragma region PACKET_RECV_HANDLER
void fn_NetSim_LTENR_Handover_RECV() {
	NetSim_PACKET* packet = pstruEventDetails->pPacket;
	ptrLTENR_HANDOVER_Hdr hdr = NULL;
	switch (packet->nControlDataType) {
	case LTENR_MSG_NAS_HANDOVER_REQUEST:
		hdr = LTENR_HANDOVER_HDR_GET_FROM_PACKET(packet, nasHandoverRequest);
		LTENR_HANDOVER_REQUEST_RECV(hdr);
		LTENR_HANDOVER_HDR_FREE_FROM_PACKET(packet, nasHandoverRequest);
		break;
	case LTENR_MSG_NAS_HANDOVER_REQUEST_ACK:
		hdr = LTENR_HANDOVER_HDR_GET_FROM_PACKET(packet, nasHandoverRequestAck);
		LTENR_HANDOVER_REQUEST_ACK_RECV(hdr);
		LTENR_HANDOVER_HDR_FREE_FROM_PACKET(packet, nasHandoverRequestAck);
		break;
	case LTENR_MSG_NAS_HANDOVER_UE_CONTEXT_RELEASE:
		hdr = LTENR_HANDOVER_HDR_GET_FROM_PACKET(packet, nasUEContextRelease);
		LTENR_HANDOVER_UE_CONTEXT_RELEASE_RECV(hdr);
		LTENR_HANDOVER_HDR_FREE_FROM_PACKET(packet, nasUEContextRelease);
		break;
	case LTENR_MSG_NAS_HANDOVER_UE_CONTEXT_RELEASE_ACK:
		hdr = LTENR_HANDOVER_HDR_GET_FROM_PACKET(packet, nasUEContextReleaseACK);
		LTENR_HANDOVER_UE_CONTEXT_RELEASE_ACK_RECV(hdr);
		LTENR_HANDOVER_HDR_FREE_FROM_PACKET(packet, nasUEContextReleaseACK);
		break;
	default:
		break;
	}
}
#pragma endregion