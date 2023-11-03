/*
 * input_reader.c
 *
 *  Created on: Nov 2, 2023
 *      Author: K21
 */
#include "input_reader.h"
#include "button.h"

int seg1 = 12, seg2 = 34;
int SEG_turn = 1;
int MODE = 0;
int new_mode = 1;
int mode0_stage = 4;
int pre_mode_value = 0;
int count = -1;
int count_vertical = -1;
int green_time = 600;
int yellow_time = 300;
int red_time = 900;

void MODE_Selector() {
	if (isButtonPressed(0) == 1) {
		MODE++;
		new_mode = 1;
		if (MODE > 3) { //only 4 mode;
			MODE = 0;
		}
		if (MODE == 1) {
			pre_mode_value = red_time;
		}
		if (MODE == 2) {
			pre_mode_value = yellow_time;
		}
		if (MODE == 3) {
			pre_mode_value = green_time;
		}
	}
}

void mode_123() {
	seg1 = pre_mode_value / 100;
	seg2 = MODE + 1;
	if (isButtonPressed(1) == 1) {
		pre_mode_value += 100;
		if (pre_mode_value > 9900) {
			pre_mode_value = 100;
		}
	}
	if (isButtonPressed(2) == 1) {
		if (MODE == 1) {
			red_time = pre_mode_value;
			green_time = red_time - yellow_time; //increase green time whenever red time was increased
		}
		if (MODE == 2) {
			yellow_time = pre_mode_value;
			red_time = green_time + yellow_time; //increase red time whenever yellow time was increased
		}
		if (MODE == 3) {
			green_time = pre_mode_value;
			red_time = green_time + yellow_time; //increase red time whenever green time was increased
		}
		MODE = 0; //back to auto mode when time was set
		new_mode = 1;
	}
}
