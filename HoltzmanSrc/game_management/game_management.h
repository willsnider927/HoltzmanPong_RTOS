/*
 * game_management.h
 *
 *  Created on: Dec 2, 2022
 *      Author: willsnider
 */

#ifndef GAME_MANAGEMENT_GAME_MANAGEMENT_H_
#define GAME_MANAGEMENT_GAME_MANAGEMENT_H_

enum game_state_e {PREGAME = 0x1, IN_PROGRESS = 0x2, GAME_OVER = 0x4};
enum difficulty_e {EASY, MEDIUM, HARD, INSANE};



void game_over(char cause[]);
void decrement_life(void);
void start_game(void);
void game_stopped_task(void);
void game_stopped_task_create(void);

#endif /* GAME_MANAGEMENT_GAME_MANAGEMENT_H_ */
