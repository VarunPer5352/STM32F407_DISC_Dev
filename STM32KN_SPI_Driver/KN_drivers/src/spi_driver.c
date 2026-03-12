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