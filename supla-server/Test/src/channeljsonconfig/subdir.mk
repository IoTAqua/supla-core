################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/channeljsonconfig/action_trigger_config.cpp \
../src/channeljsonconfig/channel_json_config.cpp \
../src/channeljsonconfig/controlling_the_gate_config.cpp \
../src/channeljsonconfig/electicity_meter_config.cpp \
../src/channeljsonconfig/impulse_counter_config.cpp 

OBJS += \
./src/channeljsonconfig/action_trigger_config.o \
./src/channeljsonconfig/channel_json_config.o \
./src/channeljsonconfig/controlling_the_gate_config.o \
./src/channeljsonconfig/electicity_meter_config.o \
./src/channeljsonconfig/impulse_counter_config.o 

CPP_DEPS += \
./src/channeljsonconfig/action_trigger_config.d \
./src/channeljsonconfig/channel_json_config.d \
./src/channeljsonconfig/controlling_the_gate_config.d \
./src/channeljsonconfig/electicity_meter_config.d \
./src/channeljsonconfig/impulse_counter_config.d 


# Each subdirectory must supply rules for building sources it contributes
src/channeljsonconfig/%.o: ../src/channeljsonconfig/%.cpp src/channeljsonconfig/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D__DEBUG=1 -DUSE_DEPRECATED_EMEV_V1 -D__TEST=1 -D__OPENSSL_TOOLS=1 -D__BCRYPT=1 -I../src -I../src/asynctask -I../src/mqtt -I$(INCMYSQL) -I../src/user -I../src/device -I../src/client -I$(SSLDIR)/include -I../src/test -O2 -g3 -Wall -fsigned-char -c -fmessage-length=0 -fstack-protector-all -D_FORTIFY_SOURCE=2 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


