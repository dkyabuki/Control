################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../actuator.c \
../control.c \
../global.c \
../main.c \
../sensor.c \
../tasks.c 

OBJS += \
./actuator.o \
./control.o \
./global.o \
./main.o \
./sensor.o \
./tasks.o 

C_DEPS += \
./actuator.d \
./control.d \
./global.d \
./main.d \
./sensor.d \
./tasks.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_GNU_SOURCE -D_REENTRANT -D__XENO__ -I/usr/include/xenomai -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


