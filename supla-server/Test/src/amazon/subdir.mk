################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/amazon/alexachangereportrequest.cpp \
../src/amazon/alexaclient.cpp \
../src/amazon/alexacredentials.cpp \
../src/amazon/alexarequest.cpp \
../src/amazon/alexaresponserequest.cpp 

OBJS += \
./src/amazon/alexachangereportrequest.o \
./src/amazon/alexaclient.o \
./src/amazon/alexacredentials.o \
./src/amazon/alexarequest.o \
./src/amazon/alexaresponserequest.o 

CPP_DEPS += \
./src/amazon/alexachangereportrequest.d \
./src/amazon/alexaclient.d \
./src/amazon/alexacredentials.d \
./src/amazon/alexarequest.d \
./src/amazon/alexaresponserequest.d 


# Each subdirectory must supply rules for building sources it contributes
src/amazon/%.o: ../src/amazon/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D__DEBUG=1 -DUSE_DEPRECATED_EMEV_V1 -D__TEST=1 -D__OPENSSL_TOOLS=1 -D__BCRYPT=1 -I../src -I$(INCMYSQL) -I../src/user -I../src/device -I../src/client -I$(SSLDIR)/include -I../src/test -O2 -g3 -Wall -fsigned-char -c -fmessage-length=0 -fstack-protector-all -D_FORTIFY_SOURCE=2 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


