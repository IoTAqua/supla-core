################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cfg.c \
../src/eh.c \
../src/ini.c \
../src/jsmn.c \
../src/lck.c \
../src/log.c \
../src/safearray.c \
../src/schedulercfg.c \
../src/sthread.c \
../src/tools.c 

CPP_SRCS += \
../src/abstract_worker.cpp \
../src/action.cpp \
../src/action_openclose.cpp \
../src/action_rgb.cpp \
../src/action_shutreveal.cpp \
../src/action_turn_onoff.cpp \
../src/database.cpp \
../src/dbcommon.cpp \
../src/ipcclient.cpp \
../src/queue.cpp \
../src/supla-scheduler.cpp \
../src/worker.cpp 

OBJS += \
./src/abstract_worker.o \
./src/action.o \
./src/action_openclose.o \
./src/action_rgb.o \
./src/action_shutreveal.o \
./src/action_turn_onoff.o \
./src/cfg.o \
./src/database.o \
./src/dbcommon.o \
./src/eh.o \
./src/ini.o \
./src/ipcclient.o \
./src/jsmn.o \
./src/lck.o \
./src/log.o \
./src/queue.o \
./src/safearray.o \
./src/schedulercfg.o \
./src/sthread.o \
./src/supla-scheduler.o \
./src/tools.o \
./src/worker.o 

C_DEPS += \
./src/cfg.d \
./src/eh.d \
./src/ini.d \
./src/jsmn.d \
./src/lck.d \
./src/log.d \
./src/safearray.d \
./src/schedulercfg.d \
./src/sthread.d \
./src/tools.d 

CPP_DEPS += \
./src/abstract_worker.d \
./src/action.d \
./src/action_openclose.d \
./src/action_rgb.d \
./src/action_shutreveal.d \
./src/action_turn_onoff.d \
./src/database.d \
./src/dbcommon.d \
./src/ipcclient.d \
./src/queue.d \
./src/supla-scheduler.d \
./src/worker.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I$(INCMYSQL) -O3 -Wall -fsigned-char -c -fmessage-length=0 -fstack-protector-all -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I$(INCMYSQL) -O3 -Wall -fsigned-char -c -fmessage-length=0 -fstack-protector-all -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


