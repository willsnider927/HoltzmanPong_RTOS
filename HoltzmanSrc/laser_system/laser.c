/*
 * laser.c
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */
#include "laser.h"
#include "../holtzman_masses/hm.h"

static OS_TCB laserTCB;
static CPU_STK laserSTK[STACK_SIZES];

void laser_task_create(void) {
  RTOS_ERR semErr;
  RTOS_ERR tskErr;
  OSSemCreate(
      &laser_semaphore,
      "laser semaphore",
      0,
      &semErr
  );
  OSTaskCreate(
         &laserTCB,                /* Pointer to the task's TCB.  */
        "laser Task.",                    /* Name to help debugging.     */
        &laser_task,                   /* Pointer to the task's code. */
         DEF_NULL,                          /* Pointer to task's argument. */
         NORMAL_PRIORITY,             /* Task's priority.            */
        &laserSTK[0],             /* Pointer to base of stack.   */
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

void laser_task(void) {
  RTOS_ERR semErr;
  RTOS_ERR mutErr;
  while (1) {
      OSSemPend(&laser_semaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &semErr);
      if (semErr.Code) EFM_ASSERT(false);
      //TODO check laser count, if good, decrement and pend hm mutex and respawn it
  }
}


