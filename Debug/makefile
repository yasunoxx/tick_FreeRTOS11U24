################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/freertos/src/subdir.mk
-include src/board/src/subdir.mk
-include src/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := tick_FreeRTOS11U24
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: tick_FreeRTOS11U24.axf

# Tool invocations
tick_FreeRTOS11U24.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L../CMSISv2p00/Debug -L../lpc_chip_11uxx_lib/libs -Xlinker -Map="tick_FreeRTOS11U24.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0 -mthumb -T tick_FreeRTOS11U24_Debug.ld -o "tick_FreeRTOS11U24.axf" $(OBJS) $(USER_OBJS) $(LIBS) -l:libCMSISv2p00_LPC11Uxx.a -l:libusbd_11uxx_lib.a
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) tick_FreeRTOS11U24.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "tick_FreeRTOS11U24.axf" ; arm-none-eabi-objcopy -v -O binary "tick_FreeRTOS11U24.axf" "tick_FreeRTOS11U24.bin" ; checksum -p LPC11U24_301 -d "tick_FreeRTOS11U24.bin"
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
