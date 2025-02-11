################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/freertos/src/FreeRTOSCommonHooks.c \
../src/freertos/src/croutine.c \
../src/freertos/src/heap_1.c \
../src/freertos/src/list.c \
../src/freertos/src/port.c \
../src/freertos/src/queue.c \
../src/freertos/src/tasks.c \
../src/freertos/src/timers.c 

C_DEPS += \
./src/freertos/src/FreeRTOSCommonHooks.d \
./src/freertos/src/croutine.d \
./src/freertos/src/heap_1.d \
./src/freertos/src/list.d \
./src/freertos/src/port.d \
./src/freertos/src/queue.d \
./src/freertos/src/tasks.d \
./src/freertos/src/timers.d 

OBJS += \
./src/freertos/src/FreeRTOSCommonHooks.o \
./src/freertos/src/croutine.o \
./src/freertos/src/heap_1.o \
./src/freertos/src/list.o \
./src/freertos/src/port.o \
./src/freertos/src/queue.o \
./src/freertos/src/tasks.o \
./src/freertos/src/timers.o 


# Each subdirectory must supply rules for building sources it contributes
src/freertos/src/%.o: ../src/freertos/src/%.c src/freertos/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -DCORE_M0 -D__USE_ROMDIVIDE -D__LPC11UXX__ -D__USE_CMSIS=CMSISv2p00_LPC11Uxx -D__NEWLIB__ -I../src -I../inc -I../src/freertos/inc -I../src/board/inc -I../lpc_chip_11uxx_lib/inc -I../CMSISv2p00/inc -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-freertos-2f-src

clean-src-2f-freertos-2f-src:
	-$(RM) ./src/freertos/src/FreeRTOSCommonHooks.d ./src/freertos/src/FreeRTOSCommonHooks.o ./src/freertos/src/croutine.d ./src/freertos/src/croutine.o ./src/freertos/src/heap_1.d ./src/freertos/src/heap_1.o ./src/freertos/src/list.d ./src/freertos/src/list.o ./src/freertos/src/port.d ./src/freertos/src/port.o ./src/freertos/src/queue.d ./src/freertos/src/queue.o ./src/freertos/src/tasks.d ./src/freertos/src/tasks.o ./src/freertos/src/timers.d ./src/freertos/src/timers.o

.PHONY: clean-src-2f-freertos-2f-src

