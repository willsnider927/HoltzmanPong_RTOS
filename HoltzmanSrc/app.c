/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

#include <gpio.h>
#include <capsense.h>
#include "os.h"
#include "em_emu.h"
#include "os_trace.h"
#include "stdio.h"
#include "holtzman_masses/hm.h"
#include "laser_system/laser.h"
#include "lcd_control/lcd.h"
#include "physics/engine.h"
#include "physics/engine_test.h"
#include "platform_control/platform.h"
#include "shield_system/shield.h"
#include "led_control/led.h"
#include "constants.h"

/***************************************************************************//**
 * Task configuration data.
 ******************************************************************************/

OS_TCB idleTCB;
CPU_STK idleStk[STACK_SIZES];

void App_IdleTask(void);


/***************************************************************************//**
 * Tank Init functions
 ******************************************************************************/

void  App_IdleTaskCreate (void) {
    RTOS_ERR     err;

    OSTaskCreate(&idleTCB,                /* Pointer to the task's TCB.  */
                 "idle Task.",                    /* Name to help debugging.     */
                 &App_IdleTask,                   /* Pointer to the task's code. */
                  DEF_NULL,                          /* Pointer to task's argument. */
                  BELOW_NORMAL_PRIORITY,             /* Task's priority.            */
                 &idleStk[0],             /* Pointer to base of stack.   */
                 (STACK_SIZES / 10u),  /* Stack limit, from base.     */
                  STACK_SIZES,         /* Stack size, in CPU_STK.     */
                  10u,                               /* Messages in task queue.     */
                  0u,                                /* Round-Robin time quanta.    */
                  DEF_NULL,                          /* External TCB data.          */
                  OS_OPT_TASK_STK_CHK,               /* Task options.               */
                 &err);
    if (err.Code != RTOS_ERR_NONE) {
        /* Handle error on task creation. */
        EFM_ASSERT(false);
    }
}
/***************************************************************************//**
 * Tank functions
***************************************************************************/


void App_IdleTask(void) {
  RTOS_ERR timeDlyError;
  while(1) {
      EMU_EnterEM1();
      OSTimeDly(50, OS_OPT_TIME_DLY, &timeDlyError);
  }
}

/*****************************************************************************/


void app_init(void)
{
  // Initialize GPIO
  gpio_open();

  // Initialize our capactive touch sensor driver!
  CAPSENSE_Init();

  // Initialize our tasks

  hm_init();
  laser_task_create();
  physics_task_create();
  platform_task_create();
  shield_task_create();
  led_task_create();
  LCD_init();
  App_IdleTaskCreate();

  //tests
  physics_test_driver();
}

