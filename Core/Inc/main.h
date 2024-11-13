/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

extern TIM_HandleTypeDef htim4;

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;

#define BUFFERSIZE 100 // 80
extern  char tx_buffer[BUFFERSIZE]; // lets put the char's in one at a time in the main function.

// Wuerth UART traffic is terminated via '\n' so lets look for an '\n', 0xA, 10d
#define END_BYTE 0xA

extern char RSP[BUFFERSIZE]; // Rx data ReSPonse buffer
extern int rx_indx; // = 0;
extern uint8_t rx_data; // RAW UART data is put here
extern char rx_buffer[BUFFERSIZE]; // UART data is copied to this buffer if it's got the correct 'END_SBYTE' and it's for us

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define T4_PRE 7199
#define T4_CNT 4999
#define WiFi_Connected_Pin GPIO_PIN_4
#define WiFi_Connected_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define IoTHub_Connected_Pin GPIO_PIN_6
#define IoTHub_Connected_GPIO_Port GPIOA
#define Published_Pin GPIO_PIN_7
#define Published_GPIO_Port GPIOA
#define Subscribe_Pin GPIO_PIN_4
#define Subscribe_GPIO_Port GPIOC
#define CordeliaReset_Pin GPIO_PIN_5
#define CordeliaReset_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
