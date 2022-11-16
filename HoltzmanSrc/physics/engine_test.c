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


void test_hms_check_return_to_harkonnen() {
  struct HoltzmanData hms[] = {
      {
          0, //vx
          0, //vy
          0, //x
          -2, //y
          0
      },
  };
  struct PlatformData plat_data = {
      0, //x
      0, //vx
      0 //ax
  };
  check_hms_vertical(hms, &plat_data);
  EFM_ASSERT(hms[0].y == 0); //we know it was caught and reset
}

void test_hms_check_repelled_no_shield() {
  struct HoltzmanData hms[] = {
      {
          0, //vx
          10, //vy
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
  check_hms_vertical(hms, &plat_data);
  EFM_ASSERT(hms[0].vy == 10 * PASSIVE_KINETIC_REDUCTION);
}

void test_hms_check_repelled_shield() {
  struct HoltzmanData hms[] = {
      {
          0, //vx
          10, //vy
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
  shield_state.active = true;
  check_hms_vertical(hms, &plat_data);
  shield_state.active = false;
  EFM_ASSERT(hms[0].vy == 10 * ACTIVE_KINETIC_GAIN);
}

void test_hms_game_over(void) {
  //TODO
}

void test_platform_update(void) {
  struct PlatformData plat_data = {
      64, //x
      2, //vx
      1 //ax
  };
  update_platform(&plat_data);
  EFM_ASSERT(plat_data.x == 66);
  EFM_ASSERT(plat_data.vx == 3);
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
  EFM_ASSERT(hms[0].x == 84);
  EFM_ASSERT(hms[0].y == 69);
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

void test_integrated_physics(void) {
  //TODO
}


void physics_test_driver(void) {
  test_hms_check_return_to_harkonnen();
  test_hms_check_repelled_no_shield();
  test_hms_check_repelled_shield();
  test_hms_game_over();
  test_platform_update();
  test_hms_update();
  test_hms_update_wall();
  test_integrated_physics();
}


//none of the tests currently work, need to do stubbing for mutexes



