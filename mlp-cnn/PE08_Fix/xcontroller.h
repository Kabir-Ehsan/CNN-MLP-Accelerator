// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XCONTROLLER_H
#define XCONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xcontroller_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Bus_bundle_configur_BaseAddress;
} XController_Config;
#endif

typedef struct {
    u32 Bus_bundle_configur_BaseAddress;
    u32 IsReady;
} XController;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XController_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XController_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XController_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XController_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XController_Initialize(XController *InstancePtr, u16 DeviceId);
XController_Config* XController_LookupConfig(u16 DeviceId);
int XController_CfgInitialize(XController *InstancePtr, XController_Config *ConfigPtr);
#else
int XController_Initialize(XController *InstancePtr, const char* InstanceName);
int XController_Release(XController *InstancePtr);
#endif

void XController_Start(XController *InstancePtr);
u32 XController_IsDone(XController *InstancePtr);
u32 XController_IsIdle(XController *InstancePtr);
u32 XController_IsReady(XController *InstancePtr);
void XController_EnableAutoRestart(XController *InstancePtr);
void XController_DisableAutoRestart(XController *InstancePtr);

void XController_Set_Config_Ninputs_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_Ninputs_V(XController *InstancePtr);
void XController_Set_Config_Noutputs_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_Noutputs_V(XController *InstancePtr);
void XController_Set_Config_NnuronsH1_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_NnuronsH1_V(XController *InstancePtr);
void XController_Set_Config_NnuronsH2_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_NnuronsH2_V(XController *InstancePtr);
void XController_Set_Config_NnuronsH3_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_NnuronsH3_V(XController *InstancePtr);
void XController_Set_Config_NnuronsH4_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_NnuronsH4_V(XController *InstancePtr);
void XController_Set_Config_convOut_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_convOut_V(XController *InstancePtr);
void XController_Set_Config_Nlayers_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_Nlayers_V(XController *InstancePtr);
void XController_Set_Config_imageSize_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_imageSize_V(XController *InstancePtr);
void XController_Set_Config_KernelSize_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_KernelSize_V(XController *InstancePtr);
void XController_Set_Config_channels_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_channels_V(XController *InstancePtr);
void XController_Set_Config_stride_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_stride_V(XController *InstancePtr);
void XController_Set_Config_padding_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_padding_V(XController *InstancePtr);
void XController_Set_Config_KSIZE_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_KSIZE_V(XController *InstancePtr);
void XController_Set_Config_convOut1D_V(XController *InstancePtr, u32 Data);
u32 XController_Get_Config_convOut1D_V(XController *InstancePtr);
void XController_Set_new_net(XController *InstancePtr, u32 Data);
u32 XController_Get_new_net(XController *InstancePtr);
void XController_Set_conv(XController *InstancePtr, u32 Data);
u32 XController_Get_conv(XController *InstancePtr);

void XController_InterruptGlobalEnable(XController *InstancePtr);
void XController_InterruptGlobalDisable(XController *InstancePtr);
void XController_InterruptEnable(XController *InstancePtr, u32 Mask);
void XController_InterruptDisable(XController *InstancePtr, u32 Mask);
void XController_InterruptClear(XController *InstancePtr, u32 Mask);
u32 XController_InterruptGetEnabled(XController *InstancePtr);
u32 XController_InterruptGetStatus(XController *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
