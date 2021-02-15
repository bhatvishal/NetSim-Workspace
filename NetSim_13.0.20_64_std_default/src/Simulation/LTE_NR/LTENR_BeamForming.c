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
#include "LTENR_PHY.h"
#include "LTENR_AntennaModel.h"
#pragma endregion

#define MAX_FASTFADING_ENTRY 50000
typedef struct stru_fastfadinghandle
{
	char fileName[BUFSIZ];
	FILE* file;

	UINT txAntennaCount;
	UINT rxAntennaCount;

	UINT entryCount;
	double** entries;
}LTENR_FASTFADINGHANDLE, * ptrLTENR_FASTFADINGHANDLE;
static ptrLTENR_FASTFADINGHANDLE* fastFadingHandles = NULL;
static UINT fastFadingHandleCount = 0;

static ptrLTENR_FASTFADINGHANDLE fastfading_find_handle(UINT txAntennaCount, UINT rxAntennaCount)
{
	if (fastFadingHandleCount == 0) return NULL;
	for (UINT i = 0; i < fastFadingHandleCount; i++)
	{
		if ((fastFadingHandles[i]->txAntennaCount == 0 || fastFadingHandles[i]->txAntennaCount == txAntennaCount) &&
			(fastFadingHandles[i]->rxAntennaCount == 0 || fastFadingHandles[i]->rxAntennaCount == rxAntennaCount))
			return fastFadingHandles[i];
	}
	return NULL;
}

static bool read_fastfading_file(ptrLTENR_FASTFADINGHANDLE handle, UINT layerCount)
{
	handle->file = fopen(handle->fileName, "r");
	if (!handle->file)
	{
		fnSystemError("Unable to open %s file.", handle->fileName);
		perror(handle->fileName);
		return false;
	}

	char s[5000];
	while (fgets(s, 5000, handle->file))
	{
		char* t;
		for (UINT i = 0; i < layerCount; i++)
		{
			if (handle->entries[i])
				handle->entries[i] = realloc(handle->entries[i], ((size_t)handle->entryCount + 1) * (sizeof * handle->entries[i]));
			else
				handle->entries[i] = calloc(1, sizeof * handle->entries[i]);

			if (i == 0) t = strtok(s, ",");
			else t = strtok(NULL,",");
			handle->entries[i][handle->entryCount] = atof(t);
		}
		handle->entryCount++;
	}
	fclose(handle->file);
	return true;
}

static bool fastfading_open_handle(ptrLTENR_FASTFADINGHANDLE handle)
{
	sprintf(handle->fileName, "%s/5GBeamforming/%d_%d.beamforming",
			pszAppPath,
			handle->txAntennaCount,
			handle->rxAntennaCount);

	UINT layerCount = min(handle->txAntennaCount, handle->rxAntennaCount);
	handle->entries = calloc(layerCount, sizeof * handle->entries);

	if (read_fastfading_file(handle, layerCount))
	{
		return true;
	}
	else
	{
		free(handle->entries);
		return false;
	}
}

static ptrLTENR_FASTFADINGHANDLE fastfading_init_handle(UINT txAntennaCount, UINT rxAntennaCount)
{
	ptrLTENR_FASTFADINGHANDLE handle = fastfading_find_handle(txAntennaCount, rxAntennaCount);
	if (handle) return handle;

	handle = calloc(1, sizeof * handle);
	handle->rxAntennaCount = rxAntennaCount;
	handle->txAntennaCount = txAntennaCount;

	if (fastfading_open_handle(handle))
	{
		if (fastFadingHandleCount == 0)
			fastFadingHandles = calloc(1, sizeof * fastFadingHandles);
		else
			fastFadingHandles = realloc(fastFadingHandles, ((size_t)fastFadingHandleCount + 1) * (sizeof * fastFadingHandles));
		fastFadingHandles[fastFadingHandleCount] = handle;
		fastFadingHandleCount++;
		return handle;
	}
	else
	{
		free(handle);
		return NULL;
	}
}

double LTENR_BeamForming_GetValue(ptrLTENR_FASTFADINGHANDLE handle, UINT layerId, UINT* index)
{
	double val = handle->entries[layerId][*index];
	*index++;
	return val;
}

