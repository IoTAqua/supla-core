################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ipc/abstract_get_char_command.cpp \
../src/ipc/abstract_ipc_command.cpp \
../src/ipc/abstract_ipc_ctrl.cpp \
../src/ipc/abstract_ipc_socket_adapter.cpp \
../src/ipc/get_char_command.cpp \
../src/ipc/ipc_ctrl.cpp 

CPP_DEPS += \
./src/ipc/abstract_get_char_command.d \
./src/ipc/abstract_ipc_command.d \
./src/ipc/abstract_ipc_ctrl.d \
./src/ipc/abstract_ipc_socket_adapter.d \
./src/ipc/get_char_command.d \
./src/ipc/ipc_ctrl.d 

OBJS += \
./src/ipc/abstract_get_char_command.o \
./src/ipc/abstract_ipc_command.o \
./src/ipc/abstract_ipc_ctrl.o \
./src/ipc/abstract_ipc_socket_adapter.o \
./src/ipc/get_char_command.o \
./src/ipc/ipc_ctrl.o 


# Each subdirectory must supply rules for building sources it contributes
src/ipc/%.o: ../src/ipc/%.cpp src/ipc/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	$(CXX) -D__DEBUG=1 -DSPROTO_WITHOUT_OUT_BUFFER -DSRPC_WITHOUT_OUT_QUEUE -DUSE_DEPRECATED_EMEV_V1 -D__OPENSSL_TOOLS=1 -D__SSOCKET_WRITE_TO_FILE=$(SSOCKET_WRITE_TO_FILE) -D__BCRYPT=1 -I$(INCMYSQL) -I../src/mqtt -I../src/device -I../src/user -I../src -I$(SSLDIR)/include -I../src/client -O2 -g3 -Wall -fsigned-char -c -fmessage-length=0 -fstack-protector-all -D_FORTIFY_SOURCE=2 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-ipc

clean-src-2f-ipc:
	-$(RM) ./src/ipc/abstract_get_char_command.d ./src/ipc/abstract_get_char_command.o ./src/ipc/abstract_ipc_command.d ./src/ipc/abstract_ipc_command.o ./src/ipc/abstract_ipc_ctrl.d ./src/ipc/abstract_ipc_ctrl.o ./src/ipc/abstract_ipc_socket_adapter.d ./src/ipc/abstract_ipc_socket_adapter.o ./src/ipc/get_char_command.d ./src/ipc/get_char_command.o ./src/ipc/ipc_ctrl.d ./src/ipc/ipc_ctrl.o

.PHONY: clean-src-2f-ipc

