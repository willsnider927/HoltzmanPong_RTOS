/*
 * platform.c
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */

#include "platform.h"
#include "capsense.h"

#define PLATFORM_PERIOD 5

static OS_SEM platform_semaphore;
static OS_TMR platform_timer;
static OS_TCB platformTCB;
static CPU_STK platformSTK[STACK_SIZES];


void platform_timer_cb(void) {
  RTOS_ERR semPostErr;
   OSSemPost(
       &platform_semaphore,
       OS_OPT_POST_1,
       &semPostErr
   );
   if (semPostErr.Code) EFM_ASSERT(false);
}

void platform_task_create(void) {
  RTOS_ERR semErr;
  RTOS_ERR tmrErr;
  RTOS_ERR tskErr;
  RTOS_ERR mutexErr;
  OSSemCreate(
       &platform_semaphore,
       "platform Semaphore",
       0,
       &semErr
   );
   OSTmrCreate(
       &platform_timer,
       "platform timer",
       0,
       PLATFORM_PERIOD,
       OS_OPT_TMR_PERIODIC,
       &platform_timer_cb,
       NULL,
       &tmrErr
   );
   OSTaskCreate(
        &platformTCB,                /* Pointer to the task's TCB.  */
       "platform Task.",                    /* Name to help debugging.     */
       &platform_task,                   /* Pointer to the task's code. */
        DEF_NULL,                          /* Pointer to task's argument. */
        NORMAL_PRIORITY,             /* Task's priority.            */
       &platformSTK[0],             /* Pointer to base of stack.   */
       (STACK_SIZES / 10u),  /* Stack limit, from base.     */
        STACK_SIZES,         /* Stack size, in CPU_STK.     */
        10u,                               /* Messages in task queue.     */
        120u,                                /* Round-Robin time quanta.    */
        DEF_NULL,                          /* External TCB data.          */
        OS_OPT_TASK_STK_CHK,               /* Task options.               */
       &tskErr
     );
    OSMutexCreate(&platform_mutex, "platform_mutex", &mutexErr);
    if (semErr.Code || tmrErr.Code || tskErr.Code || mutexErr.Code) EFM_ASSERT(false);

    platform_data.ax = 0;
    platform_data.vx = 0;
    platform_data.x = SCREEN_PIXELS / 2;
}

void platform_task(void) {
  RTOS_ERR semErr;
  RTOS_ERR mutErr;

  //TODO START TIMER
  while (1) {
      OSSemPend(&platform_semaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &semErr);
      if (semErr.Code) EFM_ASSERT(false);
      //TODO read capsense and update acceleration
  }
}
