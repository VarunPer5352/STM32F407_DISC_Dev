/*
 * gpio.h
 *
 *  Created on: Feb 12, 2026
 *      Author: varun.s.patil
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "main.h"

/*
 *  CLock Enable & Disable Macros for various GPIO Ports 
 */

/* Enable */ 
#define GPIOA_PCLK_EN()				RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN
#define GPIOB_PCLK_EN()             RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN
#define GPIOC_PCLK_EN()				RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN
#define GPIOD_PCLK_EN()             RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN
#define GPIOE_PCLK_EN()				RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN

/* Disable */ 
#define GPIOA_PCLK_DI()				RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOAEN)
#define GPIOB_PCLK_DI()             RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOBEN)
#define GPIOC_PCLK_DI()				RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOCEN)
#define GPIOD_PCLK_DI()             RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIODEN)
#define GPIOE_PCLK_DI()				RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOEEN)

#endif /* INC_GPIO_H_ */
