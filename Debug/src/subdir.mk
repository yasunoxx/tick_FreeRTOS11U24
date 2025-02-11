################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/chip_11xx.c \
../src/clock_11xx.c \
../src/cr_startup_lpc11xx.c \
../src/freertos_blinky.c \
../src/glcd.c \
../src/gpio_11xx_1.c \
../src/i2c_11xx.c \
../src/iocon_11xx.c \
../src/premain.c \
../src/privgpio.c \
../src/sysctl_11xx.c \
../src/sysinit.c 

C_DEPS += \
./src/chip_11xx.d \
./src/clock_11xx.d \
./src/cr_startup_lpc11xx.d \
./src/freertos_blinky.d \
./src/glcd.d \
./src/gpio_11xx_1.d \
./src/i2c_11xx.d \
./src/iocon_11xx.d \
./src/premain.d \
./src/privgpio.d \
./src/sysctl_11xx.d \
./src/sysinit.d 

OBJS += \
./src/chip_11xx.o \
./src/clock_11xx.o \
./src/cr_startup_lpc11xx.o \
./src/freertos_blinky.o \
./src/glcd.o \
./src/gpio_11xx_1.o \
./src/i2c_11xx.o \
./src/iocon_11xx.o \
./src/premain.o \
./src/privgpio.o \
./src/sysctl_11xx.o \
./src/sysinit.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -DCORE_M0 -D__USE_ROMDIVIDE -D__LPC11UXX__ -D__USE_CMSIS=CMSISv2p00_LPC11Uxx -D__NEWLIB__ -I../src -I../inc -I../src/freertos/inc -I../src/board/inc -I../lpc_chip_11uxx_lib/inc -I../CMSISv2p00/inc -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/chip_11xx.d ./src/chip_11xx.o ./src/clock_11xx.d ./src/clock_11xx.o ./src/cr_startup_lpc11xx.d ./src/cr_startup_lpc11xx.o ./src/freertos_blinky.d ./src/freertos_blinky.o ./src/glcd.d ./src/glcd.o ./src/gpio_11xx_1.d ./src/gpio_11xx_1.o ./src/i2c_11xx.d ./src/i2c_11xx.o ./src/iocon_11xx.d ./src/iocon_11xx.o ./src/premain.d ./src/premain.o ./src/privgpio.d ./src/privgpio.o ./src/sysctl_11xx.d ./src/sysctl_11xx.o ./src/sysinit.d ./src/sysinit.o

.PHONY: clean-src

