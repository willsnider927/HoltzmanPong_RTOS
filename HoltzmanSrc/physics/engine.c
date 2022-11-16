/*
 * engine.c
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */

#include "engine.h"
#include "shield_system/shield.h"
#include "math.h"

static OS_SEM physics_semaphore;
static OS_TMR physics_timer;
static OS_TCB physicsTCB;
static CPU_STK physicsSTK[STACK_SIZES];

void physics_timer_cb(void) {
  RTOS_ERR semPostErr;
   OSSemPost(
       &physics_semaphore,
       OS_OPT_POST_1,
       &semPostErr
   );
   if (semPostErr.Code) EFM_ASSERT(false);
}

void physics_task_create(void) {
  RTOS_ERR semErr;
  RTOS_ERR tmrErr;
  RTOS_ERR tskErr;
  OSSemCreate(
       &physics_semaphore,
       "physics Semaphore",
       0,
       &semErr
   );
   OSTmrCreate(
       &physics_timer,
       "physics timer",
       0,
       PHYSICS_PERIOD,
       OS_OPT_TMR_PERIODIC,
       &physics_timer_cb,
       NULL,
       &tmrErr
   );

   OSTaskCreate(
       &physicsTCB,                /* Pointer to the task's TCB.  */
      "physics Task.",                    /* Name to help debugging.     */
      &physics_task,                   /* Pointer to the task's code. */
       DEF_NULL,                          /* Pointer to task's argument. */
       NORMAL_PRIORITY,             /* Task's priority.            */
      &physicsSTK[0],             /* Pointer to base of stack.   */
      (STACK_SIZES / 10u),  /* Stack limit, from base.     */
       STACK_SIZES,         /* Stack size, in CPU_STK.     */
       10u,                               /* Messages in task queue.     */
       120u,                                /* Round-Robin time quanta.    */
       DEF_NULL,                          /* External TCB data.          */
       OS_OPT_TASK_STK_CHK,               /* Task options.               */
      &tskErr
    );
   if (semErr.Code || tmrErr.Code || tskErr.Code) EFM_ASSERT(false);
}

void physics_task(void) {
  RTOS_ERR semErr;
  RTOS_ERR mutErr;
  //TODO START TIMER
  while(1) {
      OSSemPend(&physics_semaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &semErr);
      if (semErr.Code) EFM_ASSERT(false);

      update_hms(HMs);
      update_platform(&platform_data);
      check_hms_vertical(HMs, &platform_data);
  }
}

void update_hms(struct HoltzmanData hms[]) {
  RTOS_ERR mutexErr;
  OSMutexPend(&hm_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &mutexErr);
  if (mutexErr.Code) EFM_ASSERT(false);
  for (int i = 0; i < HM_COUNT; i++) {
      hms[i].x += hms[i].vx;
      hms[i].y += hms[i].vy;
      hms[i].vy += GRAVITY_PIXELS * PHYSICS_DELTA;
      if ((hms[i].x - HM_PIXEL_RADIUS) < CANYON_START) {
          hms[i].vx = abs(hms[i].vx);
      } else if ((hms[i].x + HM_PIXEL_RADIUS) > CANYON_END) {
          hms[i].vx = -1 * abs(HMs[i].vx);
      }
  }
  OSMutexPost(&hm_mutex, OS_OPT_POST_NONE, &mutexErr);
  if (mutexErr.Code) EFM_ASSERT(false);
}

void update_platform(struct PlatformData * plat_data) {
  RTOS_ERR mutexErr;
  OSMutexPend(&platform_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &mutexErr);
  if (mutexErr.Code) EFM_ASSERT(false);
  plat_data->x += plat_data->vx ;
  plat_data->vx += plat_data->ax * PHYSICS_DELTA;
  if ((plat_data->x - (PLATFORM_WIDTH/2)) < CANYON_START) {
      if (PLATFORM_BOUNCE_ENABLED) {
          plat_data->ax = 0;
          plat_data->vx = fmax(fabs(plat_data->vx), PLATFORM_BOUNCE_LIMITED * MAX_BOUNCE_SPEED);
      } else {
          plat_data->ax = 0;
          plat_data->vx = 0;
          plat_data->x = CANYON_START + PLATFORM_WIDTH/2;
      }
  } else if ((plat_data->x + (PLATFORM_WIDTH/2)) > CANYON_END) {
      if (PLATFORM_BOUNCE_ENABLED) {
          plat_data->ax = 0;
          plat_data->vx = fmin(-1 * fabs(platform_data.vx), PLATFORM_BOUNCE_LIMITED * MAX_BOUNCE_SPEED * -1);
      } else {
          plat_data->ax = 0;
          plat_data->vx = 0;
          plat_data->x = CANYON_END - PLATFORM_WIDTH/2;
      }
  }
  OSMutexPost(&platform_mutex, OS_OPT_POST_NONE, &mutexErr);
  if (mutexErr.Code) EFM_ASSERT(false);
}

void check_hms_vertical(struct HoltzmanData hms[], struct PlatformData* plat_data) {
  RTOS_ERR mutexErr;
  for (int i = 0; i < HM_COUNT; i++) {
      if ((hms[i].y + HM_PIXEL_RADIUS) >= PLATFORM_Y && hms[i].vy > 0) { //reached the platforms y
          if (hms[i].vy > GAME_OVER_SPEED &&
              hms[i].x < (plat_data->x + (PLATFORM_WIDTH / 2)) &&
              hms[i].x > (plat_data->x - (PLATFORM_WIDTH/2))) { //between the platform bounds

              hms[i].vy *= shield_state.active ? ACTIVE_KINETIC_GAIN : PASSIVE_KINETIC_REDUCTION;
          } else {
              //TODO game over
          }
      }
      else if (hms[i].y < 0) { //hm reached the top and is continuing to harkonnens
          OSMutexPend(&hm_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &mutexErr);
          if (mutexErr.Code) EFM_ASSERT(false);

          generate_hm(i);

          OSMutexPost(&hm_mutex, OS_OPT_POST_NONE, &mutexErr);
          if (mutexErr.Code) EFM_ASSERT(false);
      }
  }
}