/*
 * led.c
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */


#include "led.h"
#include "platform_control/platform.h"
#include "gpio.h"
#include "shield_system/shield.h"
#include "math.h"

OS_TMR ledTmrs[3];

static OS_TCB ledTCB;
static CPU_STK ledSTK[STACK_SIZES];
static OS_SEM led_semaphore;

extern struct PlatformData platform_data;
extern int MAX_SPEED;

void led_timer_cb(void) {
  RTOS_ERR semErr;
  OSSemPost(&led_semaphore, OS_OPT_POST_1 ,&semErr);
  if(semErr.Code) EFM_ASSERT(false);
}

void led_task_create() {
  RTOS_ERR semErr;
  RTOS_ERR tskErr;
  RTOS_ERR tmrErr;
  OSSemCreate(
       &led_semaphore,
       "led Semaphore",
       0,
       &semErr
   );
   OSTaskCreate(
       &ledTCB,                /* Pointer to the task's TCB.  */
      "led Task.",                    /* Name to help debugging.     */
      &led_task,                   /* Pointer to the task's code. */
       DEF_NULL,                          /* Pointer to task's argument. */
       NORMAL_PRIORITY,             /* Task's priority.            */
      &ledSTK[0],             /* Pointer to base of stack.   */
      (STACK_SIZES / 10u),  /* Stack limit, from base.     */
       STACK_SIZES,         /* Stack size, in CPU_STK.     */
       10u,                               /* Messages in task queue.     */
       120u,                                /* Round-Robin time quanta.    */
       DEF_NULL,                          /* External TCB data.          */
       OS_OPT_TASK_STK_CHK,               /* Task options.               */
      &tskErr
    );
   OSTmrCreate(
      &ledTmrs[0],
      "led timer0",
      0,
      10,
      OS_OPT_TMR_PERIODIC,
      &led_timer_cb,
      NULL,
      &tmrErr
  );
   OSTmrCreate(
       &ledTmrs[1],
       "led timer1",
       0,
       5,
       OS_OPT_TMR_PERIODIC,
       &led_timer_cb,
       NULL,
       &tmrErr
   );
   OSTmrCreate(
       &ledTmrs[2],
       "led timer2",
       0,
       1,
       OS_OPT_TMR_PERIODIC,
       &led_timer_cb,
       NULL,
       &tmrErr
   );
   if (semErr.Code || tskErr.Code || tmrErr.Code) EFM_ASSERT(false);
}

void led_task() {
  RTOS_ERR semErr;
  RTOS_ERR tmrErr;

  while (1) {
      OSSemPend(&led_semaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &semErr);
      if (semErr.Code) EFM_ASSERT(false);
      int led_percent = fmin(2, round((fabs(platform_data.vx) / MAX_SPEED) * 2.0));
      for (int i = 0; i < 3; i++) {
          if (led_percent == i) {
              if (OSTmrStateGet(&ledTmrs[i], &tmrErr) != OS_TMR_STATE_RUNNING) {
                  OSTmrStart(&ledTmrs[i], &tmrErr);
              }
              if (tmrErr.Code) EFM_ASSERT(false);
          } else {
              if (OSTmrStateGet(&ledTmrs[i], &tmrErr) != OS_TMR_STATE_STOPPED) {
                  OSTmrStop(&ledTmrs[i], OS_OPT_TMR_NONE, NULL, &tmrErr);
              }
              if (tmrErr.Code) EFM_ASSERT(false);
          }
      }
      toggle_led();
  }
}
