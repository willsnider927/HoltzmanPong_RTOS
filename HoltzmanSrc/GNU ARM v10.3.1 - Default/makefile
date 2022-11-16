################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include shield_system/subdir.mk
-include platform_control/subdir.mk
-include physics/subdir.mk
-include led_control/subdir.mk
-include lcd_control/subdir.mk
-include laser_system/subdir.mk
-include holtzman_masses/subdir.mk
-include gecko_sdk_3.2.3/util/third_party/segger/systemview/Sample/MicriumOSKernel/Config/Cortex-M/subdir.mk
-include gecko_sdk_3.2.3/util/third_party/segger/systemview/Sample/MicriumOSKernel/subdir.mk
-include gecko_sdk_3.2.3/util/third_party/segger/systemview/SEGGER/subdir.mk
-include gecko_sdk_3.2.3/platform/service/udelay/src/subdir.mk
-include gecko_sdk_3.2.3/platform/service/system/src/subdir.mk
-include gecko_sdk_3.2.3/platform/service/sleeptimer/src/subdir.mk
-include gecko_sdk_3.2.3/platform/service/device_init/src/subdir.mk
-include gecko_sdk_3.2.3/platform/middleware/glib/glib/subdir.mk
-include gecko_sdk_3.2.3/platform/middleware/glib/dmd/display/subdir.mk
-include gecko_sdk_3.2.3/platform/micrium_os/ports/source/gnu/subdir.mk
-include gecko_sdk_3.2.3/platform/micrium_os/ports/source/generic/subdir.mk
-include gecko_sdk_3.2.3/platform/micrium_os/kernel/source/subdir.mk
-include gecko_sdk_3.2.3/platform/micrium_os/cpu/source/subdir.mk
-include gecko_sdk_3.2.3/platform/micrium_os/common/source/rtos/subdir.mk
-include gecko_sdk_3.2.3/platform/micrium_os/common/source/ring_buf/subdir.mk
-include gecko_sdk_3.2.3/platform/micrium_os/common/source/logging/subdir.mk
-include gecko_sdk_3.2.3/platform/micrium_os/common/source/lib/subdir.mk
-include gecko_sdk_3.2.3/platform/micrium_os/common/source/kal/subdir.mk
-include gecko_sdk_3.2.3/platform/emlib/src/subdir.mk
-include gecko_sdk_3.2.3/platform/driver/leddrv/src/subdir.mk
-include gecko_sdk_3.2.3/platform/common/toolchain/src/subdir.mk
-include gecko_sdk_3.2.3/platform/common/src/subdir.mk
-include gecko_sdk_3.2.3/platform/Device/SiliconLabs/EFM32PG12B/Source/GCC/subdir.mk
-include gecko_sdk_3.2.3/platform/Device/SiliconLabs/EFM32PG12B/Source/subdir.mk
-include gecko_sdk_3.2.3/platform/CMSIS/RTOS2/Source/subdir.mk
-include gecko_sdk_3.2.3/hardware/driver/memlcd/src/memlcd_usart/subdir.mk
-include gecko_sdk_3.2.3/hardware/driver/memlcd/src/subdir.mk
-include gecko_sdk_3.2.3/hardware/board/src/subdir.mk
-include autogen/subdir.mk
-include subdir.mk
-include objects.mk

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


BUILD_ARTIFACT_NAME := HoltzmanSrc
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: HoltzmanSrc.axf

