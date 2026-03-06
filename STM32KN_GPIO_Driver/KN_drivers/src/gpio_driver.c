/*
 * gpio_driver.c
 *
 *  Created on: Mar 6, 2026
 *      Author: varun.s.patil
 */

#include "gpio_driver.h"

/****************************************************
 * @intro:
            This func enables or diables the clock to a gpio peripheral
            from A-I.

 * @param1[in]:
                GPIO_RegDef_t *pGPIOx_addr -> this is the pointer to the gpio peripheral.

 * @param1[in]:
                uint8_t state -> this is the state of the clock ctrl i.e. 0 means disable & 1 means enable.

 * @return: 
            Nothing.
 * @Note:
        Uses the RCC clock control MARCOs for GPIO clock control.
 */
void gpio_clk_ctrl(GPIO_RegDef_t *pGPIOx_addr, uint8_t state)
{
    if (state == ENABLE)
    {
        if (pGPIOx_addr == GPIOA)
        {
            GPIOA_PCLK_EN();
        }
        else if (pGPIOx_addr == GPIOB)
        {
            GPIOB_PCLK_EN();
        }
        else if (pGPIOx_addr == GPIOC)
        {
            GPIOC_PCLK_EN();
        }
        else if (pGPIOx_addr == GPIOD)
        {
            GPIOD_PCLK_EN();
        }
        else if (pGPIOx_addr == GPIOE)
        {
            GPIOE_PCLK_EN();
        }
        else if (pGPIOx_addr == GPIOF)
        {
            GPIOF_PCLK_EN();
        }
        else if (pGPIOx_addr == GPIOG)
        {
            GPIOG_PCLK_EN();
        }
        else if (pGPIOx_addr == GPIOH)
        {
            GPIOH_PCLK_EN();
        }
        else if (pGPIOx_addr == GPIOI)
        {
            GPIOI_PCLK_EN();
        }        
    }
    else
    {
        if (pGPIOx_addr == GPIOA)
        {
            GPIOA_PCLK_DI();
        }
        else if (pGPIOx_addr == GPIOB)
        {
            GPIOB_PCLK_DI();
        }
        else if (pGPIOx_addr == GPIOC)
        {
            GPIOC_PCLK_DI();
        }
        else if (pGPIOx_addr == GPIOD)
        {
            GPIOD_PCLK_DI();
        }
        else if (pGPIOx_addr == GPIOE)
        {
            GPIOE_PCLK_DI();
        }
        else if (pGPIOx_addr == GPIOF)
        {
            GPIOF_PCLK_DI();
        }
        else if (pGPIOx_addr == GPIOG)
        {
            GPIOG_PCLK_DI();
        }
        else if (pGPIOx_addr == GPIOH)
        {
            GPIOH_PCLK_DI();
        }
        else if (pGPIOx_addr == GPIOI)
        {
            GPIOI_PCLK_DI();
        }        
    }
}