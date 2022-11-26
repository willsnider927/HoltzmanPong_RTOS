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
#include "math.h"

extern struct HoltzmanData HMs[HM_COUNT];
extern OS_MUTEX hm_mutex;
extern OS_MUTEX platform_mutex;
extern struct PlatformData platform_data;

#define LCD_PERIOD 1

static OS_SEM lcd_semaphore;
static OS_TCB lcdTCB;
static CPU_STK lcdSTK[STACK_SIZES];
static OS_TMR lcdTmr;

static GLIB_Context_t glibContext;

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

  /* Use Normal font */
  GLIB_setFont(&glibContext, (GLIB_Font_t *) &GLIB_FontNormal8x8);

  /* Draw text on the memory lcd display*/
  GLIB_drawStringOnLine(&glibContext,
                        "Welcome to...\n*Holtzman Pong**!",
                        0,
                        GLIB_ALIGN_LEFT,
                        5,
                        5,
                        true);

  /* Draw text on the memory lcd display*/
  GLIB_drawStringOnLine(&glibContext,
                        "",
                        2,
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


void lcd_task() {
  RTOS_ERR semErr;
  RTOS_ERR mutErr;
  RTOS_ERR tmrErr;
  OSTmrStart(&lcdTmr, &tmrErr);
  if (tmrErr.Code != RTOS_ERR_NONE) EFM_ASSERT(false);
  GLIB_Rectangle_t pRectLeft = {
      0,
      0,
      CANYON_START,
      SCREEN_PIXELS -1
  };
  GLIB_Rectangle_t pRectRight = {
      CANYON_END,
      0,
      SCREEN_PIXELS - 1,
      SCREEN_PIXELS -1
  };


  while (1) {
      OSSemPend(&lcd_semaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &semErr);
      if (semErr.Code) EFM_ASSERT(false);

      GLIB_clear(&glibContext);
      draw_sides(&pRectLeft, &pRectRight);

      OSMutexPend(&platform_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &mutErr);
      if (mutErr.Code) EFM_ASSERT(false);
      int platX = (int)round(platform_data.x);
      OSMutexPost(&platform_mutex, OS_OPT_POST_NONE, &mutErr);
      if (mutErr.Code) EFM_ASSERT(false);
      draw_plat(platX);

      OSMutexPend(&hm_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &mutErr);
      if (mutErr.Code) EFM_ASSERT(false);
      for (int i = 0; i < HM_COUNT; i++) {
          draw_hm((int)round(HMs[i].x), (int)round(HMs[i].y));
      }
      OSMutexPost(&hm_mutex, OS_OPT_POST_NONE, &mutErr);
      if (mutErr.Code) EFM_ASSERT(false);

      DMD_updateDisplay();
  }
}

