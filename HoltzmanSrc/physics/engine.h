/*
 * engine.h
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */

#ifndef PHYSICS_ENGINE_H_
#define PHYSICS_ENGINE_H_

#include "os.h"
#include "constants.h"
#include "holtzman_masses/hm.h"
#include "platform_control/platform.h"
#include "shield_system/shield.h"

#define GRAVITY GRAVITY_PIXELS * SCREEN_MM / SCREEN_PIXELS
#define GAME_OVER_SPEED 25
#define PHYSICS_PERIOD 1
#define PHYSICS_DELTA (PHYSICS_PERIOD / 10.0)

void update_hms(struct HoltzmanData hms[]);
void update_platform(struct PlatformData * plat_data);
void check_hms_vertical(struct HoltzmanData hms[], struct PlatformData * plat_data, struct ShieldState * shieldDat);

void physics_timer_cb(void);
void physics_task(void);
void physics_task_create(void);

#endif /* PHYSICS_ENGINE_H_ */

