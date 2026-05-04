/*
 * spi_driver.c
 *
 *  Created on: Mar 10, 2026
 *      Author: varun.s.patil
 */

#include "spi_driver.h"

/***************************************************************************
 * @brief       Controls the peripheral clock for the specified SPI
 *
 * @details     This function enables or disables the clock for a given
 *              SPI peripheral by configuring the corresponding RCC
 *              peripheral clock enable register.
 *
 *              In STM32F407, SPI peripherals are connected to two
 *              different APB buses:
 *
 *                  SPI1 → APB2 bus
 *                  SPI2 → APB1 bus
 *                  SPI3 → APB1 bus
 *                  SPI4 → APB2 bus
 *
 *              The clock must be enabled before accessing any SPI
 *              registers. If the clock is disabled, register writes
 *              will have no effect.
 *
 * @param[in]   pSPIx_addr
 *              Pointer to the SPI peripheral base address.
 *              Possible values:
 *                  SPI1
 *                  SPI2
 *                  SPI3
 *                  SPI4
 *
 * @param[in]   state
 *              Enable or disable control for the SPI clock.
 *
 *              Possible values:
 *                  ENABLE
 *                  DISABLE
 *
 * @return      None
 *
 * @note        Enabling the SPI clock is typically the first step
 *              before configuring the SPI peripheral registers.
 ***************************************************************************/
void spi_clk_ctrl(SPI_RegDef_t *pSPIx_addr, uint8_t state)
{
    if (state == ENABLE)
    {
        if(pSPIx_addr == SPI1)
        {
            SPI1_PCLK_EN();
        }
        else if (pSPIx_addr == SPI2)
        {
            SPI2_PCLK_EN();
        }
        else if (pSPIx_addr == SPI3)
        {
            SPI3_PCLK_EN();
        }
        else if (pSPIx_addr == SPI4)
        {
            SPI4_PCLK_EN();
        }
    }
    else if (state == DISABLE)
    {
        if(pSPIx_addr == SPI1)
        {
            SPI1_PCLK_DI();
        }
        else if (pSPIx_addr == SPI2)
        {
            SPI2_PCLK_DI();
        }
        else if (pSPIx_addr == SPI3)
        {
            SPI3_PCLK_DI();
        }
        else if (pSPIx_addr == SPI4)
        {
            SPI4_PCLK_DI();
        }
    }
}

/***************************************************************************
 * @brief       Initializes the SPI peripheral with the specified configuration
 *
 * @details     This function configures the SPI peripheral registers using
 *              the parameters stored in the SPI configuration structure
 *              within the SPI handle.
 *
 *              The initialization typically involves configuring the CR1
 *              and CR2 control registers of the SPI peripheral.
 *
 *              The configuration parameters include:
 *                  - Master / Slave mode
 *                  - Bus configuration (Full duplex / Half duplex / Simplex)
 *                  - Clock speed (baud rate prescaler)
 *                  - Data frame format (8-bit / 16-bit)
 *                  - Clock polarity (CPOL)
 *                  - Clock phase (CPHA)
 *                  - NSS management (hardware / software)
 *                  - Bit order (MSB first / LSB first)
 *                  - TI frame format mode
 *                  - CRC calculation settings
 *
 *              The SPI peripheral clock must be enabled before calling
 *              this function.
 *
 * @param[in]   pSPI_handle
 *              Pointer to the SPI handle structure containing the
 *              configuration information and peripheral base address.
 *
 * @return      None
 *
 * @note        This function only configures the SPI registers. The SPI
 *              peripheral may still need to be enabled by setting the
 *              SPE bit in CR1 after configuration is complete.
 ***************************************************************************/
void spi_init(SPI_Handle_t *pSPI_handle)
{
    /* Step 1: Alwasys enable peripheral clock */
    spi_clk_ctrl(pSPI_handle->Instance, ENABLE);

    /*
        NOTE:-
        ------
        Sometimes you must modify a register bit-by-bit, when:
            * you want to preserve existing bits
            * hardware requires sequential updates
        But for initialization like this case, the temporary register approach is almost always better.    
    */
    // Step 2: Init a temp reg for SPI_CR1 Reg
    uint32_t tmpSPI_CR1 = 0;
    
    // Step 3: Configure SPI Mode
    tmpSPI_CR1 |= pSPI_handle->Init.Mode << 2;
    
    // Step 4: Config the bus config or the Direction
    if(pSPI_handle->Init.Direction == SPI_COM_FD)
    {
        // BIDIMODE Bit in Reg should be cleared
        tmpSPI_CR1 &= ~(1U << 15); 
    }
    else if (pSPI_handle->Init.Direction == SPI_COM_HD)
    {
        // BIDIMODE Bit in Reg should be set
        tmpSPI_CR1 |= (1U << 15); 
    }
    else if (pSPI_handle->Init.Direction == SPI_COM_SIMPLEX_RX)
    {
        // BIDIMODE cleared
        tmpSPI_CR1 &= ~(1U << 15); 
        // RXONLY bit set
        tmpSPI_CR1 |= (1U << 10); 
    }

    // Step 5: Data size selection
    /*
        if (pSPI_handle->Init.DataSize == BYTE_FRAME_LEN)
        {
            tmpSPI_CR1 &= ~(1u << 11);
        }
        else if (pSPI_handle->Init.DataSize == DOUBLE_FRAME_LEN)
        {
            tmpSPI_CR1 |= (1u << 11);
        }
    */
    tmpSPI_CR1 |= pSPI_handle->Init.DataSize << 11;

    // Step 6: BaudRate Generator i.e. clock freq selection
    tmpSPI_CR1 |= pSPI_handle->Init.BaudRatePrescaler << 3;

    // Step 7: Configgure CPOL
    tmpSPI_CR1 |= pSPI_handle->Init.CLKPolarity << 1;

    // Step 8: Configgure CPHA
    tmpSPI_CR1 |= pSPI_handle->Init.CLKPhase << 0;

    // Finally plcae/assign content of this tmpSPI_CR1 reg into actual SPI_CR1 reg of a SPIx
    pSPI_handle->Instance->CR1 = tmpSPI_CR1;
}