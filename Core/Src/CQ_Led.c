/**
  ******************************************************************************
  * @file           : CQ_Led.c
  * @brief          : LED functions used for status indications
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

#include "CQ_Led.h"
#include "CQ_Debug.h"

// *********************************************************************
// * LED Functions
// *********************************************************************
LEDControl ledPubControl;
void Start_PUBLED_Flashing(void) {
	ledPubControl.state = FLASHLED;
	ledPubControl.counter = 0;
}
LEDControl ledSubControl;
void Start_SUBLED_Flashing(void) {
	ledSubControl.state = FLASHLED;
	ledSubControl.counter = 0;
}
