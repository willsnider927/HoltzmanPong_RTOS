/*
 * shield.c
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */

#include "shield.h"

static OS_TMR shieldActiveTimer;
static OS_TMR shieldRechargeTimer;
static OS_TCB shieldTCB;
static CPU_STK shieldSTK[STACK_SIZES];

void shield_task_create(void) {
  RTOS_ERR mutexErr;
  RTOS_ERR tskErr;
  RTOS_ERR qErr;
  RTOS_ERR semErr;
  OSMutexCreate(&shield_mutex, "shield_mutex", &mutexErr);
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
  if (qErr.Code || mutexErr.Code || tskErr.Code) EFM_ASSERT(false);

  shield_state.active = false;
  shield_state.recharging = false;
}

void shield_task(void) {
  RTOS_ERR qErr;
  RTOS_ERR tmrErr;
  RTOS_ERR mutexErr;

  uint8_t msg;
  while (1) {
      OSQPend(&shield_msg, 0, OS_OPT_PEND_BLOCKING, &msg, NULL,&qErr);
      if (qErr.Code) EFM_ASSERT(false);
      //TODO shield task
  }
}

