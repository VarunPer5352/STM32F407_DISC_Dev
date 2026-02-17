/*
 * gpio.c
 *
 *  Created on: Feb 12, 2026
 *      Author: varun.s.patil
 */

#include "gpio.h"

void gpio_clk_ctrl(GPIO_TypeDef *pGPIOx, uint8_t state)
{
    if (state == ENABLE)
    {
        if(pGPIOx == GPIOA)
        {
            GPIOA_PCLK_EN();
        }
        else if(pGPIOx == GPIOB)
        {
            GPIOB_PCLK_EN();
        }
        else if(pGPIOx == GPIOC)
        {
            GPIOC_PCLK_EN();
        }
        else if(pGPIOx == GPIOD)
        {
            GPIOD_PCLK_EN();
        }
        else if(pGPIOx == GPIOE)
        {
            GPIOE_PCLK_EN();
        }
    }
    else
    {
        // Diabling clock to a gpio port:
        if(pGPIOx == GPIOA)
        {
            GPIOA_PCLK_DI();
        }
        else if(pGPIOx == GPIOB)
        {
            GPIOB_PCLK_DI();
        }
        else if(pGPIOx == GPIOC)
        {
            GPIOC_PCLK_DI();
        }
        else if(pGPIOx == GPIOD)
        {
            GPIOD_PCLK_DI();
        }
        else if(pGPIOx == GPIOE)
        {
            GPIOE_PCLK_DI();
        }        
    }
}

void gpio_init(GPIO_HandleTypeDef *pGPIO_Handle)
{
    // 1. Configure Mode of the pin, o/p or input & other stuff.

    uint8_t pin_number = pGPIO_Handle->Pin_Settings.Pin;
    pGPIO_Handle->pGPIOx->MODER = pGPIO_Handle->Pin_Settings.Mode << (pin_number)*2;
}