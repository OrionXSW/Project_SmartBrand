################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Hardware/LED.c \
../Hardware/UART.c 

OBJS += \
./Hardware/LED.o \
./Hardware/UART.o 

C_DEPS += \
./Hardware/LED.d \
./Hardware/UART.d 


# Each subdirectory must supply rules for building sources it contributes
Hardware/%.o Hardware/%.su Hardware/%.cyclo: ../Hardware/%.c Hardware/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Hardware -I../System -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Hardware

clean-Hardware:
	-$(RM) ./Hardware/LED.cyclo ./Hardware/LED.d ./Hardware/LED.o ./Hardware/LED.su ./Hardware/UART.cyclo ./Hardware/UART.d ./Hardware/UART.o ./Hardware/UART.su

.PHONY: clean-Hardware

