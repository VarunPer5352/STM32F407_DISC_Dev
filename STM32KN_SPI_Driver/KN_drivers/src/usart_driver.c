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

/***************************************************************************
 * @brief       Initializes the selected USART/UART peripheral.
 *
 * @details     Configures the peripheral according to the values supplied
 *              in the USART handle. Baud rate, word length, parity, stop
 *              bits, operating mode, and hardware flow control are applied
 *              during initialization.
 *
 * @param[in]   pUSARTx_handle
 *              Pointer to the USART handle and its configuration structure.
 *
 * @return      None
 *
 * @note        This function is currently reserved for the USART
 *              initialization implementation.
 ***************************************************************************/
void usart_init(USART_Handle_t *pUSARTx_handle)
{
    // Enable clock to the desired peripheral
    usart_clk_ctrl(pUSARTx_handle->pUSARTx, ENABLE);

    /**** Configuration of CR1 Register ****/
    // Temp variable
    uint32_t tmpUSARTx_CR1 = 0;

    if (pUSARTx_handle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
    {
        // Enable the transmitter
        tmpUSARTx_CR1 |= (1U << USART_CR1_TE);
    }
    else if (pUSARTx_handle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
    {
        // Enable the receiver
        tmpUSARTx_CR1 |= (1U << USART_CR1_RE);
    }
    else if (pUSARTx_handle->USART_Config.USART_Mode == USART_MODE_TXRX)
    {
        // Enable the Receiver & Transmitter bit field
        tmpUSARTx_CR1 |= (1U << USART_CR1_RE) | (1U << USART_CR1_TE);
    }
    
    // Configure the Word length configuration item 
    tmpUSARTx_CR1 |= (pUSARTx_handle->USART_Config.USART_WordLength << USART_CR1_M);

    // Parity settings
    if (pUSARTx_handle->USART_Config.USART_ParityControl != USART_PARITY_DISABLE)
    {
        tmpUSARTx_CR1 |= (1U << USART_CR1_PCE);
        if (pUSARTx_handle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD)
        {
            // Odd Parity
            tmpUSARTx_CR1 |= (1U << USART_CR1_PS);
        }
        else if (pUSARTx_handle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
        {
            // Even Parity
            tmpUSARTx_CR1 &= ~(1U << USART_CR1_PS);
        }
    }

    //Program the CR1 register
    pUSARTx_handle->pUSARTx->USART_CR1 = tmpUSARTx_CR1;
    
    /**** Configuration of CR2 Register ****/
    uint32_t tmp_USARTx_CR2 = 0; // tmp variable

    tmp_USARTx_CR2 |= (pUSARTx_handle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP);
    
    //Program the CR2 register
    pUSARTx_handle->pUSARTx->USART_CR2 = tmp_USARTx_CR2;

    /**** Configuration of CR3 Register ****/
    uint32_t tmp_USARTx_CR3 = 0; // tmp variable

    if (pUSARTx_handle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
    {
        // Enable CTS flow control 
        tmp_USARTx_CR3 |= ( 1U << USART_CR3_CTSE);
    }
    else if (pUSARTx_handle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		// Enable RTS flow control 
		tmp_USARTx_CR3 |= ( 1U << USART_CR3_RTSE);

	}else if (pUSARTx_handle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		// Enable both CTS and RTS Flow control 
		tmp_USARTx_CR3 |= ( 1U << USART_CR3_CTSE) | ( 1U << USART_CR3_RTSE);
	}
    
    pUSARTx_handle->pUSARTx->USART_CR3 = tmp_USARTx_CR3;

    // Configuration of BRR(Baudrate register) {Later}
}
