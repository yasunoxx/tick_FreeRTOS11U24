################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/clock_11xx.c \
../src/cr_startup_lpc11xx.c \
../src/freertos_blinky.c \
../src/glcd.c \
../src/i2c_11xx.c \
../src/iic.c \
../src/iocon_11xx.c \
../src/premain.c \
../src/privgpio.c \
../src/rtc.c \
../src/sysctl_11xx.c \
../src/sysinit_Bb-LPC11U24custom.c \
../src/task_lcd.c \
../src/task_led.c 

C_DEPS += \
./src/clock_11xx.d \
./src/cr_startup_lpc11xx.d \
./src/freertos_blinky.d \
./src/glcd.d \
./src/i2c_11xx.d \
./src/iic.d \
./src/iocon_11xx.d \
./src/premain.d \
./src/privgpio.d \
./src/rtc.d \
./src/sysctl_11xx.d \
./src/sysinit_Bb-LPC11U24custom.d \
./src/task_lcd.d \
./src/task_led.d 

OBJS += \
./src/clock_11xx.o \
./src/cr_startup_lpc11xx.o \
./src/freertos_blinky.o \
./src/glcd.o \
./src/i2c_11xx.o \
./src/iic.o \
./src/iocon_11xx.o \
./src/premain.o \
./src/privgpio.o \
./src/rtc.o \
./src/sysctl_11xx.o \
./src/sysinit_Bb-LPC11U24custom.o \
./src/task_lcd.o \
./src/task_led.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -DCORE_M0 -D__USE_ROMDIVIDE -D__LPC11UXX__ -D__USE_CMSIS=CMSISv2p00_LPC11Uxx -D__NEWLIB__ -I../src -I../inc -I../src/freertos/inc -I../src/board/inc -I../lpc_chip_11uxx_lib/inc -I../CMSISv2p00/inc -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/clock_11xx.d ./src/clock_11xx.o ./src/cr_startup_lpc11xx.d ./src/cr_startup_lpc11xx.o ./src/freertos_blinky.d ./src/freertos_blinky.o ./src/glcd.d ./src/glcd.o ./src/i2c_11xx.d ./src/i2c_11xx.o ./src/iic.d ./src/iic.o ./src/iocon_11xx.d ./src/iocon_11xx.o ./src/premain.d ./src/premain.o ./src/privgpio.d ./src/privgpio.o ./src/rtc.d ./src/rtc.o ./src/sysctl_11xx.d ./src/sysctl_11xx.o ./src/sysinit_Bb-LPC11U24custom.d ./src/sysinit_Bb-LPC11U24custom.o ./src/task_lcd.d ./src/task_lcd.o ./src/task_led.d ./src/task_led.o

.PHONY: clean-src

