// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// bus_bundle_configur
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read)
//        bit 7  - auto_restart (Read/Write)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0  - Channel 0 (ap_done)
//        bit 1  - Channel 1 (ap_ready)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0  - Channel 0 (ap_done)
//        bit 1  - Channel 1 (ap_ready)
//        others - reserved
// 0x10 : Data signal of Config_Ninputs_V
//        bit 15~0 - Config_Ninputs_V[15:0] (Read/Write)
//        others   - reserved
// 0x14 : reserved
// 0x18 : Data signal of Config_Noutputs_V
//        bit 15~0 - Config_Noutputs_V[15:0] (Read/Write)
//        others   - reserved
// 0x1c : reserved
// 0x20 : Data signal of Config_NnuronsH1_V
//        bit 15~0 - Config_NnuronsH1_V[15:0] (Read/Write)
//        others   - reserved
// 0x24 : reserved
// 0x28 : Data signal of Config_NnuronsH2_V
//        bit 15~0 - Config_NnuronsH2_V[15:0] (Read/Write)
//        others   - reserved
// 0x2c : reserved
// 0x30 : Data signal of Config_NnuronsH3_V
//        bit 15~0 - Config_NnuronsH3_V[15:0] (Read/Write)
//        others   - reserved
// 0x34 : reserved
// 0x38 : Data signal of Config_NnuronsH4_V
//        bit 15~0 - Config_NnuronsH4_V[15:0] (Read/Write)
//        others   - reserved
// 0x3c : reserved
// 0x40 : Data signal of Config_convOut_V
//        bit 15~0 - Config_convOut_V[15:0] (Read/Write)
//        others   - reserved
// 0x44 : reserved
// 0x48 : Data signal of Config_Nlayers_V
//        bit 7~0 - Config_Nlayers_V[7:0] (Read/Write)
//        others  - reserved
// 0x4c : reserved
// 0x50 : Data signal of Config_imageSize_V
//        bit 7~0 - Config_imageSize_V[7:0] (Read/Write)
//        others  - reserved
// 0x54 : reserved
// 0x58 : Data signal of Config_KernelSize_V
//        bit 7~0 - Config_KernelSize_V[7:0] (Read/Write)
//        others  - reserved
// 0x5c : reserved
// 0x60 : Data signal of Config_channels_V
//        bit 7~0 - Config_channels_V[7:0] (Read/Write)
//        others  - reserved
// 0x64 : reserved
// 0x68 : Data signal of Config_stride_V
//        bit 7~0 - Config_stride_V[7:0] (Read/Write)
//        others  - reserved
// 0x6c : reserved
// 0x70 : Data signal of Config_padding_V
//        bit 7~0 - Config_padding_V[7:0] (Read/Write)
//        others  - reserved
// 0x74 : reserved
// 0x78 : Data signal of Config_KSIZE_V
//        bit 7~0 - Config_KSIZE_V[7:0] (Read/Write)
//        others  - reserved
// 0x7c : reserved
// 0x80 : Data signal of Config_convOut1D_V
//        bit 7~0 - Config_convOut1D_V[7:0] (Read/Write)
//        others  - reserved
// 0x84 : reserved
// 0x88 : Data signal of new_net
//        bit 31~0 - new_net[31:0] (Read/Write)
// 0x8c : reserved
// 0x90 : Data signal of conv
//        bit 31~0 - conv[31:0] (Read/Write)
// 0x94 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_AP_CTRL                  0x00
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_GIE                      0x04
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_IER                      0x08
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_ISR                      0x0c
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_NINPUTS_V_DATA    0x10
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_NINPUTS_V_DATA    16
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_NOUTPUTS_V_DATA   0x18
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_NOUTPUTS_V_DATA   16
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_NNURONSH1_V_DATA  0x20
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_NNURONSH1_V_DATA  16
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_NNURONSH2_V_DATA  0x28
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_NNURONSH2_V_DATA  16
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_NNURONSH3_V_DATA  0x30
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_NNURONSH3_V_DATA  16
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_NNURONSH4_V_DATA  0x38
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_NNURONSH4_V_DATA  16
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_CONVOUT_V_DATA    0x40
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_CONVOUT_V_DATA    16
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_NLAYERS_V_DATA    0x48
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_NLAYERS_V_DATA    8
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_IMAGESIZE_V_DATA  0x50
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_IMAGESIZE_V_DATA  8
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_KERNELSIZE_V_DATA 0x58
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_KERNELSIZE_V_DATA 8
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_CHANNELS_V_DATA   0x60
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_CHANNELS_V_DATA   8
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_STRIDE_V_DATA     0x68
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_STRIDE_V_DATA     8
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_PADDING_V_DATA    0x70
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_PADDING_V_DATA    8
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_KSIZE_V_DATA      0x78
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_KSIZE_V_DATA      8
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONFIG_CONVOUT1D_V_DATA  0x80
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONFIG_CONVOUT1D_V_DATA  8
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_NEW_NET_DATA             0x88
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_NEW_NET_DATA             32
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_ADDR_CONV_DATA                0x90
#define XCONTROLLER_BUS_BUNDLE_CONFIGUR_BITS_CONV_DATA                32

