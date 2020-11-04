################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/client/client.cpp \
../src/client/clientchannel.cpp \
../src/client/clientchannelgroup.cpp \
../src/client/clientchannelgrouprelation.cpp \
../src/client/clientchannelgroups.cpp \
../src/client/clientchannelgroupvalue.cpp \
../src/client/clientchannels.cpp \
../src/client/clientlocation.cpp \
../src/client/clientobjcontainer.cpp \
../src/client/clientobjcontaineritem.cpp 

OBJS += \
./src/client/client.o \
./src/client/clientchannel.o \
./src/client/clientchannelgroup.o \
./src/client/clientchannelgrouprelation.o \
./src/client/clientchannelgroups.o \
./src/client/clientchannelgroupvalue.o \
./src/client/clientchannels.o \
./src/client/clientlocation.o \
./src/client/clientobjcontainer.o \
./src/client/clientobjcontaineritem.o 

CPP_DEPS += \
./src/client/client.d \
./src/client/clientchannel.d \
./src/client/clientchannelgroup.d \
./src/client/clientchannelgrouprelation.d \
./src/client/clientchannelgroups.d \
./src/client/clientchannelgroupvalue.d \
./src/client/clientchannels.d \
./src/client/clientlocation.d \
./src/client/clientobjcontainer.d \
./src/client/clientobjcontaineritem.d 


# Each subdirectory must supply rules for building sources it contributes
src/client/%.o: ../src/client/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D__DEBUG=1 -DSERVER_VERSION_23 -DUSE_DEPRECATED_EMEV_V1 -D__TEST=1 -D__OPENSSL_TOOLS=1 -D__BCRYPT=1 -I../src -I../src/mqtt -I$(INCMYSQL) -I../src/user -I../src/device -I../src/client -I$(SSLDIR)/include -I../src/test -O2 -g3 -Wall -fsigned-char -c -fmessage-length=0 -fstack-protector-all -D_FORTIFY_SOURCE=2 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


