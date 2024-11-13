/** 
  ******************************************************************************
  * @file           : CQ_Led.h
  * @brief          : CQ Led  redirect header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Crypto Quantique.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
**/

#ifndef INC_CQ_LED_H_
#define INC_CQ_LED_H_

#define IoTHub_Connected_Pin GPIO_PIN_6
#define IoTHub_Connected_GPIO_Port GPIOA
#define Published_Pin GPIO_PIN_7
#define Published_GPIO_Port GPIOA
#define Subscribe_Pin GPIO_PIN_4
#define Subscribe_GPIO_Port GPIOC

#define WiFi_Connected_Pin GPIO_PIN_4
#define WiFi_Connected_GPIO_Port GPIOA

void Start_PUBLED_Flashing(void);
void Start_SUBLED_Flashing(void);

/**
  ******************************************************************************
  * @file           : CQ_Led.h
  * @brief          : LED functions used for status indications header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 Crypto Quantique.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
**/

// Define states and control structure for LEDs
#define MAX_CYCLES 6 // number of flashes x 2.

typedef enum {
    IDLELED,
    FLASHLED
} LEDState;

typedef struct {
    LEDState state;
    int counter;
} LEDControl;

extern LEDControl ledPubControl;
extern LEDControl ledSubControl;



























#endif /* INC_CQ_LED_H_ */
