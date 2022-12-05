/*
 * game_management.c
 *
 *  Created on: Dec 2, 2022
 *      Author: willsnider
 */

#include "game_management.h"
#include "os.h"
#include "constants.h"
#include "holtzman_masses/hm.h"
#include "platform_control/platform.h"
#include "string.h"
#include "stdlib.h"

OS_FLAG_GRP game_state;
OS_Q btn_q;
enum game_state_e gameState = PREGAME;
enum difficulty_e difficulty;
int cursor_pos = 0;
char death_cause[32] = "";
int score = 0;
int high_score = 0;
int lives = 3;


static OS_TCB gameTCB;
static CPU_STK gameSTK[STACK_SIZES];

extern OS_TMR ledTmrs[];
extern OS_TMR physics_timer;
extern OS_TMR platform_timer;
extern int PLATFORM_BOUNCE_ENABLED;
extern struct PlatformData platform_data;
extern uint8_t laser_charges;
extern uint8_t HM_COUNT;
extern int GRAVITY_PIXELS;
extern int MAX_SPEED;
extern bool auto_cannon;


void game_over(char cause[]) {
  RTOS_ERR flgErr;
  RTOS_ERR tmrErr;
  gameState = GAME_OVER;
  cursor_pos = 0;
  if (score > high_score) high_score = score;
  strcpy(death_cause, cause);
  for (int i = 0; i < 3; i++) {
      if(OSTmrStateGet(&ledTmrs[i], &tmrErr) == OS_TMR_STATE_RUNNING) {
          OSTmrStop(&ledTmrs[i], OS_OPT_TMR_NONE, NULL, &tmrErr);
          if (tmrErr.Code) EFM_ASSERT(false);
      }
  }
  OSTmrStop(&physics_timer, OS_OPT_TMR_NONE, NULL, &tmrErr);
  if (tmrErr.Code) EFM_ASSERT(false);
  OSTmrStop(&platform_timer, OS_OPT_TMR_NONE, NULL, &tmrErr);
  if (tmrErr.Code) EFM_ASSERT(false);
  OSFlagPost(&game_state, GAME_OVER, OS_OPT_POST_FLAG_SET, &flgErr);
  if (flgErr.Code) EFM_ASSERT(false);
  OSFlagPost(&game_state, IN_PROGRESS, OS_OPT_POST_FLAG_CLR, &flgErr);
  if (flgErr.Code) EFM_ASSERT(false);
}

void decrement_life(void) {
  lives--;
  if (lives == 0) {
      game_over("HM Hit");
  }
}

void start_game() {
  RTOS_ERR tmrErr;
  RTOS_ERR flgErr;

  platform_data.ax = 0;
  platform_data.vx = 0;
  platform_data.x = SCREEN_PIXELS / 2;
  laser_charges = 5;
  lives = 3;
  score = 0;
  hm_init();
  gameState = IN_PROGRESS;

  OSTmrStart(&ledTmrs[0], &tmrErr);
  if (tmrErr.Code != RTOS_ERR_NONE) EFM_ASSERT(false);
  OSTmrStart(&physics_timer, &tmrErr);
  OSFlagPost(&game_state, IN_PROGRESS, OS_OPT_POST_FLAG_SET, &flgErr);
  if (tmrErr.Code != RTOS_ERR_NONE || flgErr.Code) EFM_ASSERT(false);
  OSTmrStart(&platform_timer, &tmrErr);
  OSFlagPost(&game_state, PREGAME | GAME_OVER, OS_OPT_POST_FLAG_CLR, &flgErr);
  if (flgErr.Code || tmrErr.Code) EFM_ASSERT(false);
}

void game_stopped_task_create(void) {
  RTOS_ERR tskErr;
  RTOS_ERR flgErr;
  RTOS_ERR qErr;
  OSTaskCreate(
     &gameTCB,                /* Pointer to the task's TCB.  */
    "game Task.",                    /* Name to help debugging.     */
    &game_stopped_task,                   /* Pointer to the task's code. */
     DEF_NULL,                          /* Pointer to task's argument. */
     ABOVE_NORMAL_PRIORITY,             /* Task's priority.            */
    &gameSTK[0],             /* Pointer to base of stack.   */
    (STACK_SIZES / 10u),  /* Stack limit, from base.     */
     STACK_SIZES,         /* Stack size, in CPU_STK.     */
     10u,                               /* Messages in task queue.     */
     120u,                                /* Round-Robin time quanta.    */
     DEF_NULL,                          /* External TCB data.          */
     OS_OPT_TASK_STK_CHK,               /* Task options.               */
    &tskErr
  );
  OSFlagCreate(
      &game_state,
      "game state flags",
      PREGAME,
      &flgErr
  );
  OSQCreate(
      &btn_q,
      "Q for managing game state",
      4,
      &qErr
  );
  if (flgErr.Code || tskErr.Code || qErr.Code) EFM_ASSERT(false);
}

void update_difficulty(void) {
  switch (difficulty) {
      case EASY:
        difficulty = MEDIUM;
        MAX_SPEED = 50;
        break;
      case MEDIUM:
        difficulty = HARD;
        HM_COUNT = 2;
        break;
      case HARD:
        difficulty = INSANE;
        GRAVITY_PIXELS = 35;
        break;
      case INSANE:
        difficulty = EASY;
        HM_COUNT = 1;
        MAX_SPEED = 100;
        GRAVITY_PIXELS = 25;
        break;
      default:
        EFM_ASSERT(false);
        break;
  }
}

void game_stopped_task(void) {
  RTOS_ERR flgErr;
  RTOS_ERR qErr;

  uint8_t * btnMsg;
  uint8_t * size;
  while (1) {
      while (OSFlagPend(&game_state, PREGAME | GAME_OVER, 0, OS_OPT_PEND_FLAG_SET_ANY, NULL, &flgErr) != IN_PROGRESS) {
          btnMsg = OSQPend(&btn_q, 0, OS_OPT_PEND_BLOCKING, &size, NULL,&qErr);
          if (qErr.Code) EFM_ASSERT(false);
          if(*btnMsg == 1) {
              cursor_pos = (cursor_pos + 1) % 4;
          } else {
              switch (cursor_pos) {
                case 0:
                  update_difficulty();
                  break;
                case 1:
                  PLATFORM_BOUNCE_ENABLED = !PLATFORM_BOUNCE_ENABLED;
                  break;
                case 2:
                  auto_cannon = !auto_cannon;
                  break;
                case 3:
                  start_game();
                  break;
                default:
                  EFM_ASSERT(false);
                  break;
              }
          }
          free(btnMsg);
      }
  }
}
