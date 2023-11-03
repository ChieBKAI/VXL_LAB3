/*
 * input_reader.h
 *
 *  Created on: Nov 2, 2023
 *      Author: K21
 */

#ifndef INC_INPUT_READER_H_
#define INC_INPUT_READER_H_

#include "main.h"

extern int MODE;
extern int new_mode;
extern int mode0_stage;
extern int pre_mode_value;
extern int count;
extern int count_vertical;
extern int green_time;
extern int yellow_time;
extern int red_time;
extern int seg1;
extern int seg2;
extern int SEG_turn;

void MODE_Selector();
void mode_123();


#endif /* INC_INPUT_READER_H_ */