# Tool invocations
HoltzmanSrc.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "/Users/willsnider/SimplicityStudio/v5_workspace/HoltzmanSrc/autogen/linkerfile.ld" -Xlinker --gc-sections -Xlinker -Map="HoltzmanSrc.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o HoltzmanSrc.axf -Wl,--start-group "./app.o" "./capsense.o" "./gpio.o" "./main.o" "./autogen/sl_board_default_init.o" "./autogen/sl_device_init_clocks.o" "./autogen/sl_event_handler.o" "./autogen/sl_simple_led_instances.o" "./gecko_sdk_3.2.3/hardware/board/src/sl_board_control_gpio.o" "./gecko_sdk_3.2.3/hardware/board/src/sl_board_init.o" "./gecko_sdk_3.2.3/hardware/driver/memlcd/src/memlcd_usart/sl_memlcd_spi.o" "./gecko_sdk_3.2.3/hardware/driver/memlcd/src/sl_memlcd.o" "./gecko_sdk_3.2.3/hardware/driver/memlcd/src/sl_memlcd_display.o" "./gecko_sdk_3.2.3/platform/CMSIS/RTOS2/Source/os_systick.o" "./gecko_sdk_3.2.3/platform/Device/SiliconLabs/EFM32PG12B/Source/GCC/startup_efm32pg12b.o" "./gecko_sdk_3.2.3/platform/Device/SiliconLabs/EFM32PG12B/Source/system_efm32pg12b.o" "./gecko_sdk_3.2.3/platform/common/src/sl_slist.o" "./gecko_sdk_3.2.3/platform/common/toolchain/src/sl_memory.o" "./gecko_sdk_3.2.3/platform/driver/leddrv/src/sl_led.o" "./gecko_sdk_3.2.3/platform/driver/leddrv/src/sl_simple_led.o" "./gecko_sdk_3.2.3/platform/emlib/src/em_acmp.o" "./gecko_sdk_3.2.3/platform/emlib/src/em_assert.o" "./gecko_sdk_3.2.3/platform/emlib/src/em_cmu.o" "./gecko_sdk_3.2.3/platform/emlib/src/em_core.o" "./gecko_sdk_3.2.3/platform/emlib/src/em_emu.o" "./gecko_sdk_3.2.3/platform/emlib/src/em_gpio.o" "./gecko_sdk_3.2.3/platform/emlib/src/em_rtcc.o" "./gecko_sdk_3.2.3/platform/emlib/src/em_system.o" "./gecko_sdk_3.2.3/platform/emlib/src/em_usart.o" "./gecko_sdk_3.2.3/platform/micrium_os/common/source/kal/kal_kernel.o" "./gecko_sdk_3.2.3/platform/micrium_os/common/source/lib/lib_ascii.o" "./gecko_sdk_3.2.3/platform/micrium_os/common/source/lib/lib_mem.o" "./gecko_sdk_3.2.3/platform/micrium_os/common/source/lib/lib_str.o" "./gecko_sdk_3.2.3/platform/micrium_os/common/source/logging/logging.o" "./gecko_sdk_3.2.3/platform/micrium_os/common/source/ring_buf/ring_buf.o" "./gecko_sdk_3.2.3/platform/micrium_os/common/source/rtos/rtos_err_str.o" "./gecko_sdk_3.2.3/platform/micrium_os/common/source/rtos/rtos_utils.o" "./gecko_sdk_3.2.3/platform/micrium_os/cpu/source/cpu_core.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/cmsis_os2.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_cfg_app.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_core.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_dbg.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_flag.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_mon.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_msg.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_mutex.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_prio.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_q.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_sem.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_stat.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_task.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_time.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_tmr.o" "./gecko_sdk_3.2.3/platform/micrium_os/kernel/source/os_var.o" "./gecko_sdk_3.2.3/platform/micrium_os/ports/source/generic/arm_cpu_dwt_ts.o" "./gecko_sdk_3.2.3/platform/micrium_os/ports/source/gnu/armv7m_cpu_a.o" "./gecko_sdk_3.2.3/platform/micrium_os/ports/source/gnu/armv7m_os_cpu_a.o" "./gecko_sdk_3.2.3/platform/micrium_os/ports/source/gnu/armv7m_os_cpu_c.o" "./gecko_sdk_3.2.3/platform/middleware/glib/dmd/display/dmd_memlcd.o" "./gecko_sdk_3.2.3/platform/middleware/glib/glib/bmp.o" "./gecko_sdk_3.2.3/platform/middleware/glib/glib/glib.o" "./gecko_sdk_3.2.3/platform/middleware/glib/glib/glib_bitmap.o" "./gecko_sdk_3.2.3/platform/middleware/glib/glib/glib_circle.o" "./gecko_sdk_3.2.3/platform/middleware/glib/glib/glib_font_narrow_6x8.o" "./gecko_sdk_3.2.3/platform/middleware/glib/glib/glib_font_normal_8x8.o" "./gecko_sdk_3.2.3/platform/middleware/glib/glib/glib_font_number_16x20.o" "./gecko_sdk_3.2.3/platform/middleware/glib/glib/glib_line.o" "./gecko_sdk_3.2.3/platform/middleware/glib/glib/glib_polygon.o" "./gecko_sdk_3.2.3/platform/middleware/glib/glib/glib_rectangle.o" "./gecko_sdk_3.2.3/platform/middleware/glib/glib/glib_string.o" "./gecko_sdk_3.2.3/platform/service/device_init/src/sl_device_init_dcdc_s1.o" "./gecko_sdk_3.2.3/platform/service/device_init/src/sl_device_init_emu_s1.o" "./gecko_sdk_3.2.3/platform/service/device_init/src/sl_device_init_hfxo_s1.o" "./gecko_sdk_3.2.3/platform/service/device_init/src/sl_device_init_lfxo_s1.o" "./gecko_sdk_3.2.3/platform/service/device_init/src/sl_device_init_nvic.o" "./gecko_sdk_3.2.3/platform/service/sleeptimer/src/sl_sleeptimer.o" "./gecko_sdk_3.2.3/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtcc.o" "./gecko_sdk_3.2.3/platform/service/system/src/sl_system_init.o" "./gecko_sdk_3.2.3/platform/service/system/src/sl_system_kernel.o" "./gecko_sdk_3.2.3/platform/service/udelay/src/sl_udelay.o" "./gecko_sdk_3.2.3/platform/service/udelay/src/sl_udelay_armv6m_gcc.o" "./gecko_sdk_3.2.3/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.o" "./gecko_sdk_3.2.3/util/third_party/segger/systemview/SEGGER/SEGGER_SYSVIEW.o" "./gecko_sdk_3.2.3/util/third_party/segger/systemview/Sample/MicriumOSKernel/Config/Cortex-M/SEGGER_SYSVIEW_Config_MicriumOSKernel.o" "./gecko_sdk_3.2.3/util/third_party/segger/systemview/Sample/MicriumOSKernel/SEGGER_SYSVIEW_MicriumOSKernel.o" "./holtzman_masses/hm.o" "./laser_system/laser.o" "./lcd_control/lcd.o" "./led_control/led.o" "./physics/engine.o" "./physics/engine_test.o" "./platform_control/platform.o" "./shield_system/shield.o" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: HoltzmanSrc.hex'
	arm-none-eabi-objcopy -O ihex "HoltzmanSrc.axf" "HoltzmanSrc.hex"
	@echo ' '

	@echo 'Building bin file: HoltzmanSrc.bin'
	arm-none-eabi-objcopy -O binary "HoltzmanSrc.axf" "HoltzmanSrc.bin"
	@echo ' '

	@echo 'Building s37 file: HoltzmanSrc.s37'
	arm-none-eabi-objcopy -O srec "HoltzmanSrc.axf" "HoltzmanSrc.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "HoltzmanSrc.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) HoltzmanSrc.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
