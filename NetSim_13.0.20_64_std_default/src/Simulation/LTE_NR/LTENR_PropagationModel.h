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
* Author:    Dilip Kumar Kundrapu													*
* Standard:  TR 38.901(Release 15)													*
* Updated:   27th June, 2019											            *
* ----------------------------------------------------------------------------------*/

#ifndef _NETSIM_LTENR_PROPAGATIONMODEL_H_
#define _NETSIM_LTENR_PROPAGATIONMODEL_H_
#ifdef  __cplusplus
extern "C" {
#endif

#pragma region LTENR_LOCATION
	typedef enum
	{
		LTENR_SCENARIO_RMA,
		LTENR_SCENARIO_UMA,
		LTENR_SCENARIO_UMI,
		LTENR_SCENARIO_INH,
	}LTENR_SCENARIO;
	static const char* strLTENR_SCENARIO[] =
	{ "RURAL_MACRO","URBAN_MACRO","URBAN_MICRO","INDOOR_OFFICE" };

	typedef enum
	{
		LTENR_LOCATION_OUTDOOR,
		LTENR_LOCATION_INDOOR,
	}LTENR_LOCATION;

	typedef enum
	{
		LTENR_INH_MIXED_OFFICE,
		LTENR_INH_OPEN_OFFICE,
	}LTENR_INH_TYPE;
	static const char* strLTENR_INH_TYPE[] =
	{ "MIXED_OFFICE","OPEN_OFFICE" };

	typedef enum
	{
		LTENR_STATE_LOS,
		LTENR_STATE_NLOS,
	}LTENR_LOS_NLOS_STATE;
	static const char* strLTENR_STATE[] =
	{ "LOS","NLOS" };

	typedef enum
	{
		LTENR_LOS_MODE_TR38_901,
		LTENR_LOS_MODE_USER_DEFINED,
	}LTENR_LOS_MODE;
	static const char* strLTENR_LOS_MODE[] =
	{ "3GPPTR38.901-Table7.4.2-1", "USER_DEFINED" };
#pragma endregion

#pragma region LTENR_PROPAGATION_MODEL_ENUM
	typedef enum
	{
		LTENR_PATHLOSS_MODEL_NONE,
		LTENR_PATHLOSS_MODEL_3GPP38_901_7_4_1,
		LTENR_PATHLOSS_MODEL_MATLAB,
		LTENR_PATHLOSS_MODEL_FILEBASED,
	}LTENR_PATHLOSS_MODEL;
	static const char* strLTENR_PATHLOSS_MODEL[] =
	{"NONE","3GPPTR38.901-7.4.1","Matlab","File"};

	typedef enum
	{
		LTENR_SHADOWFADING_MODEL_NONE,
		LTENR_SHADOWFADING_MODEL_LOGNORMAL,
		LTENR_SHADOWFADING_MODEL_MATLAB,
		LTENR_SHADOWFADING_MODEL_FILEBASED,
	}LTENR_SHADOWFADING_MODEL;
	static const char* strLTENR_SHADOWFADING_MODEL[] =
	{ "NONE","LOG_NORMAL","Matlab","File" };

	typedef enum
	{
		LTENR_FASTFADING_MODEL_NONE,
		LTENR_FASTFADING_MODEL_AWGN_WITH_RAYLEIGH_FADING,
		LTENR_FASTFADING_MODEL_MATLAB,
		LTENR_FASTFADING_MODEL_FILEBASED,
	}LTENR_FASTFADING_MODEL;
	static const char* strLTENR_FASTFADING_MODEL[] =
	{ "NONE","AWGN_WITH_RAYLEIGH_FADING","Matlab","File" };

	typedef enum
	{
		LTENR_O2IBUILDINGPENETRATION_MODEL_NONE,
		LTENR_O2IBUILDINGPENETRATION_MODEL_LOW_LOSS,
		LTENR_O2IBUILDINGPENETRATION_MODEL_HIGH_LOSS,
		LTENR_O2IBUILDINGPENETRATION_MODEL_MATLAB,
		LTENR_O2IBUILDINGPENETRATION_MODEL_FILEBASED,
	}LTENR_O2IBUILDINGPENETRATION_MODEL;
	static const char* strLTENR_O2IBUILDINGPENETRATION_MODEL[] =
	{ "NONE","LOW_LOSS_MODEL","HIGH_LOSS_MODEL","MATLAB","FILEBAED" };
#pragma endregion

#pragma region PROPAGATION_CONFIG
	typedef struct stru_LTENR_PropagationConfig
	{
		bool isPathLossEnabled;
		bool isShadowFadingEnabled;
		bool isFastFadingEnabled;
		bool isO2IBuildingPenetrationLossEnabled;

		LTENR_PATHLOSS_MODEL pathLossModel;
		LTENR_SHADOWFADING_MODEL shadowFadingModel;
		LTENR_FASTFADING_MODEL fastFadingModel;
		LTENR_O2IBUILDINGPENETRATION_MODEL o2iBuildingPenetrationModel;

		/// In Case of Matlab model
		char* matlabPathLossFileName;
		char* matlabShadowFadingFileName;
		char* matlabFastFadingFileName;
		char* matlabO2IBuildingPenetrationFileName;

		/// In case of file based model
		char* pathLossFileName;
		char* shadowFadingFileName;
		char* fastFadingFileName;
		char* O2IBuildingPenetrationFileName;
		
		/// For 3GPP TR38.901-7.4.1 Based Path loss
		LTENR_SCENARIO outScenario;
		LTENR_SCENARIO inScenario;
		LTENR_INH_TYPE indoor_type;
		LTENR_LOS_MODE los_mode;
		LTENR_LOS_NLOS_STATE state;
		double los_probability;
		double gNB_height;
		double UE_height;
		double buildings_height;
		double street_width;

		/// For Log normal shadowfading model
		char* shadowFadingStdDeviationModel;

		/// For AWGN-RAYLEIGH Fading
		double coherenceTime;

		/// Transmitter power
		double txPower_dbm;
		double txPower_mw;
	}LTENR_PROPAGATIONCONFIG, * ptrLTENR_PROPAGATIONCONFIG;
#pragma endregion

#ifdef  __cplusplus
}
#endif
#endif /* _NETSIM_LTENR_PROPAGATIONMODEL_H_ */
