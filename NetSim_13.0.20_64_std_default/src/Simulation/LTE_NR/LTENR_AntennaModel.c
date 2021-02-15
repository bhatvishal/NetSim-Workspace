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
* Author:    Shashi Kant Suman	                                                    *
*										                                            *
* ----------------------------------------------------------------------------------*/

#pragma region HEADER_FILES
#include "stdafx.h"
#include "LTENR_AntennaModel.h"
#pragma endregion
#define LTENR_TX_ANTENNA_COUNT_DEFAULT		1
#define LTENR_RX_ANTENNA_COUNT_DEFAULT		1

void LTENR_CONFIGURE_ANTENNA(ptrLTENR_ANTENNA antenna, void* xmlNetSimNode)
{
	void* xmlChild = fn_NetSim_xmlGetFirstChildElement(xmlNetSimNode, "ANTENNA");
	getXmlVar(&antenna->txAntennaCount, TX_ANTENNA_COUNT, xmlChild, 1, _UINT, LTENR);
	getXmlVar(&antenna->rxAntennaCount, RX_ANTENNA_COUNT, xmlChild, 1, _UINT, LTENR);
}

void LTENR_ANTENNA_SET_LAYER_COUNT(ptrLTENR_ANTENNA gnbAntenna, ptrLTENR_ANTENNA ueAntenna)
{
	if (gnbAntenna->txAntennaCount < ueAntenna->rxAntennaCount ||
		gnbAntenna->rxAntennaCount < ueAntenna->txAntennaCount)
	{
		fnNetSimError("gNB antenna count is less than the UE antenna count.\n"
					  "Please check the Antenna configuration in both gNB and UE.\n");
	}
	ueAntenna->downlinkLayerCount = min(gnbAntenna->txAntennaCount, ueAntenna->rxAntennaCount);
	ueAntenna->uplinkLayerCount = min(gnbAntenna->rxAntennaCount, ueAntenna->txAntennaCount);
}

UINT LTENR_ANTENNA_GET_LAYER_COUNT(ptrLTENR_ANTENNA ueAntenna, bool isUplink)
{
	if (isUplink) return ueAntenna->uplinkLayerCount;
	else return ueAntenna->downlinkLayerCount;
}

UINT LTENR_ANTENNA_GET_LAYER_COUNT_FOR_NONASSOCIATED_UE(ptrLTENR_ANTENNA gnBAntenna, ptrLTENR_ANTENNA ueAntenna, bool isUplink)
{
	if (isUplink)
		return min(gnBAntenna->rxAntennaCount, ueAntenna->txAntennaCount);
	else
		return min(gnBAntenna->txAntennaCount, ueAntenna->rxAntennaCount);
}
