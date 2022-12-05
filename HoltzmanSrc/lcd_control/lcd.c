/*
 * lcd.c
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */
#include "lcd.h"
#include "glib.h"
#include "dmd.h"
#include "sl_board_control.h"
#include "holtzman_masses/hm.h"
#include "platform_control/platform.h"
#include "shield_system/shield.h"
#include "game_management/game_management.h"
#include "math.h"
#include "string.h"

#define LCD_PERIOD 1

OS_TMR lcdTmr;

static OS_SEM lcd_semaphore;
static OS_TCB lcdTCB;
static CPU_STK lcdSTK[STACK_SIZES];
static GLIB_Context_t glibContext;

extern uint8_t HM_COUNT;
extern struct HoltzmanData HMs[];
extern OS_MUTEX hm_mutex;
extern OS_MUTEX platform_mutex;
extern struct PlatformData platform_data;
extern struct ShieldState shield_state;
extern uint8_t laser_charges;
extern enum difficulty_e difficulty;
extern enum game_state_e gameState;
extern int cursor_pos;
extern int PLATFORM_BOUNCE_ENABLED;
extern int score;
extern int high_score;
extern int lives;
extern bool auto_cannon;
extern int laser_fired;
extern char death_cause[];
extern int shotX;
extern int shotY;

void lcd_timer_cb(void) {
  RTOS_ERR semPostErr;
   OSSemPost(
       &lcd_semaphore,
       OS_OPT_POST_1,
       &semPostErr
   );
   if (semPostErr.Code) EFM_ASSERT(false);
}

void LCD_init()
{
  RTOS_ERR semErr;
  RTOS_ERR tmrErr;
  RTOS_ERR tskErr;
  OSSemCreate(
       &lcd_semaphore,
       "lcd Semaphore",
       0,
       &semErr
   );
   OSTmrCreate(
       &lcdTmr,
       "lcd timer",
       0,
       LCD_PERIOD,
       OS_OPT_TMR_PERIODIC,
       &lcd_timer_cb,
       NULL,
       &tmrErr
   );
   OSTaskCreate(
       &lcdTCB,                /* Pointer to the task's TCB.  */
      "lcd Task.",                    /* Name to help debugging.     */
      &lcd_task,                   /* Pointer to the task's code. */
       DEF_NULL,                          /* Pointer to task's argument. */
       NORMAL_PRIORITY,             /* Task's priority.            */
      &lcdSTK[0],             /* Pointer to base of stack.   */
      (STACK_SIZES / 10u),  /* Stack limit, from base.     */
       STACK_SIZES,         /* Stack size, in CPU_STK.     */
       10u,                               /* Messages in task queue.     */
       120u,                                /* Round-Robin time quanta.    */
       DEF_NULL,                          /* External TCB data.          */
       OS_OPT_TASK_STK_CHK,               /* Task options.               */
      &tskErr
    );
   if (semErr.Code || tmrErr.Code || tskErr.Code) EFM_ASSERT(false);



  uint32_t status;
  /* Enable the memory lcd */
  status = sl_board_enable_display();
  EFM_ASSERT(status == SL_STATUS_OK);

  /* Initialize the DMD support for memory lcd display */
  status = DMD_init(0);
  EFM_ASSERT(status == DMD_OK);

  /* Initialize the glib context */
  status = GLIB_contextInit(&glibContext);
  EFM_ASSERT(status == GLIB_OK);

  glibContext.backgroundColor = White;
  glibContext.foregroundColor = Black;

  /* Fill lcd with background color */
  GLIB_clear(&glibContext);

  /* Use Narrow font */
  GLIB_setFont(&glibContext, (GLIB_Font_t *) &GLIB_FontNarrow6x8);

  /* Draw text on the memory lcd display*/
  GLIB_drawStringOnLine(&glibContext,
                        "Welcome to...\n*Holtzman Pong**!",
                        0,
                        GLIB_ALIGN_LEFT,
                        5,
                        5,
                        true);
  /* Post updates to display */
  DMD_updateDisplay();
}

