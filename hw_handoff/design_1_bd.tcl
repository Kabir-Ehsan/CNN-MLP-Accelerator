
################################################################
# This is a generated script based on design: design_1
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2019.2
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   catch {common::send_msg_id "BD_TCL-109" "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source design_1_script.tcl

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xc7vx485tffg1761-2
   set_property BOARD_PART xilinx.com:vc707:part0:1.4 [current_project]
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name design_1

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      common::send_msg_id "BD_TCL-001" "INFO" "Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   common::send_msg_id "BD_TCL-002" "INFO" "Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   common::send_msg_id "BD_TCL-003" "INFO" "Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   common::send_msg_id "BD_TCL-004" "INFO" "Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

common::send_msg_id "BD_TCL-005" "INFO" "Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   catch {common::send_msg_id "BD_TCL-114" "ERROR" $errMsg}
   return $nRet
}

##################################################################
# DESIGN PROCs
##################################################################


# Hierarchical cell: microblaze_0_local_memory
proc create_hier_cell_microblaze_0_local_memory { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_msg_id "BD_TCL-102" "ERROR" "create_hier_cell_microblaze_0_local_memory() - Empty argument(s)!"}
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_msg_id "BD_TCL-100" "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_msg_id "BD_TCL-101" "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode MirroredMaster -vlnv xilinx.com:interface:lmb_rtl:1.0 DLMB

  create_bd_intf_pin -mode MirroredMaster -vlnv xilinx.com:interface:lmb_rtl:1.0 ILMB


  # Create pins
  create_bd_pin -dir I -type clk LMB_Clk
  create_bd_pin -dir I -type rst SYS_Rst

  # Create instance: dlmb_bram_if_cntlr, and set properties
  set dlmb_bram_if_cntlr [ create_bd_cell -type ip -vlnv xilinx.com:ip:lmb_bram_if_cntlr:4.0 dlmb_bram_if_cntlr ]
  set_property -dict [ list \
   CONFIG.C_ECC {0} \
 ] $dlmb_bram_if_cntlr

  # Create instance: dlmb_v10, and set properties
  set dlmb_v10 [ create_bd_cell -type ip -vlnv xilinx.com:ip:lmb_v10:3.0 dlmb_v10 ]

  # Create instance: ilmb_bram_if_cntlr, and set properties
  set ilmb_bram_if_cntlr [ create_bd_cell -type ip -vlnv xilinx.com:ip:lmb_bram_if_cntlr:4.0 ilmb_bram_if_cntlr ]
  set_property -dict [ list \
   CONFIG.C_ECC {0} \
 ] $ilmb_bram_if_cntlr

  # Create instance: ilmb_v10, and set properties
  set ilmb_v10 [ create_bd_cell -type ip -vlnv xilinx.com:ip:lmb_v10:3.0 ilmb_v10 ]

  # Create instance: lmb_bram, and set properties
  set lmb_bram [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 lmb_bram ]
  set_property -dict [ list \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $lmb_bram

  # Create interface connections
  connect_bd_intf_net -intf_net microblaze_0_dlmb [get_bd_intf_pins DLMB] [get_bd_intf_pins dlmb_v10/LMB_M]
  connect_bd_intf_net -intf_net microblaze_0_dlmb_bus [get_bd_intf_pins dlmb_bram_if_cntlr/SLMB] [get_bd_intf_pins dlmb_v10/LMB_Sl_0]
  connect_bd_intf_net -intf_net microblaze_0_dlmb_cntlr [get_bd_intf_pins dlmb_bram_if_cntlr/BRAM_PORT] [get_bd_intf_pins lmb_bram/BRAM_PORTA]
  connect_bd_intf_net -intf_net microblaze_0_ilmb [get_bd_intf_pins ILMB] [get_bd_intf_pins ilmb_v10/LMB_M]
  connect_bd_intf_net -intf_net microblaze_0_ilmb_bus [get_bd_intf_pins ilmb_bram_if_cntlr/SLMB] [get_bd_intf_pins ilmb_v10/LMB_Sl_0]
  connect_bd_intf_net -intf_net microblaze_0_ilmb_cntlr [get_bd_intf_pins ilmb_bram_if_cntlr/BRAM_PORT] [get_bd_intf_pins lmb_bram/BRAM_PORTB]

  # Create port connections
  connect_bd_net -net SYS_Rst_1 [get_bd_pins SYS_Rst] [get_bd_pins dlmb_bram_if_cntlr/LMB_Rst] [get_bd_pins dlmb_v10/SYS_Rst] [get_bd_pins ilmb_bram_if_cntlr/LMB_Rst] [get_bd_pins ilmb_v10/SYS_Rst]
  connect_bd_net -net microblaze_0_Clk [get_bd_pins LMB_Clk] [get_bd_pins dlmb_bram_if_cntlr/LMB_Clk] [get_bd_pins dlmb_v10/LMB_Clk] [get_bd_pins ilmb_bram_if_cntlr/LMB_Clk] [get_bd_pins ilmb_v10/LMB_Clk]

  # Restore current instance
  current_bd_instance $oldCurInst
}


# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  variable script_folder
  variable design_name

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_msg_id "BD_TCL-100" "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_msg_id "BD_TCL-101" "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set rs232_uart [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:uart_rtl:1.0 rs232_uart ]

  set sys_diff_clock [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 sys_diff_clock ]
  set_property -dict [ list \
   CONFIG.FREQ_HZ {200000000} \
   ] $sys_diff_clock


  # Create ports
  set reset [ create_bd_port -dir I -type rst reset ]
  set_property -dict [ list \
   CONFIG.POLARITY {ACTIVE_HIGH} \
 ] $reset

  # Create instance: Controller_0, and set properties
  set Controller_0 [ create_bd_cell -type ip -vlnv xilinx.com:hls:Controller:1.0 Controller_0 ]

  # Create instance: Controller_0_bram, and set properties
  set Controller_0_bram [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram

  # Create instance: Controller_0_bram_0, and set properties
  set Controller_0_bram_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_0 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_0

  # Create instance: Controller_0_bram_1, and set properties
  set Controller_0_bram_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_1 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_1

  # Create instance: Controller_0_bram_2, and set properties
  set Controller_0_bram_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_2 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_2

  # Create instance: Controller_0_bram_3, and set properties
  set Controller_0_bram_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_3 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_3

  # Create instance: Controller_0_bram_4, and set properties
  set Controller_0_bram_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_4 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_4

  # Create instance: Controller_0_bram_5, and set properties
  set Controller_0_bram_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_5 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_5

  # Create instance: Controller_0_bram_6, and set properties
  set Controller_0_bram_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_6 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_6

  # Create instance: Controller_0_bram_7, and set properties
  set Controller_0_bram_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_7 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_7

  # Create instance: Controller_0_bram_8, and set properties
  set Controller_0_bram_8 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_8 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Read_Width_A {32} \
   CONFIG.Read_Width_B {32} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Register_PortB_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.Write_Width_A {32} \
   CONFIG.Write_Width_B {32} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_8

  # Create instance: Controller_0_bram_9, and set properties
  set Controller_0_bram_9 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_9 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_9

  # Create instance: Controller_0_bram_10, and set properties
  set Controller_0_bram_10 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_10 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_10

  # Create instance: Controller_0_bram_11, and set properties
  set Controller_0_bram_11 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_11 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_11

  # Create instance: Controller_0_bram_12, and set properties
  set Controller_0_bram_12 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_12 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_12

  # Create instance: Controller_0_bram_13, and set properties
  set Controller_0_bram_13 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_13 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_13

  # Create instance: Controller_0_bram_14, and set properties
  set Controller_0_bram_14 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_14 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_14

  # Create instance: Controller_0_bram_15, and set properties
  set Controller_0_bram_15 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_15 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_15

  # Create instance: Controller_0_bram_16, and set properties
  set Controller_0_bram_16 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_16 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_16

  # Create instance: Controller_0_bram_17, and set properties
  set Controller_0_bram_17 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_17 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_17

  # Create instance: Controller_0_bram_18, and set properties
  set Controller_0_bram_18 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_18 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_18

  # Create instance: Controller_0_bram_19, and set properties
  set Controller_0_bram_19 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_19 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_19

  # Create instance: Controller_0_bram_20, and set properties
  set Controller_0_bram_20 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 Controller_0_bram_20 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $Controller_0_bram_20

  # Create instance: axi_bram_ctrl_0, and set properties
  set axi_bram_ctrl_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_0 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_0

  # Create instance: axi_bram_ctrl_1, and set properties
  set axi_bram_ctrl_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_1 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_1

  # Create instance: axi_bram_ctrl_2, and set properties
  set axi_bram_ctrl_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_2 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_2

  # Create instance: axi_bram_ctrl_3, and set properties
  set axi_bram_ctrl_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_3 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_3

  # Create instance: axi_bram_ctrl_4, and set properties
  set axi_bram_ctrl_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_4 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_4

  # Create instance: axi_bram_ctrl_5, and set properties
  set axi_bram_ctrl_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_5 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_5

  # Create instance: axi_bram_ctrl_6, and set properties
  set axi_bram_ctrl_6 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_6 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_6

  # Create instance: axi_bram_ctrl_7, and set properties
  set axi_bram_ctrl_7 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_7 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_7

  # Create instance: axi_bram_ctrl_8, and set properties
  set axi_bram_ctrl_8 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_8 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_8

  # Create instance: axi_bram_ctrl_9, and set properties
  set axi_bram_ctrl_9 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_9 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_9

  # Create instance: axi_bram_ctrl_10, and set properties
  set axi_bram_ctrl_10 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_10 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_10

  # Create instance: axi_bram_ctrl_11, and set properties
  set axi_bram_ctrl_11 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_11 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_11

  # Create instance: axi_bram_ctrl_12, and set properties
  set axi_bram_ctrl_12 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_12 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_12

  # Create instance: axi_bram_ctrl_13, and set properties
  set axi_bram_ctrl_13 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_13 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_13

  # Create instance: axi_bram_ctrl_14, and set properties
  set axi_bram_ctrl_14 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_14 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_14

  # Create instance: axi_bram_ctrl_15, and set properties
  set axi_bram_ctrl_15 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_15 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_15

  # Create instance: axi_bram_ctrl_16, and set properties
  set axi_bram_ctrl_16 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_16 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_16

  # Create instance: axi_bram_ctrl_17, and set properties
  set axi_bram_ctrl_17 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_17 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_17

  # Create instance: axi_bram_ctrl_18, and set properties
  set axi_bram_ctrl_18 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_18 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_18

  # Create instance: axi_bram_ctrl_19, and set properties
  set axi_bram_ctrl_19 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_19 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_19

  # Create instance: axi_bram_ctrl_20, and set properties
  set axi_bram_ctrl_20 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_20 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_20

  # Create instance: axi_bram_ctrl_21, and set properties
  set axi_bram_ctrl_21 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_21 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_21

  # Create instance: axi_timer_0, and set properties
  set axi_timer_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_timer:2.0 axi_timer_0 ]

  # Create instance: axi_uartlite_0, and set properties
  set axi_uartlite_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_uartlite:2.0 axi_uartlite_0 ]
  set_property -dict [ list \
   CONFIG.UARTLITE_BOARD_INTERFACE {rs232_uart} \
   CONFIG.USE_BOARD_FLOW {true} \
 ] $axi_uartlite_0

  # Create instance: clk_wiz_1, and set properties
  set clk_wiz_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wiz:6.0 clk_wiz_1 ]
  set_property -dict [ list \
   CONFIG.CLK_IN1_BOARD_INTERFACE {sys_diff_clock} \
   CONFIG.PRIM_SOURCE {Differential_clock_capable_pin} \
   CONFIG.RESET_BOARD_INTERFACE {reset} \
   CONFIG.USE_BOARD_FLOW {true} \
 ] $clk_wiz_1

  # Create instance: mdm_1, and set properties
  set mdm_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:mdm:3.2 mdm_1 ]

  # Create instance: microblaze_0, and set properties
  set microblaze_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:microblaze:11.0 microblaze_0 ]
  set_property -dict [ list \
   CONFIG.C_DEBUG_ENABLED {1} \
   CONFIG.C_D_AXI {1} \
   CONFIG.C_D_LMB {1} \
   CONFIG.C_I_LMB {1} \
 ] $microblaze_0

  # Create instance: microblaze_0_axi_periph, and set properties
  set microblaze_0_axi_periph [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 microblaze_0_axi_periph ]
  set_property -dict [ list \
   CONFIG.NUM_MI {25} \
 ] $microblaze_0_axi_periph

  # Create instance: microblaze_0_local_memory
  create_hier_cell_microblaze_0_local_memory [current_bd_instance .] microblaze_0_local_memory

  # Create instance: rst_clk_wiz_1_100M, and set properties
  set rst_clk_wiz_1_100M [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 rst_clk_wiz_1_100M ]
  set_property -dict [ list \
   CONFIG.RESET_BOARD_INTERFACE {reset} \
   CONFIG.USE_BOARD_FLOW {true} \
 ] $rst_clk_wiz_1_100M

  # Create interface connections
  connect_bd_intf_net -intf_net Controller_0_cellBias_PORTA [get_bd_intf_pins Controller_0/cellBias_PORTA] [get_bd_intf_pins Controller_0_bram/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_cellReWeight_PORTA [get_bd_intf_pins Controller_0/cellReWeight_PORTA] [get_bd_intf_pins Controller_0_bram_0/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_cellReWeight_PORTB [get_bd_intf_pins Controller_0/cellReWeight_PORTB] [get_bd_intf_pins Controller_0_bram_1/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_cellWeight_PORTA [get_bd_intf_pins Controller_0/cellWeight_PORTA] [get_bd_intf_pins Controller_0_bram_2/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_fcbias_PORTA [get_bd_intf_pins Controller_0/fcbias_PORTA] [get_bd_intf_pins Controller_0_bram_3/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_forgetBias_PORTA [get_bd_intf_pins Controller_0/forgetBias_PORTA] [get_bd_intf_pins Controller_0_bram_4/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_forgetReWeight_PORTA [get_bd_intf_pins Controller_0/forgetReWeight_PORTA] [get_bd_intf_pins Controller_0_bram_5/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_forgetReWeight_PORTB [get_bd_intf_pins Controller_0/forgetReWeight_PORTB] [get_bd_intf_pins Controller_0_bram_6/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_forgetWeight_PORTA [get_bd_intf_pins Controller_0/forgetWeight_PORTA] [get_bd_intf_pins Controller_0_bram_7/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_in_stream_PORTA [get_bd_intf_pins Controller_0/in_stream_PORTA] [get_bd_intf_pins Controller_0_bram_8/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_in_stream_PORTB [get_bd_intf_pins Controller_0/in_stream_PORTB] [get_bd_intf_pins Controller_0_bram_9/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_inputBias_PORTA [get_bd_intf_pins Controller_0/inputBias_PORTA] [get_bd_intf_pins Controller_0_bram_10/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_inputReWeight_PORTA [get_bd_intf_pins Controller_0/inputReWeight_PORTA] [get_bd_intf_pins Controller_0_bram_11/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_inputReWeight_PORTB [get_bd_intf_pins Controller_0/inputReWeight_PORTB] [get_bd_intf_pins Controller_0_bram_12/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_inputWeight_PORTA [get_bd_intf_pins Controller_0/inputWeight_PORTA] [get_bd_intf_pins Controller_0_bram_13/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_out_stream_PORTA [get_bd_intf_pins Controller_0/out_stream_PORTA] [get_bd_intf_pins Controller_0_bram_14/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_out_stream_PORTB [get_bd_intf_pins Controller_0/out_stream_PORTB] [get_bd_intf_pins Controller_0_bram_15/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_outputBias_PORTA [get_bd_intf_pins Controller_0/outputBias_PORTA] [get_bd_intf_pins Controller_0_bram_16/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_outputReWeight_PORTA [get_bd_intf_pins Controller_0/outputReWeight_PORTA] [get_bd_intf_pins Controller_0_bram_17/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_outputReWeight_PORTB [get_bd_intf_pins Controller_0/outputReWeight_PORTB] [get_bd_intf_pins Controller_0_bram_18/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_outputWeight_PORTA [get_bd_intf_pins Controller_0/outputWeight_PORTA] [get_bd_intf_pins Controller_0_bram_19/BRAM_PORTA]
  connect_bd_intf_net -intf_net Controller_0_weights_PORTA [get_bd_intf_pins Controller_0/weights_PORTA] [get_bd_intf_pins Controller_0_bram_20/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_0_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_8/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_0/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_10_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_19/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_10/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_11_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_11/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_11/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_12_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_4/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_12/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_13_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_5/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_13/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_14_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_20/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_14/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_15_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_18/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_15/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_16_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_17/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_16/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_17_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_3/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_17/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_18_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_12/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_18/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_19_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_6/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_19/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_1_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_9/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_1/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_20_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_10/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_20/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_21_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_16/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_21/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_2_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_15/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_2/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_3_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_14/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_3/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_4_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_13/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_4/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_5_BRAM_PORTA [get_bd_intf_pins Controller_0_bram/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_5/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_6_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_1/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_6/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_7_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_0/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_7/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_8_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_7/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_8/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_bram_ctrl_9_BRAM_PORTA [get_bd_intf_pins Controller_0_bram_2/BRAM_PORTB] [get_bd_intf_pins axi_bram_ctrl_9/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_uartlite_0_UART [get_bd_intf_ports rs232_uart] [get_bd_intf_pins axi_uartlite_0/UART]
  connect_bd_intf_net -intf_net microblaze_0_M_AXI_DP [get_bd_intf_pins microblaze_0/M_AXI_DP] [get_bd_intf_pins microblaze_0_axi_periph/S00_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M00_AXI [get_bd_intf_pins Controller_0/s_axi_bus_bundle] [get_bd_intf_pins microblaze_0_axi_periph/M00_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M01_AXI [get_bd_intf_pins axi_timer_0/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M01_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M02_AXI [get_bd_intf_pins axi_uartlite_0/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M02_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M03_AXI [get_bd_intf_pins axi_bram_ctrl_0/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M03_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M04_AXI [get_bd_intf_pins axi_bram_ctrl_1/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M04_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M05_AXI [get_bd_intf_pins axi_bram_ctrl_2/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M05_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M06_AXI [get_bd_intf_pins axi_bram_ctrl_3/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M06_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M07_AXI [get_bd_intf_pins axi_bram_ctrl_4/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M07_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M08_AXI [get_bd_intf_pins axi_bram_ctrl_5/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M08_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M09_AXI [get_bd_intf_pins axi_bram_ctrl_6/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M09_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M10_AXI [get_bd_intf_pins axi_bram_ctrl_7/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M10_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M11_AXI [get_bd_intf_pins axi_bram_ctrl_8/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M11_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M12_AXI [get_bd_intf_pins axi_bram_ctrl_9/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M12_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M13_AXI [get_bd_intf_pins axi_bram_ctrl_10/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M13_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M14_AXI [get_bd_intf_pins axi_bram_ctrl_11/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M14_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M15_AXI [get_bd_intf_pins axi_bram_ctrl_12/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M15_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M16_AXI [get_bd_intf_pins axi_bram_ctrl_13/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M16_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M17_AXI [get_bd_intf_pins axi_bram_ctrl_14/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M17_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M18_AXI [get_bd_intf_pins axi_bram_ctrl_15/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M18_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M19_AXI [get_bd_intf_pins axi_bram_ctrl_16/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M19_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M20_AXI [get_bd_intf_pins axi_bram_ctrl_17/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M20_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M21_AXI [get_bd_intf_pins axi_bram_ctrl_18/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M21_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M22_AXI [get_bd_intf_pins axi_bram_ctrl_19/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M22_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M23_AXI [get_bd_intf_pins axi_bram_ctrl_20/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M23_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M24_AXI [get_bd_intf_pins axi_bram_ctrl_21/S_AXI] [get_bd_intf_pins microblaze_0_axi_periph/M24_AXI]
  connect_bd_intf_net -intf_net microblaze_0_debug [get_bd_intf_pins mdm_1/MBDEBUG_0] [get_bd_intf_pins microblaze_0/DEBUG]
  connect_bd_intf_net -intf_net microblaze_0_dlmb_1 [get_bd_intf_pins microblaze_0/DLMB] [get_bd_intf_pins microblaze_0_local_memory/DLMB]
  connect_bd_intf_net -intf_net microblaze_0_ilmb_1 [get_bd_intf_pins microblaze_0/ILMB] [get_bd_intf_pins microblaze_0_local_memory/ILMB]
  connect_bd_intf_net -intf_net sys_diff_clock_1 [get_bd_intf_ports sys_diff_clock] [get_bd_intf_pins clk_wiz_1/CLK_IN1_D]

  # Create port connections
  connect_bd_net -net clk_wiz_1_locked [get_bd_pins clk_wiz_1/locked] [get_bd_pins rst_clk_wiz_1_100M/dcm_locked]
  connect_bd_net -net mdm_1_debug_sys_rst [get_bd_pins mdm_1/Debug_SYS_Rst] [get_bd_pins rst_clk_wiz_1_100M/mb_debug_sys_rst]
  connect_bd_net -net microblaze_0_Clk [get_bd_pins Controller_0/ap_clk] [get_bd_pins axi_bram_ctrl_0/s_axi_aclk] [get_bd_pins axi_bram_ctrl_1/s_axi_aclk] [get_bd_pins axi_bram_ctrl_10/s_axi_aclk] [get_bd_pins axi_bram_ctrl_11/s_axi_aclk] [get_bd_pins axi_bram_ctrl_12/s_axi_aclk] [get_bd_pins axi_bram_ctrl_13/s_axi_aclk] [get_bd_pins axi_bram_ctrl_14/s_axi_aclk] [get_bd_pins axi_bram_ctrl_15/s_axi_aclk] [get_bd_pins axi_bram_ctrl_16/s_axi_aclk] [get_bd_pins axi_bram_ctrl_17/s_axi_aclk] [get_bd_pins axi_bram_ctrl_18/s_axi_aclk] [get_bd_pins axi_bram_ctrl_19/s_axi_aclk] [get_bd_pins axi_bram_ctrl_2/s_axi_aclk] [get_bd_pins axi_bram_ctrl_20/s_axi_aclk] [get_bd_pins axi_bram_ctrl_21/s_axi_aclk] [get_bd_pins axi_bram_ctrl_3/s_axi_aclk] [get_bd_pins axi_bram_ctrl_4/s_axi_aclk] [get_bd_pins axi_bram_ctrl_5/s_axi_aclk] [get_bd_pins axi_bram_ctrl_6/s_axi_aclk] [get_bd_pins axi_bram_ctrl_7/s_axi_aclk] [get_bd_pins axi_bram_ctrl_8/s_axi_aclk] [get_bd_pins axi_bram_ctrl_9/s_axi_aclk] [get_bd_pins axi_timer_0/s_axi_aclk] [get_bd_pins axi_uartlite_0/s_axi_aclk] [get_bd_pins clk_wiz_1/clk_out1] [get_bd_pins microblaze_0/Clk] [get_bd_pins microblaze_0_axi_periph/ACLK] [get_bd_pins microblaze_0_axi_periph/M00_ACLK] [get_bd_pins microblaze_0_axi_periph/M01_ACLK] [get_bd_pins microblaze_0_axi_periph/M02_ACLK] [get_bd_pins microblaze_0_axi_periph/M03_ACLK] [get_bd_pins microblaze_0_axi_periph/M04_ACLK] [get_bd_pins microblaze_0_axi_periph/M05_ACLK] [get_bd_pins microblaze_0_axi_periph/M06_ACLK] [get_bd_pins microblaze_0_axi_periph/M07_ACLK] [get_bd_pins microblaze_0_axi_periph/M08_ACLK] [get_bd_pins microblaze_0_axi_periph/M09_ACLK] [get_bd_pins microblaze_0_axi_periph/M10_ACLK] [get_bd_pins microblaze_0_axi_periph/M11_ACLK] [get_bd_pins microblaze_0_axi_periph/M12_ACLK] [get_bd_pins microblaze_0_axi_periph/M13_ACLK] [get_bd_pins microblaze_0_axi_periph/M14_ACLK] [get_bd_pins microblaze_0_axi_periph/M15_ACLK] [get_bd_pins microblaze_0_axi_periph/M16_ACLK] [get_bd_pins microblaze_0_axi_periph/M17_ACLK] [get_bd_pins microblaze_0_axi_periph/M18_ACLK] [get_bd_pins microblaze_0_axi_periph/M19_ACLK] [get_bd_pins microblaze_0_axi_periph/M20_ACLK] [get_bd_pins microblaze_0_axi_periph/M21_ACLK] [get_bd_pins microblaze_0_axi_periph/M22_ACLK] [get_bd_pins microblaze_0_axi_periph/M23_ACLK] [get_bd_pins microblaze_0_axi_periph/M24_ACLK] [get_bd_pins microblaze_0_axi_periph/S00_ACLK] [get_bd_pins microblaze_0_local_memory/LMB_Clk] [get_bd_pins rst_clk_wiz_1_100M/slowest_sync_clk]
  connect_bd_net -net reset_1 [get_bd_ports reset] [get_bd_pins clk_wiz_1/reset] [get_bd_pins rst_clk_wiz_1_100M/ext_reset_in]
  connect_bd_net -net rst_clk_wiz_1_100M_bus_struct_reset [get_bd_pins microblaze_0_local_memory/SYS_Rst] [get_bd_pins rst_clk_wiz_1_100M/bus_struct_reset]
  connect_bd_net -net rst_clk_wiz_1_100M_mb_reset [get_bd_pins microblaze_0/Reset] [get_bd_pins rst_clk_wiz_1_100M/mb_reset]
  connect_bd_net -net rst_clk_wiz_1_100M_peripheral_aresetn [get_bd_pins Controller_0/ap_rst_n] [get_bd_pins axi_bram_ctrl_0/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_1/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_10/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_11/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_12/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_13/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_14/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_15/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_16/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_17/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_18/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_19/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_2/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_20/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_21/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_3/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_4/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_5/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_6/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_7/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_8/s_axi_aresetn] [get_bd_pins axi_bram_ctrl_9/s_axi_aresetn] [get_bd_pins axi_timer_0/s_axi_aresetn] [get_bd_pins axi_uartlite_0/s_axi_aresetn] [get_bd_pins microblaze_0_axi_periph/ARESETN] [get_bd_pins microblaze_0_axi_periph/M00_ARESETN] [get_bd_pins microblaze_0_axi_periph/M01_ARESETN] [get_bd_pins microblaze_0_axi_periph/M02_ARESETN] [get_bd_pins microblaze_0_axi_periph/M03_ARESETN] [get_bd_pins microblaze_0_axi_periph/M04_ARESETN] [get_bd_pins microblaze_0_axi_periph/M05_ARESETN] [get_bd_pins microblaze_0_axi_periph/M06_ARESETN] [get_bd_pins microblaze_0_axi_periph/M07_ARESETN] [get_bd_pins microblaze_0_axi_periph/M08_ARESETN] [get_bd_pins microblaze_0_axi_periph/M09_ARESETN] [get_bd_pins microblaze_0_axi_periph/M10_ARESETN] [get_bd_pins microblaze_0_axi_periph/M11_ARESETN] [get_bd_pins microblaze_0_axi_periph/M12_ARESETN] [get_bd_pins microblaze_0_axi_periph/M13_ARESETN] [get_bd_pins microblaze_0_axi_periph/M14_ARESETN] [get_bd_pins microblaze_0_axi_periph/M15_ARESETN] [get_bd_pins microblaze_0_axi_periph/M16_ARESETN] [get_bd_pins microblaze_0_axi_periph/M17_ARESETN] [get_bd_pins microblaze_0_axi_periph/M18_ARESETN] [get_bd_pins microblaze_0_axi_periph/M19_ARESETN] [get_bd_pins microblaze_0_axi_periph/M20_ARESETN] [get_bd_pins microblaze_0_axi_periph/M21_ARESETN] [get_bd_pins microblaze_0_axi_periph/M22_ARESETN] [get_bd_pins microblaze_0_axi_periph/M23_ARESETN] [get_bd_pins microblaze_0_axi_periph/M24_ARESETN] [get_bd_pins microblaze_0_axi_periph/S00_ARESETN] [get_bd_pins rst_clk_wiz_1_100M/peripheral_aresetn]

  # Create address segments
  assign_bd_address -offset 0x44A00000 -range 0x00010000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs Controller_0/s_axi_bus_bundle/Reg] -force
  assign_bd_address -offset 0xC0000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_0/S_AXI/Mem0] -force
  assign_bd_address -offset 0xD4000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_10/S_AXI/Mem0] -force
  assign_bd_address -offset 0xD6000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_11/S_AXI/Mem0] -force
  assign_bd_address -offset 0xD8000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_12/S_AXI/Mem0] -force
  assign_bd_address -offset 0xDA000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_13/S_AXI/Mem0] -force
  assign_bd_address -offset 0xDC000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_14/S_AXI/Mem0] -force
  assign_bd_address -offset 0xDE000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_15/S_AXI/Mem0] -force
  assign_bd_address -offset 0xE0000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_16/S_AXI/Mem0] -force
  assign_bd_address -offset 0xE2000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_17/S_AXI/Mem0] -force
  assign_bd_address -offset 0xE4000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_18/S_AXI/Mem0] -force
  assign_bd_address -offset 0xE6000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_19/S_AXI/Mem0] -force
  assign_bd_address -offset 0xC2000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_1/S_AXI/Mem0] -force
  assign_bd_address -offset 0xE8000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_20/S_AXI/Mem0] -force
  assign_bd_address -offset 0xEA000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_21/S_AXI/Mem0] -force
  assign_bd_address -offset 0xC4000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_2/S_AXI/Mem0] -force
  assign_bd_address -offset 0xC6000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_3/S_AXI/Mem0] -force
  assign_bd_address -offset 0xC8000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_4/S_AXI/Mem0] -force
  assign_bd_address -offset 0xCA000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_5/S_AXI/Mem0] -force
  assign_bd_address -offset 0xCC000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_6/S_AXI/Mem0] -force
  assign_bd_address -offset 0xCE000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_7/S_AXI/Mem0] -force
  assign_bd_address -offset 0xD0000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_8/S_AXI/Mem0] -force
  assign_bd_address -offset 0xD2000000 -range 0x00020000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_bram_ctrl_9/S_AXI/Mem0] -force
  assign_bd_address -offset 0x41C00000 -range 0x00010000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_timer_0/S_AXI/Reg] -force
  assign_bd_address -offset 0x40600000 -range 0x00010000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_uartlite_0/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x00100000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs microblaze_0_local_memory/dlmb_bram_if_cntlr/SLMB/Mem] -force
  assign_bd_address -offset 0x00000000 -range 0x00100000 -target_address_space [get_bd_addr_spaces microblaze_0/Instruction] [get_bd_addr_segs microblaze_0_local_memory/ilmb_bram_if_cntlr/SLMB/Mem] -force


  # Restore current instance
  current_bd_instance $oldCurInst

  validate_bd_design
  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


