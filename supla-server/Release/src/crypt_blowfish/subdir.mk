################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/crypt_blowfish/crypt_blowfish.c \
../src/crypt_blowfish/crypt_gensalt.c \
../src/crypt_blowfish/wrapper.c 

S_UPPER_SRCS += \
../src/crypt_blowfish/x86.S 

OBJS += \
./src/crypt_blowfish/crypt_blowfish.o \
./src/crypt_blowfish/crypt_gensalt.o \
./src/crypt_blowfish/wrapper.o \
./src/crypt_blowfish/x86.o 

C_DEPS += \
./src/crypt_blowfish/crypt_blowfish.d \
./src/crypt_blowfish/crypt_gensalt.d \
./src/crypt_blowfish/wrapper.d 


# Each subdirectory must supply rules for building sources it contributes
src/crypt_blowfish/%.o: ../src/crypt_blowfish/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -D__BCRYPT=1 -DSPROTO_WITHOUT_OUT_BUFFER -DSRPC_WITHOUT_OUT_QUEUE -DSERVER_VERSION_23 -DUSE_DEPRECATED_EMEV_V1 -D__OPENSSL_TOOLS=1 -I$(SSLDIR)/include -I../src/mqtt -I../src -I../src/user -I../src/device -I../src/client -O3 -Wall -fsigned-char -c -fmessage-length=0 -fstack-protector-all -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/crypt_blowfish/x86.o: ../src/crypt_blowfish/x86.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	gcc -c  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


