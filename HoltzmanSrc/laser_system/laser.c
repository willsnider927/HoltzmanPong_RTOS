/*
 * laser.c
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */
#include "laser.h"
#include "../holtzman_masses/hm.h"

uint8_t laser_charges = 5;
OS_SEM laser_semaphore;
bool auto_cannon = true;
int laser_fired = 0;
int shotX;
int shotY;

static OS_TCB laserTCB;
static CPU_STK laserSTK[STACK_SIZES];

extern uint8_t HM_COUNT;
extern struct HoltzmanData HMs[];
extern OS_MUTEX hm_mutex;
extern int score;

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
bool shoot_laser(int idx) {
  if (laser_charges > 0) {
    laser_charges--;
    score++;
    laser_fired = 3;
    shotX = HMs[idx].x;
    shotY = HMs[idx].y;
    RTOS_ERR mutexErr;
    OSMutexPend(&hm_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &mutexErr);
    if (mutexErr.Code) EFM_ASSERT(false);

    generate_hm(idx);

    OSMutexPost(&hm_mutex, OS_OPT_POST_NONE, &mutexErr);
    if (mutexErr.Code) EFM_ASSERT(false);
    return true;
  }
  return false;
}

void laser_task(void) {
  RTOS_ERR semErr;
  RTOS_ERR mutexErr;
  while (1) {
      OSSemPend(&laser_semaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &semErr);
      if (semErr.Code) EFM_ASSERT(false);
      //TODO check laser count, if good, decrement and pend hm mutex and respawn it
      int lowestIdx = 0;
      int lowestHeight = 0; //lowest is highest y
      for (int i = 0; i < HM_COUNT; i++) {
          if (lowestHeight < HMs[i].y) {
              lowestHeight = HMs[i].y;
              lowestIdx = i;
          }
      }
      shoot_laser(lowestIdx);
  }
}


