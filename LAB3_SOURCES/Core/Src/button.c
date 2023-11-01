#include "button.h"

int KeyReg0[BUTTON_NUMBER];
int KeyReg1[BUTTON_NUMBER];
int KeyReg2[BUTTON_NUMBER];
int KeyReg3[BUTTON_NUMBER];

int TimeOutForKeyPress[BUTTON_NUMBER];
int button_flag[BUTTON_NUMBER];
int button_long_flag[BUTTON_NUMBER];

void init_Button() {
	for (int i = 0; i < BUTTON_NUMBER; i++) {
		KeyReg0[i] = NORMAL_STATE;
		KeyReg1[i] = NORMAL_STATE;
		KeyReg2[i] = NORMAL_STATE;
		KeyReg3[i] = NORMAL_STATE;
		TimeOutForKeyPress[i] = LONG_PRESSED_TIME;
		button_flag[i] = 0;
		button_long_flag[i] = 0;
	}
}

int isButtonPressed(int button_num){
	if(button_flag[button_num] == 1){
		button_flag[button_num] = 0;
		return 1;
	}
	return 0;
}
int isButtonLongPressed(int button_num){
	if(button_long_flag[button_num] == 1){
		button_long_flag[button_num] = 0;
		return 1;
	}
	return 0;
}


void getKeyInput(){
	for (int i = 0; i < BUTTON_NUMBER; i++) {
		KeyReg2[i] = KeyReg1[i];
		KeyReg1[i] = KeyReg0[i];
	}
  //Add your button here
  KeyReg0[0] = HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin);
  KeyReg0[1] = HAL_GPIO_ReadPin(BUTTON2_GPIO_Port, BUTTON2_Pin);
  KeyReg0[2] = HAL_GPIO_ReadPin(BUTTON3_GPIO_Port, BUTTON3_Pin);

  for (int i = 0; i < BUTTON_NUMBER; i++) {
	  if ((KeyReg1[i] == KeyReg0[i]) && (KeyReg1[i] == KeyReg2[i])) {
	    if (KeyReg2[i] != KeyReg3[i]) {
	      KeyReg3[i] = KeyReg2[i];

	      if (KeyReg3[i] == PRESSED_STATE){
	    	  button_flag[i] = 1;
	    	  TimeOutForKeyPress[i] = LONG_PRESSED_TIME;
	      }

	    }else {
	    	TimeOutForKeyPress[i] --;
	        if (TimeOutForKeyPress[i] == 0){
	        	TimeOutForKeyPress[i] = LONG_PRESSED_TIME;
	        	if (KeyReg3[i] == PRESSED_STATE) {
	    			button_long_flag[i] = 1;
	        	}
	        }
	    }
	  }
  }
}

