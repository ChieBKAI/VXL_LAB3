/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "button.h"
#include "software_timer.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void display7SEG(int num1, int num2) {
    // Define the 7-segment patterns for each digit
    const uint8_t segmentPatterns[] = {
        0b11000000, // 0
        0b11111001, // 1
        0b10100100, // 2
        0b10110000, // 3
        0b10011001, // 4
        0b10010010, // 5
        0b10000010, // 6
        0b11111000, // 7
        0b10000000, // 8
        0b10010000  // 9
    };

    // Ensure the input number is within the valid range (0-9)
    if (num1 < 0 || num1 > 9 || num2 < 0 || num2 > 9) {
        // Handle invalid input
        return;
    }

    // Set the GPIO pins according to the 7-segment pattern for the digit
    HAL_GPIO_WritePin(SEG_A_1_GPIO_Port, SEG_A_1_Pin, (segmentPatterns[num1] >> 0) & 1);
    HAL_GPIO_WritePin(SEG_B_1_GPIO_Port, SEG_B_1_Pin, (segmentPatterns[num1] >> 1) & 1);
    HAL_GPIO_WritePin(SEG_C_1_GPIO_Port, SEG_C_1_Pin, (segmentPatterns[num1] >> 2) & 1);
    HAL_GPIO_WritePin(SEG_D_1_GPIO_Port, SEG_D_1_Pin, (segmentPatterns[num1] >> 3) & 1);
    HAL_GPIO_WritePin(SEG_E_1_GPIO_Port, SEG_E_1_Pin, (segmentPatterns[num1] >> 4) & 1);
    HAL_GPIO_WritePin(SEG_F_1_GPIO_Port, SEG_F_1_Pin, (segmentPatterns[num1] >> 5) & 1);
    HAL_GPIO_WritePin(SEG_G_1_GPIO_Port, SEG_G_1_Pin, (segmentPatterns[num1] >> 6) & 1);

    HAL_GPIO_WritePin(SEG_A_2_GPIO_Port, SEG_A_2_Pin, (segmentPatterns[num2] >> 0) & 1);
    HAL_GPIO_WritePin(SEG_B_2_GPIO_Port, SEG_B_2_Pin, (segmentPatterns[num2] >> 1) & 1);
    HAL_GPIO_WritePin(SEG_C_2_GPIO_Port, SEG_C_2_Pin, (segmentPatterns[num2] >> 2) & 1);
    HAL_GPIO_WritePin(SEG_D_2_GPIO_Port, SEG_D_2_Pin, (segmentPatterns[num2] >> 3) & 1);
    HAL_GPIO_WritePin(SEG_E_2_GPIO_Port, SEG_E_2_Pin, (segmentPatterns[num2] >> 4) & 1);
    HAL_GPIO_WritePin(SEG_F_2_GPIO_Port, SEG_F_2_Pin, (segmentPatterns[num2] >> 5) & 1);
    HAL_GPIO_WritePin(SEG_G_2_GPIO_Port, SEG_G_2_Pin, (segmentPatterns[num2] >> 6) & 1);
}
void toggle_SEG(int turn) {
	if (turn == 1) {
		HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
		HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 0);
		return;
	}
	HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 0);
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
}
void turn_off_SEG() {
	HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 0);
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 0);
}
int seg1 = 12, seg2= 34;
int SEG_turn = 1;
void update_SEG(int turn) {
	if (turn == 1) {
		display7SEG(seg1 / 10, seg1 % 10);
		return;
	}
	display7SEG(seg2 / 10, seg2 % 10);
}
/////////////////////////////////////////////////////
int MODE = 0;
int new_mode = 1;
int mode0_stage = 4;
int count = -1;
int count_vertical = -1;
int green_time = 600;
int yellow_time = 300;
int red_time = 900;
int pre_mode_value = 500;

void reset_mode0() {
	mode0_stage = 4;
	count = -1;
	count_vertical = -1;
	new_mode = 0;
}

void mode0_timer() {
    count = count - 1;
    count_vertical = count_vertical - 1;
}

