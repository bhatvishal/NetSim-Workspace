#pragma once
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
#ifndef _NETSIM_LTENR_ANTENNAMODEL_H_
#define _NETSIM_LTENR_ANTENNAMODEL_H_
#ifdef  __cplusplus
extern "C" {
#endif

	typedef struct LTENR_ANTENNA
	{
		//Config parameter
		UINT txAntennaCount;
		UINT rxAntennaCount;

		//Only in UE
		UINT uplinkLayerCount;
		UINT downlinkLayerCount;

	}LTENR_ANTENNA, * ptrLTENR_ANTENNA;

	//Function prototype
	void LTENR_CONFIGURE_ANTENNA(ptrLTENR_ANTENNA antenna, void* xmlNetSimNode);
	void LTENR_ANTENNA_SET_LAYER_COUNT(ptrLTENR_ANTENNA gnbAntenna, ptrLTENR_ANTENNA ueAntenna);
	UINT LTENR_ANTENNA_GET_LAYER_COUNT(ptrLTENR_ANTENNA ueAntenna, bool isUplink);
	UINT LTENR_ANTENNA_GET_LAYER_COUNT_FOR_NONASSOCIATED_UE(ptrLTENR_ANTENNA gnBAntenna, ptrLTENR_ANTENNA ueAntenna, bool isUplink);
#define LTENR_PHY_GET_LAYER_COUNT(uePhy,isUplink) LTENR_ANTENNA_GET_LAYER_COUNT(uePhy->antenna,isUplink)
#define LTENR_PHY_GET_DLLAYER_COUNT(uePhy) LTENR_PHY_GET_LAYER_COUNT(uePhy,false)
#define LTENR_PHY_GET_ULLAYER_COUNT(uePhy) LTENR_PHY_GET_LAYER_COUNT(uePhy,true)
#define LTENR_PHY_GET_LAYER_COUNT_FOR_NONASSOCIATED(gnbPhy,uePhy,isUplink) LTENR_ANTENNA_GET_LAYER_COUNT_FOR_NONASSOCIATED_UE(gnbPhy->antenna,uePhy->antenna,isUplink)
#define LTENR_PHY_GET_DLLAYER_COUNT_FOR_NONASSOCIATED(gnbPhy,uePhy) LTENR_PHY_GET_LAYER_COUNT_FOR_NONASSOCIATED(gnbPhy,uePhy,false)
#define LTENR_PHY_GET_ULLAYER_COUNT_FOR_NONASSOCIATED(gnbPhy,uePhy) LTENR_PHY_GET_LAYER_COUNT_FOR_NONASSOCIATED(gnbPhy,uePhy,true)

#ifdef  __cplusplus
}
#endif
#endif //_NETSIM_LTENR_ANTENNAMODEL_H_