static void LTENR_BeamFormingGain_Update(ptrLTENR_ASSOCIATEDUEPHYINFO info)
{
	for (NETSIM_ID c = 0; c < MAX_CA_COUNT; c++)
	{
		ptrLTENR_PROPAGATIONINFO propagation = info->propagationInfo[c];
		if (propagation)
		{
			for (UINT l = 0; l < propagation->downlink.layerCount; l++)
				propagation->downlink.beamFormingGain[l] = LTENR_BeamForming_GetValue(propagation->downlink.beamformingHandle,
																					  l,
																					  &propagation->downlink.beamFormingIndex[l]);

			for (UINT l = 0; l < propagation->uplink.layerCount; l++)
				propagation->uplink.beamFormingGain[l] = LTENR_BeamForming_GetValue(propagation->uplink.beamformingHandle,
																					l,
																					&propagation->uplink.beamFormingIndex[l]);
		}
		else break;
	}
}

void LTENR_BeamForming_HandleCoherenceTimer()
{
	NETSIM_ID d = pstruEventDetails->nDeviceId;
	NETSIM_ID in = pstruEventDetails->nInterfaceId;
	ptrLTENR_GNBPHY phy = LTENR_GNBPHY_GET(d, in);

	pstruEventDetails->dEventTime += phy->propagationConfig->coherenceTime;
	fnpAddEvent(pstruEventDetails);
	pstruEventDetails->dEventTime -= phy->propagationConfig->coherenceTime;

	ptrLTENR_ASSOCIATEDUEPHYINFO info = phy->associatedUEPhyInfo;
	while (info)
	{
		LTENR_BeamFormingGain_Update(info);
		LTENR_ASSOCIATEDUEPHYINFO_NEXT(info);
	}
}

void LTENR_BeamForming_InitCoherenceTimer(ptrLTENR_GNBPHY phy)
{
	static bool isCalled = false;
	if (!isCalled)
	{
		LTENR_SUBEVENT_REGISTER(LTENR_SUBEVENT_FASTFADING_COHERENCE_TIMER, "FastFading_Coherence_Timer", LTENR_BeamForming_HandleCoherenceTimer);
		isCalled = true;
	}

	ptrLTENR_PROPAGATIONCONFIG propagation = phy->propagationConfig;
	if (propagation->fastFadingModel != LTENR_FASTFADING_MODEL_AWGN_WITH_RAYLEIGH_FADING) return;
	
	NetSim_EVENTDETAILS pevent;
	memset(&pevent, 0, sizeof pevent);
	pevent.nDeviceId = phy->gnbId;
	pevent.nDeviceType = DEVICE_TYPE(phy->gnbId);
	pevent.nEventType = TIMER_EVENT;
	pevent.nInterfaceId = phy->gnbIf;
	pevent.nProtocolId = MAC_PROTOCOL_LTE_NR;
	pevent.nSubEventType = LTENR_SUBEVENT_FASTFADING_COHERENCE_TIMER;
	fnpAddEvent(&pevent);
}

static ptrLTENR_FASTFADINGHANDLE beamforming_init(UINT tx,UINT rx,UINT** ppindex, double** ppgain)
{
	ptrLTENR_FASTFADINGHANDLE handle = fastfading_init_handle(tx, rx);
	UINT layer = min(tx, rx);
	UINT* index = calloc(layer, sizeof * index);
	double* gain = calloc(layer, sizeof * gain);
	for (UINT i = 0; i < layer; i++)
	{
		index[i] = NETSIM_RAND_RN(handle->entryCount, 0);
		gain[i] = LTENR_BeamForming_GetValue(handle, i, &index[i]);
	}

	*ppindex = index;
	*ppgain = gain;
	return handle;
}

void LTENR_BeamForming_Init(ptrLTENR_PROPAGATIONINFO propagation)
{
	propagation->downlink.beamformingHandle = beamforming_init(propagation->downlink.txAntennaCount,
															   propagation->downlink.rxAntennaCount,
															   &propagation->downlink.beamFormingIndex,
															   &propagation->downlink.beamFormingGain);

	propagation->uplink.beamformingHandle = beamforming_init(propagation->uplink.txAntennaCount,
															 propagation->uplink.rxAntennaCount,
															   &propagation->uplink.beamFormingIndex,
															   &propagation->uplink.beamFormingGain);
}
