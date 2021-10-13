################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/Users/ekabir/Desktop/FPGA/hw/MLP_CNN/NNP3_upgrade1.cpp 

OBJS += \
./source/NNP3_upgrade1.o 

CPP_DEPS += \
./source/NNP3_upgrade1.d 


# Each subdirectory must supply rules for building sources it contributes
source/NNP3_upgrade1.o: C:/Users/ekabir/Desktop/FPGA/hw/MLP_CNN/NNP3_upgrade1.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DAESL_TB -D__llvm__ -D__llvm__ -IC:/Users/ekabir/Desktop/FPGA/hw/MLP_CNN -IC:/Xilinx2/Vivado/2019.2/include -IC:/Xilinx2/Vivado/2019.2/win64/tools/auto_cc/include -IC:/Xilinx2/Vivado/2019.2/include/etc -IC:/Xilinx2/Vivado/2019.2/win64/tools/systemc/include -IC:/Xilinx2/Vivado/2019.2/include/ap_sysc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


