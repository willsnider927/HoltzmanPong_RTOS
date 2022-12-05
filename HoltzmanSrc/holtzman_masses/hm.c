/*
 * hm.c
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */


#include "hm.h"
#include "em_assert.h"
#include "stdlib.h"

uint8_t HM_COUNT = 1;
struct HoltzmanData HMs[2];
OS_MUTEX hm_mutex;

void generate_hm(int idx) {
  HMs[idx].x = (rand() % (CANYON_SIZE_PIXELS - (2 * HM_PIXEL_RADIUS))) + HM_PIXEL_RADIUS + CANYON_START;
  HMs[idx].y = HM_PIXEL_RADIUS;
  HMs[idx].mode = 0;
  HMs[idx].vx = (rand() % 20) - 10;
  HMs[idx].vy = rand() % 10;
}

void hm_init() {
  RTOS_ERR mutexErr;
  OSMutexCreate(&hm_mutex, "hm_mutex", &mutexErr);
  if (mutexErr.Code) EFM_ASSERT(false);
  for (int i = 0; i < HM_COUNT; i++) {
      generate_hm(i);
  }
}