void draw_sides(GLIB_Rectangle_t * pRectLeft, GLIB_Rectangle_t * pRectRight) {
  GLIB_drawRectFilled(&glibContext, pRectLeft);
  GLIB_drawRectFilled(&glibContext, pRectRight);
}

void draw_plat(int x) {
  GLIB_Rectangle_t pRect = {
      x - (PLATFORM_WIDTH / 2),
      PLATFORM_Y - (PLATFORM_HEIGHT/2),
      x + (PLATFORM_WIDTH / 2),
      PLATFORM_Y + (PLATFORM_HEIGHT/2)
  };
  GLIB_drawRectFilled(&glibContext, &pRect);
}

void draw_hm(int x, int y) {
  GLIB_drawCircleFilled(&glibContext, x, y, HM_PIXEL_RADIUS);
}

static GLIB_Rectangle_t pRectLeft = {
    0,
    0,
    CANYON_START,
    SCREEN_PIXELS -1
};
static GLIB_Rectangle_t pRectRight = {
    CANYON_END,
    0,
    SCREEN_PIXELS - 1,
    SCREEN_PIXELS -1
};
static int charge_text_offset = -1 * CANYON_START - 5;
static int lifeXOffset = CANYON_START + HM_PIXEL_RADIUS + 3;
void draw_game(void) {
  RTOS_ERR mutErr;
  draw_sides(&pRectLeft, &pRectRight);

  //draw the platform
  OSMutexPend(&platform_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &mutErr);
  if (mutErr.Code) EFM_ASSERT(false);
  int platX = (int)round(platform_data.x);
  OSMutexPost(&platform_mutex, OS_OPT_POST_NONE, &mutErr);
  if (mutErr.Code) EFM_ASSERT(false);
  draw_plat(platX);

  int lowestY = 0;
  int lowestX = 0;
  //draw the hm
  OSMutexPend(&hm_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &mutErr);
  if (mutErr.Code) EFM_ASSERT(false);
  for (int i = 0; i < HM_COUNT; i++) {
      draw_hm((int)round(HMs[i].x), (int)round(HMs[i].y));
      if (lowestY < (int)HMs[i].y) {
          lowestY = (int)HMs[i].y;
          lowestX = (int)HMs[i].x;
      }
  }
  OSMutexPost(&hm_mutex, OS_OPT_POST_NONE, &mutErr);
  if (mutErr.Code) EFM_ASSERT(false);

  //draw the shield
  if (shield_state.active) {
      GLIB_drawCircle(&glibContext, platX, PLATFORM_Y, (PLATFORM_WIDTH / 2) + 1);
  }

  //draw the cannon
  int xDist = CANYON_END - lowestX;
  int yDist = PLATFORM_Y - lowestY;
  int dist = sqrt((xDist * xDist) + (yDist * yDist));
  int x2 = CANYON_END - (xDist * CANNON_LENGTH / dist);
  int y2 = PLATFORM_Y - (yDist * CANNON_LENGTH / dist);
  for (int i = -5; i <= 5; i ++) {
      GLIB_drawLine(&glibContext, CANYON_END, PLATFORM_Y + i, x2, y2 + i);
  }
  if (laser_fired > 0) {
      GLIB_drawLine(&glibContext, CANYON_END, PLATFORM_Y, shotX, shotY);
      laser_fired--;

  }
  char laser_charges_string[] = "Charges:  ";
  laser_charges_string[9] = '0' + laser_charges;
  GLIB_drawStringOnLine(
      &glibContext,
      laser_charges_string,
      12,
      GLIB_ALIGN_RIGHT,
      charge_text_offset,
      0,
      true
  );

  for (int i = 1; i <= lives; i++) {
      draw_hm(lifeXOffset, (HM_PIXEL_RADIUS + 1) * 2 * i);
  }
}

