############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project lstm5

set_top Controller
add_files Controller.h
add_files NNP3_LSTM3.cpp
add_files allParam.h
add_files allParam2.h
add_files allParam3.h
add_files inputsLSTM2.h
add_files labelLSTM2.h
add_files labelLSTM3.h
add_files -tb TB2_LSTM1.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solutio1_LUTAF_bram"
set_part {xc7vx485t-ffg1761-2}
create_clock -period 10 -name default
config_export -format ip_catalog -rtl verilog
##source "C:\\Users\\ekabir\\Desktop\\FPGA\\MLP_HLS\\MLP_LSTM4\\directives.tcl"


