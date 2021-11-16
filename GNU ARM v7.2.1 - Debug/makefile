################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include drivers/subdir.mk
-include CMSIS/EFM32GG/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Homework.axf

# Tool invocations
Homework.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -T "Homework.ld" -Xlinker --gc-sections -Xlinker -Map="Homework.map" --specs=nano.specs -o Homework.axf "./CMSIS/EFM32GG/startup_gcc_efm32gg.o" "./CMSIS/EFM32GG/system_efm32gg.o" "./drivers/segmentlcd.o" "./emlib/em_cmu.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_lcd.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./main.o" "./src/segmentlcd_individual.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: Homework.hex'
	arm-none-eabi-objcopy -O ihex "Homework.axf" "Homework.hex"
	@echo ' '

	@echo 'Building bin file: Homework.bin'
	arm-none-eabi-objcopy -O binary "Homework.axf" "Homework.bin"
	@echo ' '

	@echo 'Building s37 file: Homework.s37'
	arm-none-eabi-objcopy -O srec "Homework.axf" "Homework.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "Homework.axf" -A
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Homework.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
