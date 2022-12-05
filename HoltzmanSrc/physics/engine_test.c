/*
 * engine_test.c
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */

#include "engine.h"
#include "engine_test.h"
#include "holtzman_masses/hm.h"
#include "platform_control/platform.h"
#include "shield_system/shield.h"

extern int GRAVITY_PIXELS;

void test_hms_check_repelled_no_shield() {
  struct HoltzmanData hms[] = {
      {
          0, //vx
          30, //vy
          5, //x
          PLATFORM_Y + 1, //y
          0
      },
  };
  struct PlatformData plat_data = {
      5, //x
      0, //vx
      0 //ax
  };
  struct ShieldState shieldDat = {
        false,
        false
    };
  check_hms_vertical(hms, &plat_data, &shieldDat);
  EFM_ASSERT(hms[0].vy == 30 * PASSIVE_KINETIC_REDUCTION);
}

void test_hms_check_repelled_shield() {
  struct HoltzmanData hms[] = {
      {
          0, //vx
          30, //vy
          5, //x
          PLATFORM_Y, //y
          0
      },
  };
  struct PlatformData plat_data = {
      5, //x
      0, //vx
      0 //ax
  };
  struct ShieldState shieldDat = {
      true,
      false
  };
  check_hms_vertical(hms, &plat_data, &shieldDat);
  EFM_ASSERT(hms[0].vy == 30 * ACTIVE_KINETIC_GAIN);
}

void test_platform_update(void) {
  struct PlatformData plat_data = {
      64, //x
      2, //vx
      1 //ax
  };
  update_platform(&plat_data);
  EFM_ASSERT(plat_data.x < (64 + (2 * 2 * PHYSICS_DELTA)) && plat_data.x > 64);
  EFM_ASSERT(plat_data.vx < (2 + (2 * PHYSICS_DELTA)) && plat_data.vx > 2);
}

void test_hms_update(void) {
  struct HoltzmanData hms[] = {
        {
            20, //vx
            5, //vy
            64, //x
            64, //y
            0
        },
   };
  update_hms(hms);
  EFM_ASSERT(hms[0].vx == 20);
  EFM_ASSERT(hms[0].vy == 5 + (GRAVITY_PIXELS * PHYSICS_DELTA));
  EFM_ASSERT(hms[0].x == 64 + (20 * PHYSICS_DELTA));
  EFM_ASSERT(hms[0].y == 64 + (5 * PHYSICS_DELTA));
}

void test_hms_update_wall(void) {
  struct HoltzmanData hms[] = {
          {
              20, //vx
              0, //vy
              129, //x
              64, //y
              0
          },
     };
    update_hms(hms);
    EFM_ASSERT(hms[0].vx == -20);
}


void physics_test_driver(void) {
  test_hms_check_repelled_no_shield();
  test_hms_check_repelled_shield();
  test_platform_update();
  test_hms_update();
  test_hms_update_wall();
}


//All 5 tests are passing



