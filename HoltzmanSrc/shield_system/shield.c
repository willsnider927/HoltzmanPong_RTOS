/*
 * shield.c
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */

#include "shield.h"

OS_Q shield_msg;
struct ShieldState shield_state;
OS_TMR shieldRechargeTimer;

static OS_TMR shieldActiveTimer;
static OS_TCB shieldTCB;
static CPU_STK shieldSTK[STACK_SIZES];
static OS_MUTEX shield_mutex;

void shield_active_cb(void) {
  RTOS_ERR tmrErr;
  RTOS_ERR mutexErr;
  OSMutexPend(&shield_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &mutexErr);
  if (mutexErr.Code) EFM_ASSERT(false);

  if (shield_state.active) {
      shield_state.active = false;
      shield_state.recharging = true;
      OSTmrStart(&shieldRechargeTimer, &tmrErr);
      if (tmrErr.Code) EFM_ASSERT(false);
  }

  OSMutexPost(&shield_mutex, OS_OPT_POST_NONE, &mutexErr);
  if (mutexErr.Code) EFM_ASSERT(false);
}

void shield_recharge_cb(void) {
  RTOS_ERR mutexErr;
  OSMutexPend(&shield_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &mutexErr);
  if (mutexErr.Code) EFM_ASSERT(false);

  if (shield_state.recharging) {
      shield_state.recharging = false;
  }

  OSMutexPost(&shield_mutex, OS_OPT_POST_NONE, &mutexErr);
  if (mutexErr.Code) EFM_ASSERT(false);
}

void shield_task_create(void) {
  RTOS_ERR mutexErr;
  RTOS_ERR tskErr;
  RTOS_ERR qErr;
  RTOS_ERR semErr;
  RTOS_ERR tmrErr;
  OSTaskCreate(
     &shieldTCB,                /* Pointer to the task's TCB.  */
    "shield Task.",                    /* Name to help debugging.     */
    &shield_task,                   /* Pointer to the task's code. */
     DEF_NULL,                          /* Pointer to task's argument. */
     NORMAL_PRIORITY,             /* Task's priority.            */
    &shieldSTK[0],             /* Pointer to base of stack.   */
    (STACK_SIZES / 10u),  /* Stack limit, from base.     */
     STACK_SIZES,         /* Stack size, in CPU_STK.     */
     10u,                               /* Messages in task queue.     */
     120u,                                /* Round-Robin time quanta.    */
     DEF_NULL,                          /* External TCB data.          */
     OS_OPT_TASK_STK_CHK,               /* Task options.               */
    &tskErr
  );
  OSQCreate(
      &shield_msg,
      "shield Message queue",
      4,
      &qErr
  );
  OSTmrCreate(
      &shieldActiveTimer,
      "active timer",
      SHIELD_ACTIVE_100MS,
      0,
      OS_OPT_TMR_ONE_SHOT,
      &shield_active_cb,
      NULL,
      &tmrErr
  );
  OSTmrCreate(
      &shieldRechargeTimer,
      "recharge timer",
      SHIELD_RECHARGE_100MS,
      0,
      OS_OPT_TMR_ONE_SHOT,
      &shield_recharge_cb,
      NULL,
      &tmrErr
  );
  OSMutexCreate(
      &shield_mutex,
      "shield Mutex",
      &mutexErr
  );
  if (qErr.Code || mutexErr.Code || tskErr.Code || tmrErr.Code) EFM_ASSERT(false);

  shield_state.active = false;
  shield_state.recharging = false;
}

void shield_task(void) {
  RTOS_ERR qErr;
  RTOS_ERR tmrErr;
  RTOS_ERR mutexErr;

  uint8_t * msg;
  uint8_t * size;
  while (1) {
      msg = OSQPend(&shield_msg, 0, OS_OPT_PEND_BLOCKING, &size, NULL,&qErr);
      if (qErr.Code) EFM_ASSERT(false);
      //TODO shield task
      OSMutexPend(&shield_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &mutexErr);
      if (mutexErr.Code) EFM_ASSERT(false);

      if (*msg == 1 && !shield_state.recharging) {
          shield_state.active = true;
          OSTmrStart(&shieldActiveTimer, &tmrErr);
          if (tmrErr.Code) EFM_ASSERT(false);
      } else if (*msg == 0 && shield_state.active) {
          shield_state.active = false;
          shield_state.recharging = true;
          OSTmrStart(&shieldRechargeTimer, &tmrErr);
          if (tmrErr.Code) EFM_ASSERT(false);
      }

      OSMutexPost(&shield_mutex, OS_OPT_POST_NONE, &mutexErr);
      if (mutexErr.Code) EFM_ASSERT(false);

      free(msg);
  }
}

