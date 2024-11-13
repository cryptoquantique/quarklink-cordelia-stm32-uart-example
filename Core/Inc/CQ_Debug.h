/**
  ******************************************************************************
  * @file           : CQ_Debug.h
  * @brief          : CQ Debug redirect header file
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

#ifndef INC_CQ_DEBUG_H_
#define INC_CQ_DEBUG_H_

#define DEBUG_STARTUP_BANNER	// Print the Start up banner
#define DEBUG_PRINT_MAINx		// Debug printf macro for Main.c functions
#define INFO_PRINT_MAIN			// Information printf macro for Main.c functions
#define DEBUG_PRINT_ATx			// Debug printf macro for AT command handler
#define INFO_PRINT_AT			// Information printf macro for AT command handler

// Keep the prints at 12 characters wide
#ifdef DEBUG_STARTUP_BANNER
	#define DEBUG_STARTUP(fmt, ...) printf("\e[35mSTARTUP:    \e[0m" fmt, ##__VA_ARGS__)
#else
    #define DEBUG_STARTUP(fmt, ...) // Do nothing
#endif

#ifdef DEBUG_PRINT_MAIN
    #define DEBUG_MAIN(fmt, ...) printf("\e[31mDEBUG_MAIN: \e[0m" fmt, ##__VA_ARGS__)
#else
    #define DEBUG_MAIN(fmt, ...) // Do nothing
#endif

#ifdef INFO_PRINT_MAIN
    #define INFO_MAIN(fmt, ...) printf("\e[32mINFO_MAIN:  \e[0m" fmt, ##__VA_ARGS__)
#else
    #define INFO_MAIN(fmt, ...) // Do nothing
#endif


#ifdef DEBUG_PRINT_AT
	#define DEBUG_AT(fmt, ...) printf("\e[31mDEBUG_AT:   \e[0m" fmt, ##__VA_ARGS__)
#else
    #define DEBUG_AT(fmt, ...) // Do nothing
#endif

#ifdef INFO_PRINT_AT
	#define INFO_AT(fmt, ...) printf("\e[32mINFO_AT:    \e[0m" fmt, ##__VA_ARGS__)
#else
    #define INFO_AT(fmt, ...) // Do nothing
#endif
#endif /* INC_CQ_DEBUG_H_ */
