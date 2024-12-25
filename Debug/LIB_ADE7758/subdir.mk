################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LIB_ADE7758/Lib7758.c 

OBJS += \
./LIB_ADE7758/Lib7758.o 

C_DEPS += \
./LIB_ADE7758/Lib7758.d 


# Each subdirectory must supply rules for building sources it contributes
LIB_ADE7758/%.o LIB_ADE7758/%.su LIB_ADE7758/%.cyclo: ../LIB_ADE7758/%.c LIB_ADE7758/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/2.PROJECT/10.KitADE7758/3.SOURCECODE/EXAMPL_ADE7758/LIB_ADE7758" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-LIB_ADE7758

clean-LIB_ADE7758:
	-$(RM) ./LIB_ADE7758/Lib7758.cyclo ./LIB_ADE7758/Lib7758.d ./LIB_ADE7758/Lib7758.o ./LIB_ADE7758/Lib7758.su

.PHONY: clean-LIB_ADE7758

