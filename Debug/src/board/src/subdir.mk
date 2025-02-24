################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/board/src/board.c 

C_DEPS += \
./src/board/src/board.d 

OBJS += \
./src/board/src/board.o 


# Each subdirectory must supply rules for building sources it contributes
src/board/src/%.o: ../src/board/src/%.c src/board/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -DCORE_M0 -D__USE_ROMDIVIDE -D__LPC11UXX__ -D__USE_CMSIS=CMSISv2p00_LPC11Uxx -D__NEWLIB__ -I../src -I../inc -I../src/freertos/inc -I../src/board/inc -I../lpc_chip_11uxx_lib/inc -I../CMSISv2p00/inc -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-board-2f-src

clean-src-2f-board-2f-src:
	-$(RM) ./src/board/src/board.d ./src/board/src/board.o

.PHONY: clean-src-2f-board-2f-src

