################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/channeljsonconfig/channel_json_config.cpp \
../src/channeljsonconfig/electicity_meter_config.cpp 

OBJS += \
./src/channeljsonconfig/channel_json_config.o \
./src/channeljsonconfig/electicity_meter_config.o 

CPP_DEPS += \
./src/channeljsonconfig/channel_json_config.d \
./src/channeljsonconfig/electicity_meter_config.d 


# Each subdirectory must supply rules for building sources it contributes
src/channeljsonconfig/%.o: ../src/channeljsonconfig/%.cpp src/channeljsonconfig/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	$(CXX) -D__DEBUG=1 -DSPROTO_WITHOUT_OUT_BUFFER -DSRPC_WITHOUT_OUT_QUEUE -DUSE_DEPRECATED_EMEV_V1 -D__OPENSSL_TOOLS=1 -D__SSOCKET_WRITE_TO_FILE=$(SSOCKET_WRITE_TO_FILE) -D__BCRYPT=1 -I$(INCMYSQL) -I../src/mqtt -I../src/device -I../src/user -I../src -I$(SSLDIR)/include -I../src/client -O2 -g3 -Wall -fsigned-char -c -fmessage-length=0 -fstack-protector-all -D_FORTIFY_SOURCE=2 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


