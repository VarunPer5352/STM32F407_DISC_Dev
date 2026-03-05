/*
 * main.h
 *
 *  Created on: Oct 12, 2025
 *      Author: varun.s.patil
 */

/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_H_
#define MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f407xx.h"
#include "core_cm4.h"
#include "cmsis_version.h"
#include "cmsis_gcc.h"
#include "cmsis_compiler.h"
#include "mpu_armv7.h"

/* User Driver Includes */


// Function Prototype's
void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif /* MAIN_H_ */
