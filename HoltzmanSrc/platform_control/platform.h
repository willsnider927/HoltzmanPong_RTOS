/*
 * platform.h
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */

#ifndef PLATFORM_CONTROL_PLATFORM_H_
#define PLATFORM_CONTROL_PLATFORM_H_

#include "stdbool.h"
#include "constants.h"
#include "os.h"

//all in pixels
#define PLATFORM_Y SCREEN_PIXELS- 15
#define PLATFORM_HEIGHT 5
#define PLATFORM_WIDTH 24

#define PLATFORM_MASS 10 //kg
#define MAX_PIXEL_FORCE 100 //kg * px/s^2
#define MAX_FORCE MAX_PIXEL_FORCE * SCREEN_MM / SCREEN_PIXELS //kg * px/s^s
#define MAX_PIXEL_ACCEL MAX_PIXEL_FORCE / PLATFORM_MASS

#define PLATFORM_BOUNCE_ENABLED false
#define PLATFORM_BOUNCE_LIMITED false
#define MAX_BOUNCE_SPEED 0

struct PlatformData {
  double x;
  double vx;
  double ax;
};

void platform_task_create(void);
void platform_task(void);
void platform_timer_cb(void);

#endif /* PLATFORM_CONTROL_PLATFORM_H_ */
