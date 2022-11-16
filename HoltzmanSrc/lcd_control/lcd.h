/*
 * lcd.h
 *
 *  Created on: Nov 15, 2022
 *      Author: willsnider
 */

#ifndef LCD_CONTROL_LCD_H_
#define LCD_CONTROL_LCD_H_

#include "os.h"
#include "constants.h"

void LCD_init(void);
void lcd_task(void);
void lcd_timer_cb(void);

#endif /* LCD_CONTROL_LCD_H_ */

