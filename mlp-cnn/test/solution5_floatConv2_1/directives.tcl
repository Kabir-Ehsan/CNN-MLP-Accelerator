############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_interface -mode axis -register -register_mode both "Controller" in_stream
set_directive_interface -mode axis -register -register_mode both "Controller" out_stream
set_directive_interface -mode s_axilite -bundle bus_bundle_configur "Controller" Config
set_directive_interface -mode s_axilite -bundle bus_bundle_configur "Controller" new_net
set_directive_interface -mode s_axilite -bundle bus_bundle_configur "Controller" conv
set_directive_pipeline "Controller/INPUTS"
set_directive_pipeline "Controller/MAXW"
set_directive_pipeline "Controller/WEIGHTS"
set_directive_pipeline "Controller/PartitionL"
set_directive_pipeline "Controller/KERNELS"
set_directive_pipeline "Controller/INPUTS2"
set_directive_pipeline -II 4 "Controller/J1"
set_directive_pipeline -II 2 "Controller/Ex1"
set_directive_pipeline -II 2 "Controller/Ex2"
set_directive_pipeline -II 2 "Controller/Ex3"
set_directive_pipeline -II 2 "Controller/Ex4"
set_directive_pipeline -II 3 "Controller/Ex5"
set_directive_pipeline -II 3 "Controller/Ex6"
set_directive_pipeline -II 4 "Controller/Ex7"
set_directive_pipeline -II 5 "Controller/K2"
set_directive_pipeline -II 4 "Controller/J3"
set_directive_pipeline -II 2 "Controller/Ex8"
set_directive_pipeline -II 2 "Controller/Ex9"
set_directive_pipeline -II 2 "Controller/Ex10"
set_directive_pipeline -II 2 "Controller/Ex11"
set_directive_pipeline -II 3 "Controller/Ex12"
set_directive_pipeline -II 3 "Controller/Ex13"
set_directive_pipeline -II 4 "Controller/Ex14"
set_directive_pipeline -II 8 "Controller/K4"
set_directive_pipeline -II 4 "PE0_Unit/PE0_Unit"
set_directive_pipeline -II 4 "PE1_Unit/PE1_Unit"
set_directive_pipeline -II 4 "PE2_Unit/PE2_Unit"
set_directive_pipeline -II 4 "PE3_Unit/PE3_Unit"
set_directive_pipeline -II 4 "PE4_Unit/PE4_Unit"
set_directive_pipeline -II 4 "PE5_Unit/PE5_Unit"
set_directive_pipeline -II 4 "PE6_Unit/PE6_Unit"
set_directive_pipeline -II 4 "PE7_Unit/PE7_Unit"
set_directive_pipeline -II 21 "Controller/copy1"
set_directive_resource -core RAM_2P_BRAM "Controller" weights
set_directive_resource -core RAM_2P_BRAM "Controller" kernels
set_directive_resource -core RAM_2P_BRAM "Controller" inputs
set_directive_pipeline -II 4 "Controller/ExC7"
set_directive_pipeline -II 3 "Controller/ExC6"
set_directive_pipeline -II 3 "Controller/ExC5"
set_directive_pipeline -II 2 "Controller/ExC4"
set_directive_pipeline -II 2 "Controller/ExC3"
set_directive_pipeline -II 2 "Controller/ExC2"
set_directive_pipeline -II 2 "Controller/ExC1"
set_directive_pipeline -II 4 "Controller/col"
set_directive_unroll "Controller/initial"
set_directive_pipeline "Controller/PartitionC"
set_directive_interface -mode s_axilite -bundle bus_bundle_configur "Controller"
set_directive_unroll "Controller/inputCopy"
