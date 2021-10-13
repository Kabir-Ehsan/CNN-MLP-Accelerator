############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project test
set_top Controller
add_files weights_mnist5.h
add_files weights_mnist4.h
add_files weights_mnist3.h
add_files weights_mnist2.h
add_files weights_mnist.h
add_files weights_kernel4.h
add_files weights_kernel3.h
add_files weights_kernel2.h
add_files weights_kernel.h
add_files inputs3.h
add_files inputs.h
add_files NNP3_upgrade1.cpp
add_files Controller_net_upgrade1.h
add_files -tb NNP_TB_upgrade1.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution4_fix"
set_part {xc7vx485t-ffg1761-2}
create_clock -period 10ns -name default
config_export -format ip_catalog -rtl verilog -vivado_optimization_level 2 -vivado_phys_opt place -vivado_report_level 0
config_sdx -optimization_level none -target none
set_clock_uncertainty 12.5%
source "./test/solution4_fix/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -rtl verilog -format ip_catalog
