/*
 * hm.h
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */

#ifndef HOLTZMAN_MASSES_HM_H_
#define HOLTZMAN_MASSES_HM_H_

#include "stdbool.h"
#include "os.h"
#include "constants.h"

#define HM_COUNT 1
#define HM_PIXEL_RADIUS 3
#define HOLTZMAN_DIAMETER (2*HM_PIXEL_RADIUS) * SCREEN_MM / SCREEN_PIXELS
#define SIDE_HIT_KINETIC_REDUCTION 0.75


struct HoltzmanData {
  int vx;
  int vy;
  int x;
  int y;
  uint8_t mode;
};

static struct HoltzmanData HMs[HM_COUNT];

static OS_MUTEX hm_mutex;

void generate_hm(int idx);
void hm_init(void);

#endif /* HOLTZMAN_MASSES_HM_H_ */

