/*
 * shield.h
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */

#ifndef SHIELD_SYSTEM_SHIELD_H_
#define SHIELD_SYSTEM_SHIELD_H_
#include "stdbool.h"
#include "os.h"
#include "constants.h"

#define SHIELD_ACTIVE_100MS 5
#define SHIELD_RECHARGE_100MS 15

#define PASSIVE_KINETIC_REDUCTION -0.75
#define ACTIVE_KINETIC_GAIN -1.25

struct ShieldState {
  bool active;
  bool recharging;
};


void shield_task(void);
void shield_task_create(void);

#endif /* SHIELD_SYSTEM_SHIELD_H_ */
