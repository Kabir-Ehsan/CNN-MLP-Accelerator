############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_interface -mode s_axilite -bundle bus_bundle "Controller" new_net
set_directive_interface -mode s_axilite -bundle bus_bundle "Controller"
set_directive_pipeline "forgetGate/inputLayerf"
set_directive_pipeline "forgetGate/hiddenLayerf"
set_directive_pipeline "forgetGate/inputMultf"
set_directive_pipeline "forgetGate/inputAccumf"
set_directive_pipeline "forgetGate/hiddenMultf"
set_directive_pipeline "forgetGate/hiddenAccumf"
set_directive_pipeline "forgetGate/finalf"
set_directive_pipeline "inputGate/inputLayeri"
set_directive_pipeline "inputGate/inputMulti"
set_directive_pipeline "inputGate/inputAccumi"
set_directive_pipeline "inputGate/hiddenMulti"
set_directive_pipeline "inputGate/hiddenAccumi"
set_directive_pipeline "inputGate/hiddenLayeri"
set_directive_pipeline "inputGate/finali"
set_directive_pipeline "cellGate/inputLayerc"
set_directive_pipeline "cellGate/inputMultc"
set_directive_pipeline "cellGate/inputAccumi"
set_directive_pipeline "cellGate/hiddenLayerc"
set_directive_pipeline "cellGate/hiddenMultc"
set_directive_pipeline "cellGate/hiddenAccumc"
set_directive_pipeline "cellGate/finalc"
set_directive_pipeline "outputGate/inputLayero"
set_directive_pipeline "outputGate/inputMulto"
set_directive_pipeline "outputGate/inputAccumo"
set_directive_pipeline "outputGate/hiddenLayero"
set_directive_pipeline "outputGate/hiddenMulto"
set_directive_pipeline "outputGate/hiddenAccumo"
set_directive_pipeline "outputGate/finalo"
set_directive_pipeline -II 4 "Controller/result"
set_directive_pipeline "Controller/outStatus"
set_directive_pipeline "Controller/hiddenStatus"
set_directive_pipeline "Controller/cellStatusNew"
set_directive_pipeline "Controller/cellStatusOld"
set_directive_pipeline "Controller/cellCandidate"
##set_directive_pipeline "Controller/INPUTS"
##set_directive_pipeline "Controller/InWeight1"
##set_directive_pipeline "Controller/ReWeight1"
##set_directive_pipeline "Controller/WEIGHTS"
##set_directive_pipeline "Controller/BIAS1"
set_directive_pipeline "Controller/initial"
##set_directive_pipeline -II 2 "Controller/SetInWeights"
##set_directive_pipeline -II 2 "Controller/SetReWeights"
##set_directive_pipeline -II 2 "Controller/SetBias"
##set_directive_pipeline -II 2 "Controller/nextState"
##set_directive_pipeline -II 2 "Controller/OUTstream"
##set_directive_dataflow "Controller/gates"
set_directive_loop_merge -force "Controller/region1"
set_directive_interface -mode bram "Controller" in_stream
set_directive_interface -mode bram "Controller" out_stream
set_directive_interface -mode bram "Controller" inputWeight
set_directive_interface -mode bram "Controller" forgetWeight
set_directive_interface -mode bram "Controller" cellWeight
set_directive_interface -mode bram "Controller" outputWeight
set_directive_interface -mode bram "Controller" inputReWeight
set_directive_interface -mode bram "Controller" forgetReWeight
set_directive_interface -mode bram "Controller" cellReWeight
set_directive_interface -mode bram "Controller" outputReWeight
set_directive_interface -mode bram "Controller" weights
set_directive_interface -mode bram "Controller" inputBias
set_directive_interface -mode bram "Controller" forgetBias
set_directive_interface -mode bram "Controller" cellBias
set_directive_interface -mode bram "Controller" outputBias
set_directive_interface -mode bram "Controller" fcbias
