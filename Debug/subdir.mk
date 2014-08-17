################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AppImg.c \
../AppInit.c \
../AppInput.c \
../global_var.c \
../main.c 

OBJS += \
./AppImg.o \
./AppInit.o \
./AppInput.o \
./global_var.o \
./main.o 

C_DEPS += \
./AppImg.d \
./AppInit.d \
./AppInput.d \
./global_var.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include/SDL -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


