#include "sl_event_handler.h"

#include "em_chip.h"
#include "sl_device_init_nvic.h"
#include "sl_board_init.h"
#include "sl_device_init_dcdc.h"
#include "sl_device_init_hfxo.h"
#include "sl_device_init_lfxo.h"
#include "sl_device_init_clocks.h"
#include "sl_device_init_emu.h"
#include "sl_sleeptimer.h"
#include "SEGGER_SYSVIEW.h"
#include "sl_simple_led_instances.h"
#include "cpu.h"
#include "cmsis_os2.h"

void sl_platform_init(void)
{
  CHIP_Init();
  sl_device_init_nvic();
  sl_board_preinit();
  sl_device_init_dcdc();
  sl_device_init_hfxo();
  sl_device_init_lfxo();
  sl_device_init_clocks();
  sl_device_init_emu();
  sl_board_init();
  SEGGER_SYSVIEW_Conf();
  CPU_Init();
  osKernelInitialize();
}

void sl_kernel_start(void)
{
  osKernelStart();
}

void sl_driver_init(void)
{
  sl_simple_led_init_instances();
}

void sl_service_init(void)
{
  sl_sleeptimer_init();
}

void sl_stack_init(void)
{
}

void sl_internal_app_init(void)
{
}

