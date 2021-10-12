// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xcontroller.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XController_CfgInitialize(XController *InstancePtr, XController_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Bus_bundle_BaseAddress = ConfigPtr->Bus_bundle_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XController_Start(XController *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XController_ReadReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_AP_CTRL) & 0x80;
    XController_WriteReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_AP_CTRL, Data | 0x01);
}

u32 XController_IsDone(XController *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XController_ReadReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XController_IsIdle(XController *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XController_ReadReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XController_IsReady(XController *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XController_ReadReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XController_EnableAutoRestart(XController *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XController_WriteReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_AP_CTRL, 0x80);
}

void XController_DisableAutoRestart(XController *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XController_WriteReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_AP_CTRL, 0);
}

void XController_Set_new_net(XController *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XController_WriteReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_NEW_NET_DATA, Data);
}

u32 XController_Get_new_net(XController *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XController_ReadReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_NEW_NET_DATA);
    return Data;
}

void XController_InterruptGlobalEnable(XController *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XController_WriteReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_GIE, 1);
}

void XController_InterruptGlobalDisable(XController *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XController_WriteReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_GIE, 0);
}

void XController_InterruptEnable(XController *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XController_ReadReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_IER);
    XController_WriteReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_IER, Register | Mask);
}

void XController_InterruptDisable(XController *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XController_ReadReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_IER);
    XController_WriteReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_IER, Register & (~Mask));
}

void XController_InterruptClear(XController *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XController_WriteReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_ISR, Mask);
}

u32 XController_InterruptGetEnabled(XController *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XController_ReadReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_IER);
}

u32 XController_InterruptGetStatus(XController *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XController_ReadReg(InstancePtr->Bus_bundle_BaseAddress, XCONTROLLER_BUS_BUNDLE_ADDR_ISR);
}

