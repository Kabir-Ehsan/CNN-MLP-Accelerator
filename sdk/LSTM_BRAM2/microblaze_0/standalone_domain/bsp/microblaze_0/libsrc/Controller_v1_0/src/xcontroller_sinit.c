// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xcontroller.h"

extern XController_Config XController_ConfigTable[];

XController_Config *XController_LookupConfig(u16 DeviceId) {
	XController_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XCONTROLLER_NUM_INSTANCES; Index++) {
		if (XController_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XController_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XController_Initialize(XController *InstancePtr, u16 DeviceId) {
	XController_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XController_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XController_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

