################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/http/httprequest.cpp \
../src/http/httprequestqueue.cpp \
../src/http/trivialhttp.cpp \
../src/http/trivialhttpfactory.cpp \
../src/http/trivialhttps.cpp 

OBJS += \
./src/http/httprequest.o \
./src/http/httprequestqueue.o \
./src/http/trivialhttp.o \
./src/http/trivialhttpfactory.o \
./src/http/trivialhttps.o 

CPP_DEPS += \
./src/http/httprequest.d \
./src/http/httprequestqueue.d \
./src/http/trivialhttp.d \
./src/http/trivialhttpfactory.d \
./src/http/trivialhttps.d 


# Each subdirectory must supply rules for building sources it contributes
src/http/%.o: ../src/http/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D__DEBUG=1 -DUSE_DEPRECATED_EMEV_V1 -D__TEST=1 -D__OPENSSL_TOOLS=1 -D__BCRYPT=1 -I../src -I$(INCMYSQL) -I../src/user -I../src/device -I../src/client -I$(SSLDIR)/include -I../src/test -O2 -g3 -Wall -fsigned-char -c -fmessage-length=0 -fstack-protector-all -D_FORTIFY_SOURCE=2 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


