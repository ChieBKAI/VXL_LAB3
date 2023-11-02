/*
 * button.h
 *
 *  Created on: Oct 5, 2023
 *      Author: KAI
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#define NORMAL_STATE SET
#define PRESSED_STATE RESET
#define BUTTON_NUMBER 3
#define LONG_PRESSED_TIME 200


extern int button_flag[BUTTON_NUMBER];
extern int button_long_flag[BUTTON_NUMBER];

void init_Button();
void getKeyInput();
int isButtonPressed(int button_num);
int isButtonLongPressed(int button_num);

#endif /* INC_BUTTON_H_ */

