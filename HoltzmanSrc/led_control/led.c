/*
 * led.c
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */


#include "led.h"
#include "platform_control/platform.h"

static OS_TCB ledTCB;
static CPU_STK ledSTK[STACK_SIZES];

void led_task_create() {
  RTOS_ERR semErr;
  RTOS_ERR tskErr;
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
   if (semErr.Code || tskErr.Code) EFM_ASSERT(false);
}

void led_task() {
  RTOS_ERR semErr;

  while (1) {
      OSSemPend(&led_semaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &semErr);
      if (semErr.Code) EFM_ASSERT(false);
  }
}
