################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/supla-client-lib/suplasinglecall.cpp 

C_SRCS += \
../src/supla-client-lib/cfg.c \
../src/supla-client-lib/eh.c \
../src/supla-client-lib/ini.c \
../src/supla-client-lib/lck.c \
../src/supla-client-lib/log.c \
../src/supla-client-lib/proto.c \
../src/supla-client-lib/safearray.c \
../src/supla-client-lib/srpc.c \
../src/supla-client-lib/sthread.c \
../src/supla-client-lib/supla-client.c \
../src/supla-client-lib/supla-socket.c \
../src/supla-client-lib/tools.c 

CPP_DEPS += \
./src/supla-client-lib/suplasinglecall.d 

C_DEPS += \
./src/supla-client-lib/cfg.d \
./src/supla-client-lib/eh.d \
./src/supla-client-lib/ini.d \
./src/supla-client-lib/lck.d \
./src/supla-client-lib/log.d \
./src/supla-client-lib/proto.d \
./src/supla-client-lib/safearray.d \
./src/supla-client-lib/srpc.d \
./src/supla-client-lib/sthread.d \
./src/supla-client-lib/supla-client.d \
./src/supla-client-lib/supla-socket.d \
./src/supla-client-lib/tools.d 

OBJS += \
./src/supla-client-lib/cfg.o \
./src/supla-client-lib/eh.o \
./src/supla-client-lib/ini.o \
./src/supla-client-lib/lck.o \
./src/supla-client-lib/log.o \
./src/supla-client-lib/proto.o \
./src/supla-client-lib/safearray.o \
./src/supla-client-lib/srpc.o \
./src/supla-client-lib/sthread.o \
./src/supla-client-lib/supla-client.o \
./src/supla-client-lib/supla-socket.o \
./src/supla-client-lib/suplasinglecall.o \
./src/supla-client-lib/tools.o 


# Each subdirectory must supply rules for building sources it contributes
src/supla-client-lib/%.o: ../src/supla-client-lib/%.c src/supla-client-lib/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -D__DEBUG=1 -DNOMYSQL=1 -D__OPENSSL_TOOLS=1 -D__SSOCKET_WRITE_TO_FILE=$(SSOCKET_WRITE_TO_FILE) -I../src/supla-client-lib -I../src/test -I$(SSLDIR)/include -O0 -g3 -Wall -fsigned-char  -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/supla-client-lib/%.o: ../src/supla-client-lib/%.cpp src/supla-client-lib/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D__DEBUG=1 -DNOMYSQL=1 -D__OPENSSL_TOOLS=1 -D__SSOCKET_WRITE_TO_FILE=$(SSOCKET_WRITE_TO_FILE) -I$(SSLDIR)/include -I../src/supla-client-lib -I../src/test -O0 -g3 -Wall -fsigned-char  -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-supla-2d-client-2d-lib

clean-src-2f-supla-2d-client-2d-lib:
	-$(RM) ./src/supla-client-lib/cfg.d ./src/supla-client-lib/cfg.o ./src/supla-client-lib/eh.d ./src/supla-client-lib/eh.o ./src/supla-client-lib/ini.d ./src/supla-client-lib/ini.o ./src/supla-client-lib/lck.d ./src/supla-client-lib/lck.o ./src/supla-client-lib/log.d ./src/supla-client-lib/log.o ./src/supla-client-lib/proto.d ./src/supla-client-lib/proto.o ./src/supla-client-lib/safearray.d ./src/supla-client-lib/safearray.o ./src/supla-client-lib/srpc.d ./src/supla-client-lib/srpc.o ./src/supla-client-lib/sthread.d ./src/supla-client-lib/sthread.o ./src/supla-client-lib/supla-client.d ./src/supla-client-lib/supla-client.o ./src/supla-client-lib/supla-socket.d ./src/supla-client-lib/supla-socket.o ./src/supla-client-lib/suplasinglecall.d ./src/supla-client-lib/suplasinglecall.o ./src/supla-client-lib/tools.d ./src/supla-client-lib/tools.o

.PHONY: clean-src-2f-supla-2d-client-2d-lib

