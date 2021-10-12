################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/platform.c 

CPP_SRCS += \
../src/lstm.cpp 

OBJS += \
./src/lstm.o \
./src/platform.o 

C_DEPS += \
./src/platform.d 

CPP_DEPS += \
./src/lstm.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze g++ compiler'
	mb-g++ -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -IC:/Users/ekabir/Desktop/FPGA/MLP_HLS/MLP_LSTM4/PE08_FLOAT/LSTM_BRAM2/export/LSTM_BRAM2/sw/LSTM_BRAM2/standalone_domain/bspinclude/include -mlittle-endian -mcpu=v11.0 -mxl-soft-mul -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze g++ compiler'
	mb-g++ -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -IC:/Users/ekabir/Desktop/FPGA/MLP_HLS/MLP_LSTM4/PE08_FLOAT/LSTM_BRAM2/export/LSTM_BRAM2/sw/LSTM_BRAM2/standalone_domain/bspinclude/include -mlittle-endian -mcpu=v11.0 -mxl-soft-mul -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


