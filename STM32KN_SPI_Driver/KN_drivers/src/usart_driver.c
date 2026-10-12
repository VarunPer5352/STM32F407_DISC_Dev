#include "usart_driver.h"

/***************************************************************************
 * @brief       Controls the peripheral clock for the specified USART/UART
 *
 * @details     Enables or disables the clock through the RCC APB1/APB2
 *              peripheral clock-enable registers. The clock must be enabled
 *              before accessing the peripheral registers.
 *
 *              STM32F407 bus mapping:
 *                  USART1 -> APB2     USART6 -> APB2
 *                  USART2 -> APB1     USART3 -> APB1
 *                  UART4  -> APB1     UART5  -> APB1
 *
 * @param[in]   pUSARTx_addr
 *              Pointer to USART/UART peripheral (USART1, USART2, USART3,
 *              UART4, UART5, or USART6).
 *
 * @param[in]   state
 *              ENABLE or DISABLE.
 *
 * @return      None
 ***************************************************************************/
void usart_clk_ctrl(USART_RegDef_t *pUSARTx_addr, uint8_t state)
{
    if (state == ENABLE)
    {
        if (pUSARTx_addr == USART1)
        {
            USART1_PCLK_EN();
        }
        else if (pUSARTx_addr == USART2)
        {
            USART2_PCLK_EN();
        }
        else if (pUSARTx_addr == USART3)
        {
            USART3_PCLK_EN();
        }
        else if (pUSARTx_addr == UART4)
        {
            UART4_PCLK_EN();
        }
        else if (pUSARTx_addr == UART5)
        {
            UART5_PCLK_EN();
        }
        else if (pUSARTx_addr == USART6)
        {
            USART6_PCLK_EN();
        }
    }
    else if (state == DISABLE)
    {
        if (pUSARTx_addr == USART1)
        {
            USART1_PCLK_DI();
        }
        else if (pUSARTx_addr == USART2)
        {
            USART2_PCLK_DI();
        }
        else if (pUSARTx_addr == USART3)
        {
            USART3_PCLK_DI();
        }
        else if (pUSARTx_addr == UART4)
        {
            UART4_PCLK_DI();
        }
        else if (pUSARTx_addr == UART5)
        {
            UART5_PCLK_DI();
        }
        else if (pUSARTx_addr == USART6)
        {
            USART6_PCLK_DI();
        }
    }
}