void draw_game_stopped() {
  char state_string[14];
  sprintf(state_string, gameState == GAME_OVER ? "Game Over" : "Holtzman Pong");
  char difficulty_string[14];
  char bounce_string[14];
  char start_string[8];
  char cannon_string[18];
  char score_string[9];
  char hs_string[20];
  switch (difficulty) {
    case EASY:
      sprintf(difficulty_string,  "%sLevel: Easy", cursor_pos == 0 ? ">" : " ");
      break;
    case MEDIUM:
      sprintf(difficulty_string, "%sLevel: Medium", cursor_pos == 0 ? ">" : " ");
      break;
    case HARD:
      sprintf(difficulty_string, "%sLevel: Hard", cursor_pos == 0 ? ">" : " ");
      break;
    case INSANE:
      sprintf(difficulty_string, "%sLevel: Insane", cursor_pos == 0 ? ">" : " ");
      break;
    default:
      EFM_ASSERT(false);
      break;
  }
  if (PLATFORM_BOUNCE_ENABLED) {
      sprintf(bounce_string, "%sBounce:On", cursor_pos == 1 ? ">" : " ");
  } else {
      sprintf(bounce_string, "%sBounce:Off", cursor_pos == 1 ? ">" : " ");
  }
  if (auto_cannon) {
      sprintf(cannon_string, "%sAuto Cannon:On", cursor_pos == 2 ? ">" : " ");
  } else {
      sprintf(cannon_string, "%sAuto Cannon:Off", cursor_pos == 2 ? ">" : " ");
  }
  if (cursor_pos == 3) {
      sprintf(start_string, ">Start!");
  } else {
      sprintf(start_string, "Start!");
  }
  if (gameState == GAME_OVER) {
      sprintf(score_string, "Score: %d", score);
      sprintf(hs_string, "High Score: %d", high_score);
      GLIB_drawStringOnLine(
          &glibContext,
          score_string,
          0,
          GLIB_ALIGN_LEFT,
          0,
          0,
          true
      );
      GLIB_drawStringOnLine(
          &glibContext,
          hs_string,
          1,
          GLIB_ALIGN_LEFT,
          0,
          0,
          true
      );
  }

  GLIB_drawStringOnLine(
        &glibContext,
        state_string,
        3,
        GLIB_ALIGN_CENTER,
        0,
        0,
        true
  );
  GLIB_drawStringOnLine(
          &glibContext,
          death_cause,
          4,
          GLIB_ALIGN_CENTER,
          0,
          0,
          true
  );
  GLIB_drawStringOnLine(
        &glibContext,
        difficulty_string,
        5,
        GLIB_ALIGN_CENTER,
        0,
        0,
        true
  );
  GLIB_drawStringOnLine(
        &glibContext,
        bounce_string,
        6,
        GLIB_ALIGN_CENTER,
        0,
        0,
        true
  );
  GLIB_drawStringOnLine(
          &glibContext,
          cannon_string,
          7,
          GLIB_ALIGN_CENTER,
          0,
          0,
          true
  );
  GLIB_drawStringOnLine(
        &glibContext,
        start_string,
        8,
        GLIB_ALIGN_CENTER,
        0,
        0,
        true
  );
}


void lcd_task() {
  RTOS_ERR semErr;
  RTOS_ERR tmrErr;

  GLIB_setFont(&glibContext, (GLIB_Font_t *) &GLIB_FontNormal8x8);
  OSTmrStart(&lcdTmr, &tmrErr);
  if (tmrErr.Code != RTOS_ERR_NONE) EFM_ASSERT(false);

  while (1) {
      OSSemPend(&lcd_semaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &semErr);
      if (semErr.Code) EFM_ASSERT(false);

      GLIB_clear(&glibContext);
      if (gameState == IN_PROGRESS) {
          draw_game();
      } else {
          draw_game_stopped();
      }
      DMD_updateDisplay();
  }
}

