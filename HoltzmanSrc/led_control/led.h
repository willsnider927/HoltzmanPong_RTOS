/*
 * led.h
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */

#ifndef LED_CONTROL_LED_H_
#define LED_CONTROL_LED_H_
#include "os.h"
#include "constants.h"

static OS_SEM led_semaphore;

void led_task_create();
void led_task();


#endif /* LED_CONTROL_LED_H_ */
