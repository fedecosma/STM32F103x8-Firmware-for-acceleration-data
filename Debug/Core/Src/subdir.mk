################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/callbacks.c \
../Core/Src/command_handler.c \
../Core/Src/dma.c \
../Core/Src/flash_utils.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/lis3dhtr.c \
../Core/Src/main.c \
../Core/Src/rtc.c \
../Core/Src/sensors_data.c \
../Core/Src/serial_parser.c \
../Core/Src/sht40.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/tim.c \
../Core/Src/time_utils.c \
../Core/Src/usart.c \
../Core/Src/usart_io.c 

OBJS += \
./Core/Src/callbacks.o \
./Core/Src/command_handler.o \
./Core/Src/dma.o \
./Core/Src/flash_utils.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/lis3dhtr.o \
./Core/Src/main.o \
./Core/Src/rtc.o \
./Core/Src/sensors_data.o \
./Core/Src/serial_parser.o \
./Core/Src/sht40.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/tim.o \
./Core/Src/time_utils.o \
./Core/Src/usart.o \
./Core/Src/usart_io.o 

C_DEPS += \
./Core/Src/callbacks.d \
./Core/Src/command_handler.d \
./Core/Src/dma.d \
./Core/Src/flash_utils.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/lis3dhtr.d \
./Core/Src/main.d \
./Core/Src/rtc.d \
./Core/Src/sensors_data.d \
./Core/Src/serial_parser.d \
./Core/Src/sht40.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/tim.d \
./Core/Src/time_utils.d \
./Core/Src/usart.d \
./Core/Src/usart_io.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/callbacks.cyclo ./Core/Src/callbacks.d ./Core/Src/callbacks.o ./Core/Src/callbacks.su ./Core/Src/command_handler.cyclo ./Core/Src/command_handler.d ./Core/Src/command_handler.o ./Core/Src/command_handler.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/flash_utils.cyclo ./Core/Src/flash_utils.d ./Core/Src/flash_utils.o ./Core/Src/flash_utils.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/lis3dhtr.cyclo ./Core/Src/lis3dhtr.d ./Core/Src/lis3dhtr.o ./Core/Src/lis3dhtr.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/rtc.cyclo ./Core/Src/rtc.d ./Core/Src/rtc.o ./Core/Src/rtc.su ./Core/Src/sensors_data.cyclo ./Core/Src/sensors_data.d ./Core/Src/sensors_data.o ./Core/Src/sensors_data.su ./Core/Src/serial_parser.cyclo ./Core/Src/serial_parser.d ./Core/Src/serial_parser.o ./Core/Src/serial_parser.su ./Core/Src/sht40.cyclo ./Core/Src/sht40.d ./Core/Src/sht40.o ./Core/Src/sht40.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/time_utils.cyclo ./Core/Src/time_utils.d ./Core/Src/time_utils.o ./Core/Src/time_utils.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/usart_io.cyclo ./Core/Src/usart_io.d ./Core/Src/usart_io.o ./Core/Src/usart_io.su

.PHONY: clean-Core-2f-Src