void mode_0() {
    if (mode0_stage == 1) {
        if (count <= 0) {
            count = yellow_time;
            mode0_stage = 2;
            HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, 0); // Turn on yellow 1
            HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, 1); // Turn off green 1
        }
    }
    if (mode0_stage == 2) {
        if (count <= 0) {
            count = red_time;
            count_vertical = green_time;
            mode0_stage = 3;
            HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, 0); // Turn on red 1
            HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, 0); // Turn on green 2
            HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, 1); // Turn off red 2
            HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, 1); // Turn off yellow 1
        }
    }
    if (mode0_stage == 3) {
  	  if (count_vertical <= 0) {
            count_vertical = yellow_time;
            mode0_stage = 4;
            HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, 1); // Turn off green 2
            HAL_GPIO_WritePin(LED_YELLOW2_GPIO_Port, LED_YELLOW2_Pin, 0); // Turn on yellow 2
        }
    }
    if (mode0_stage == 4) {
        if (count <= 0) {
            count = green_time;
            count_vertical = red_time;
            mode0_stage = 1;
            HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, 1);
            HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, 1);
            HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, 0);
            HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, 0);
            HAL_GPIO_WritePin(LED_YELLOW2_GPIO_Port, LED_YELLOW2_Pin, 1);
            HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, 1);
        }
    }
    seg1 = count/100 + 1;
    seg2 = count_vertical/100 + 1;
}
////////////////////////////////////////////////////////
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
void OFF_ALL_LED() {
    HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, 1);
    HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, 1);
    HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, 1);
    HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, 1);
    HAL_GPIO_WritePin(LED_YELLOW2_GPIO_Port, LED_YELLOW2_Pin, 1);
    HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, 1);
}
void LED_BLINKY() {
	if (new_mode == 1) {
		OFF_ALL_LED();
		new_mode = 0;
	}
	if (timer2_flag == 1) {
		if (MODE == 1) {
			HAL_GPIO_TogglePin(LED_RED1_GPIO_Port, LED_RED1_Pin);
			HAL_GPIO_TogglePin(LED_RED2_GPIO_Port, LED_RED2_Pin);
		}
		if (MODE == 2) {
			HAL_GPIO_TogglePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin);
			HAL_GPIO_TogglePin(LED_YELLOW2_GPIO_Port, LED_YELLOW2_Pin);
		}
		if (MODE == 3) {
			HAL_GPIO_TogglePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin);
			HAL_GPIO_TogglePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin);
		}
		setTimer2(25); //2Hz
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
			green_time = red_time - yellow_time;
		}
		if (MODE == 2) {
			yellow_time = pre_mode_value;
			red_time = green_time + yellow_time;
		}
		if (MODE == 3) {
			green_time = pre_mode_value;
			red_time = green_time + yellow_time;
		}
		MODE = 0;
		new_mode = 1;
	}
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  init_Button();
  while (1)
  {
	  MODE_Selector();
	  switch (MODE) {
		  case 0:
			  if (new_mode == 1) {
				  reset_mode0();
			  }
			  mode_0();
			  break;
		  default:
			  LED_BLINKY();
			  mode_123();
			  break;
	  }

	  /////////////////////////
	  if (timer0_flag == 1) {
		turn_off_SEG();
		toggle_SEG(SEG_turn);
		SEG_turn = 1 - SEG_turn;
		setTimer0(30);
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_RED1_Pin|LED_YELLOW1_Pin|LED_GREEN1_Pin|LED_RED2_Pin
                          |LED_YELLOW2_Pin|LED_GREEN2_Pin|SEG_A_1_Pin|SEG_B_1_Pin
                          |SEG_C_1_Pin|SEG_D_1_Pin|SEG_E_1_Pin|SEG_F_1_Pin
                          |SEG_G_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SEG_A_2_Pin|SEG_B_2_Pin|SEG_C_2_Pin|SEG_D_2_Pin
                          |SEG_E_2_Pin|SEG_F_2_Pin|SEG_G_2_Pin|EN1_Pin
                          |EN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_RED1_Pin LED_YELLOW1_Pin LED_GREEN1_Pin LED_RED2_Pin
                           LED_YELLOW2_Pin LED_GREEN2_Pin SEG_A_1_Pin SEG_B_1_Pin
                           SEG_C_1_Pin SEG_D_1_Pin SEG_E_1_Pin SEG_F_1_Pin
                           SEG_G_1_Pin */
  GPIO_InitStruct.Pin = LED_RED1_Pin|LED_YELLOW1_Pin|LED_GREEN1_Pin|LED_RED2_Pin
                          |LED_YELLOW2_Pin|LED_GREEN2_Pin|SEG_A_1_Pin|SEG_B_1_Pin
                          |SEG_C_1_Pin|SEG_D_1_Pin|SEG_E_1_Pin|SEG_F_1_Pin
                          |SEG_G_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SEG_A_2_Pin SEG_B_2_Pin SEG_C_2_Pin SEG_D_2_Pin
                           SEG_E_2_Pin SEG_F_2_Pin SEG_G_2_Pin EN1_Pin
                           EN2_Pin */
  GPIO_InitStruct.Pin = SEG_A_2_Pin|SEG_B_2_Pin|SEG_C_2_Pin|SEG_D_2_Pin
                          |SEG_E_2_Pin|SEG_F_2_Pin|SEG_G_2_Pin|EN1_Pin
                          |EN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : BUTTON2_Pin BUTTON3_Pin BUTTON1_Pin */
  GPIO_InitStruct.Pin = BUTTON2_Pin|BUTTON3_Pin|BUTTON1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	timerRun();
	getKeyInput();
	update_SEG(SEG_turn);
	if (MODE == 0){
		mode0_timer();
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
