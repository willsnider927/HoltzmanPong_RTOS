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

#define SHIELD_ACTIVE_MS 100
#define SHIELD_RECHARGE_MS 2000

#define PASSIVE_KINETIC_REDUCTION -0.75
#define ACTIVE_KINETIC_GAIN -1.25

static struct ShieldState {
  bool active;
  bool recharging;
} shield_state;

static OS_MUTEX shield_mutex;
static OS_Q shield_msg;

void shield_task(void);
void shield_task_create(void);

#endif /* SHIELD_SYSTEM_SHIELD_H_ */
