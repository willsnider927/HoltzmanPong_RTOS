/*
 * laser.h
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */

#ifndef LASER_SYSTEM_LASER_H_
#define LASER_SYSTEM_LASER_H_
#include "os.h"
#include "constants.h"

void laser_task_create(void);
void laser_task(void);

#endif /* LASER_SYSTEM_LASER_H_ */
