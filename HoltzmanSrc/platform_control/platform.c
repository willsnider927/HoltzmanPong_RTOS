/*
 * platform.c
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */

#include "platform.h"
#include "capsense.h"

#define PLATFORM_PERIOD 1

int PLATFORM_BOUNCE_ENABLED;
int MAX_SPEED = 100;
OS_TMR platform_timer;
OS_MUTEX platform_mutex;

static OS_SEM platform_semaphore;
static OS_TCB platformTCB;
static CPU_STK platformSTK[STACK_SIZES];

struct PlatformData platform_data;


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
        ABOVE_NORMAL_PRIORITY-1,             /* Task's priority.            */
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

    PLATFORM_BOUNCE_ENABLED = false;
    platform_data.ax = 0;
    platform_data.vx = 0;
    platform_data.x = SCREEN_PIXELS / 2;
}

void platform_task(void) {
  RTOS_ERR semErr;
  RTOS_ERR mutexErr;

  while (1) {
      OSSemPend(&platform_semaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &semErr);
      if (semErr.Code) EFM_ASSERT(false);

      CAPSENSE_Sense();
      int pressed = -1;
      for (int i = 0; i < 4; i++) {
          if (CAPSENSE_getPressed(i)) {
              if (pressed == -1) {
                  pressed = i;
              } else {
                  pressed = -1;
                  break;
              }
          }
      }

      OSMutexPend(&platform_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &mutexErr);
      if (mutexErr.Code) EFM_ASSERT(false);

      switch (pressed) {
        case 0:
          platform_data.ax = -24;
          break;
        case 1:
          platform_data.ax = -12;
          break;
        case 2:
          platform_data.ax = 12;
          break;
        case 3:
          platform_data.ax = 24;
          break;
        default:
          platform_data.ax = 0;
          break;
      }

      OSMutexPost(&platform_mutex, OS_OPT_POST_NONE, &mutexErr);
      if (mutexErr.Code) EFM_ASSERT(false);
  }
}
