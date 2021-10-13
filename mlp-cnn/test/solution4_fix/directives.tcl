############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_resource -core RAM_2P_BRAM "Controller" NInputs
set_directive_resource -core RAM_2P_BRAM "Controller" NNeurons
set_directive_resource -core RAM_2P_BRAM "Controller" NPartions
set_directive_resource -core RAM_2P_BRAM "Controller" NExtraPE_Needed
set_directive_interface -mode axis -register -register_mode both "Controller" in_stream
set_directive_interface -mode axis -register -register_mode both "Controller" out_stream
set_directive_interface -mode s_axilite -bundle bus_bundle_configur "Controller" Config
set_directive_interface -mode s_axilite -bundle bus_bundle_configur "Controller" new_net
set_directive_interface -mode s_axilite -bundle bus_bundle_configur "Controller" conv
set_directive_interface -mode s_axilite -bundle bus_bundle_configur "Controller"
set_directive_pipeline "Controller/INPUTS"
set_directive_pipeline "Controller/MAXW"
set_directive_pipeline "Controller/WEIGHTS"
set_directive_pipeline "Controller/PartitionL"
set_directive_pipeline "Controller/KERNELS"
set_directive_pipeline -II 2 "Controller/INPUTS2"
set_directive_pipeline -II 4 "Controller/J1"
set_directive_pipeline -II 2 "Controller/Ex1"
set_directive_pipeline -II 2 "Controller/Ex2"
set_directive_pipeline -II 2 "Controller/Ex3"
set_directive_pipeline -II 2 "Controller/Ex4"
set_directive_pipeline -II 3 "Controller/Ex5"
set_directive_pipeline -II 3 "Controller/Ex6"
set_directive_pipeline -II 4 "Controller/Ex7"
set_directive_pipeline -II 4 "Controller/K2"
set_directive_pipeline -II 4 "Controller/J3"
set_directive_pipeline -II 2 "Controller/Ex8"
set_directive_pipeline -II 2 "Controller/Ex9"
set_directive_pipeline -II 2 "Controller/Ex10"
set_directive_pipeline -II 2 "Controller/Ex11"
set_directive_pipeline -II 3 "Controller/Ex12"
set_directive_pipeline -II 3 "Controller/Ex13"
set_directive_pipeline -II 4 "Controller/Ex14"
set_directive_pipeline -II 8 "Controller/K4"
set_directive_pipeline "PE0_Unit/PE0_Unit"
set_directive_pipeline "PE1_Unit/PE1_Unit"
set_directive_pipeline "PE2_Unit/PE2_Unit"
set_directive_pipeline "PE3_Unit/PE3_Unit"
set_directive_pipeline "PE4_Unit/PE4_Unit"
set_directive_pipeline "PE5_Unit/PE5_Unit"
set_directive_pipeline "PE6_Unit/PE6_Unit"
set_directive_pipeline "PE7_Unit/PE7_Unit"
set_directive_pipeline -II 39 "Controller/copy1"
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" weights
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" kernels
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" inputs
set_directive_pipeline -II 4 "Controller/col"
set_directive_pipeline "Controller/ExC1"
set_directive_pipeline "Controller/ExC2"
set_directive_pipeline -II 2 "Controller/ExC3"
set_directive_pipeline -II 2 "Controller/ExC4"
set_directive_pipeline -II 3 "Controller/ExC5"
set_directive_pipeline -II 3 "Controller/ExC6"
set_directive_pipeline -II 4 "Controller/ExC7"
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_weight.N0_weight
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_weight.N1_weight
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_weight.N2_weight
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_weight.N3_weight
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_weight.N4_weight
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_weight.N5_weight
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_weight.N6_weight
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_weight.N7_weight
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_input.N0_input
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_input.N1_input
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_input.N2_input
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_input.N3_input
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_input.N4_input
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_input.N5_input
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_input.N6_input
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_input.N7_input
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_output.PE0_output
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_output.PE1_output
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_output.PE2_output
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_output.PE3_output
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_output.PE4_output
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_output.PE5_output
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_output.PE6_output
set_directive_array_partition -type block -factor 4 -dim 1 "Controller" PE_output.PE7_output
set_directive_resource -core RAM_2P_BRAM "Controller" weights
set_directive_resource -core RAM_2P_BRAM "Controller" kernels
set_directive_resource -core RAM_2P_BRAM "Controller" inputs
