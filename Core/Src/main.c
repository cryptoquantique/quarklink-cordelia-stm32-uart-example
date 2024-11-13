/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
#include "CQ_AT_Cmds.h"
#include "CQ_Led.h"
#include "CQ_Debug.h"

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
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch) // redirect Printf to USB port
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

uint8_t rx_data;
char tx_buffer[BUFFERSIZE];
TIM_HandleTypeDef htim4;
uint8_t rx_data; // RAW UART data is put here
char rx_buffer[BUFFERSIZE];
int rx_indx;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Override the weak call back function
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
      if (htim->Instance == TIM4) {
    	  // Handle PUBLED
    	          if (ledPubControl.state == FLASHLED) {
    	              HAL_GPIO_TogglePin(Published_GPIO_Port, Published_Pin);
    	              ledPubControl.counter++;
    	              if (ledPubControl.counter >= MAX_CYCLES) {
    	            	  ledPubControl.state = IDLELED;
    	            	  HAL_GPIO_WritePin(Published_GPIO_Port, Published_Pin, GPIO_PIN_RESET);  // Turn off PUB LED
    	              }
    	          }

    	          // Handle SUBLED
    	          if (ledSubControl.state == FLASHLED) {
    	              HAL_GPIO_TogglePin(Subscribe_GPIO_Port, Subscribe_Pin);
    	              ledSubControl.counter++;
    	              if (ledSubControl.counter >= MAX_CYCLES) {
    	            	  ledSubControl.state = IDLELED;
    	            	  HAL_GPIO_WritePin(Subscribe_GPIO_Port, Subscribe_Pin, GPIO_PIN_RESET);  // Turn off SUB LED
    	              }
    	          }
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
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart6, &rx_data, 1);

  // Fire up the timer used to toggle the LEDs
  HAL_TIM_Base_Start_IT(&htim4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  // issue a HW reset of Cordelia at STM boot to ensure a known start up place.
  	HAL_GPIO_WritePin(CordeliaReset_GPIO_Port, CordeliaReset_Pin, GPIO_PIN_RESET);
  	HAL_Delay(200); //200ms should be enough
  	HAL_GPIO_WritePin(CordeliaReset_GPIO_Port, CordeliaReset_Pin, GPIO_PIN_SET);

  	HAL_Delay(200); // short start up delay to allow Cordelia to boot

  	DEBUG_STARTUP("**************************************************\r\n");
  	DEBUG_STARTUP("*                                                *\r\n");
  	DEBUG_STARTUP("* Welcome to Crypto Quantique's Secure           *\r\n");
  	DEBUG_STARTUP("* Wurth Cordelia-I & STM32 AT Host example       *\r\n");
  	DEBUG_STARTUP("*                                                *\r\n");
  	DEBUG_STARTUP("* Version 1.0.0, (c) Crypto Quantique Aug 2024   *\r\n");
  	DEBUG_STARTUP("*                                                *\r\n");
  	DEBUG_STARTUP("**************************************************\r\n");

	while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = T4_PRE;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = T4_CNT;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, WiFi_Connected_Pin|LD2_Pin|IoTHub_Connected_Pin|Published_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Subscribe_Pin|CordeliaReset_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : WiFi_Connected_Pin LD2_Pin IoTHub_Connected_Pin Published_Pin */
  GPIO_InitStruct.Pin = WiFi_Connected_Pin|LD2_Pin|IoTHub_Connected_Pin|Published_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Subscribe_Pin CordeliaReset_Pin */
  GPIO_InitStruct.Pin = Subscribe_Pin|CordeliaReset_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// Function to parse and execute AT commands
void parseATCommand(const char *command) {
    char cmd[60];		// Assuming command string won't exceed 60 characters
    char params[60];	// Assuming parameters won't exceed 50 characters

    // Separate command and parameters
    sscanf(rx_buffer, "%s %s", cmd, params);

    // Find the command in the array and call its handler
    for (int i = 0; i < AT_RESPONSES_SIZE; i++) {
   	if (strstr(rx_buffer, commands[i].command)) {
            commands[i].handler(params);
            // clear the rx_buffer so we don't accidently trigger on old trash data once we're done with it.
            uint8_t j;
        	for (j=0; j<BUFFERSIZE; j++) {
        		rx_buffer[j] = 0;
        	}
        return;
        }
    }
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (rx_indx < BUFFERSIZE) {
		// HAL_UART_Transmit(&huart2, (uint8_t*)&rx_data, 1, 10); // echo the byte back to the STM32 debug UART
		rx_buffer[rx_indx] = rx_data;	//Put the received byte in the buffer and then increment rx_indx
				if (rx_buffer[rx_indx] == END_BYTE) {
			rx_indx = 0; 				//Reset the buffer pointer
			parseATCommand(rx_buffer);
		}
		else {
			rx_indx++;
 		}
	}
	HAL_UART_Receive_IT(&huart6, &rx_data,1);
}
/*
* @brief  EXTI line detection callbacks.
* @param  GPIO_Pin Specifies the pins connected EXTI line
* This is our INT button call back
* @retval None
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_13) { // if BN1 (blue user button)
	// check we are connected
    if (status.connected) {
    	char string3[] = "AT+iotpublish=0,\"Hello from button press\"\r\n";
     	HAL_UART_Transmit(&huart6, (uint8_t*)&string3, (strlen(string3)), 100);
     	// HAL_GPIO_TogglePin(Published_GPIO_Port, Published_Pin);
     	DEBUG_MAIN("%s",string3);
     	INFO_MAIN("MQTT Publish message sent\r\n");
    	// Fire up the timer used to toggle the LEDs
     	// HAL_TIM_Base_Start_IT(&htim4);
     	// currentState = TOGGLE_PUB;  // Set the state to toggle LED1 next
     	Start_PUBLED_Flashing();
    }
    // Device Revoked?
	if (!status.enroled) {
		INFO_MAIN("Device not enroled, attempting to re-enrol\r\n");
		// Lets try to re-enrol
		char string1[] = "AT+iotenrol\r\n";
		HAL_UART_Transmit(&huart6, (uint8_t*) &string1, (strlen(string1)), 100);
		DEBUG_MAIN("%s",string1);
	}
	// Not connected or revoked
    } else {
   	 DEBUG_MAIN("MQTT Publish failed - not connected\r\n");
    }

}

/**
  * @brief  Retargets the C library printf function to the USART.
  *   None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
