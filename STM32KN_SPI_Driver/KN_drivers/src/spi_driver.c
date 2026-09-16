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
    
    // Step 3: Configure SPI Mode, or device mode
    tmpSPI_CR1 |= pSPI_handle->Init.Mode << SPI_CR1_MSTR;
    
    // Step 4: Config the bus config or the Direction
    if(pSPI_handle->Init.Direction == SPI_COM_FD)
    {
        // BIDIMODE Bit in Reg should be cleared
        tmpSPI_CR1 &= ~(1U << SPI_CR1_BIDIMODE); 
    }
    else if (pSPI_handle->Init.Direction == SPI_COM_HD)
    {
        // BIDIMODE Bit in Reg should be set
        tmpSPI_CR1 |= (1U << SPI_CR1_BIDIMODE); 
    }
    else if (pSPI_handle->Init.Direction == SPI_COM_SIMPLEX_RX)
    {
        // BIDIMODE cleared
        tmpSPI_CR1 &= ~(1U << SPI_CR1_BIDIMODE); 
        // RXONLY bit set
        tmpSPI_CR1 |= (1U << SPI_CR1_RXONLY); 
    }

    // Step 5: Data size selection
    tmpSPI_CR1 |= pSPI_handle->Init.DataSize << SPI_CR1_DFF;

    // Step 6: BaudRate Generator i.e. clock freq selection
    tmpSPI_CR1 |= pSPI_handle->Init.BaudRatePrescaler << SPI_CR1_BR;

    // Step 7: Configgure CPOL
    tmpSPI_CR1 |= pSPI_handle->Init.CLKPolarity << SPI_CR1_CPOL;

    // Step 8: Configgure CPHA
    tmpSPI_CR1 |= pSPI_handle->Init.CLKPhase << SPI_CR1_CPHA;


    // Step 9: Configure SSM settings
    tmpSPI_CR1 |= pSPI_handle->Init.NSS << SPI_CR1_SSM;

    // Finally place/assign content of this tmpSPI_CR1 reg into actual SPI_CR1 reg of a SPIx NOTE: This is only good during initilisation
    pSPI_handle->Instance->CR1 = tmpSPI_CR1;
}

/***************************************************************************
 * @brief       De-initializes the specified SPI peripheral
 *
 * @details     This function resets the selected SPI peripheral to its
 *              default reset state by asserting and releasing the
 *              corresponding peripheral reset signal through the RCC.
 *
 *              All SPI control, status, and configuration registers are
 *              restored to their hardware reset values.
 *
 *              This function does not disable the peripheral clock.
 *
 * @param[in]   pSPIx_addr
 *              Pointer to the base address of the SPI peripheral to
 *              be de-initialized.
 *
 * @return      None
 *
 * @note        Any ongoing SPI communication should be completed or
 *              terminated before calling this function, as all peripheral
 *              configuration will be lost after reset.
 ***************************************************************************/
void spi_deinit(SPI_RegDef_t *pSPIx_addr)
{
    if (pSPIx_addr == SPI1)
    {
        SPI1_REG_RESET();
    }
    else if (pSPIx_addr == SPI2)
    {
        SPI2_REG_RESET();
    }
    else if (pSPIx_addr == SPI3)
    {
        SPI3_REG_RESET();
    }
    else if (pSPIx_addr == SPI4)
    {
        SPI4_REG_RESET();
    }
}

/***************************************************************************
 * @brief       Transmits data over the SPI peripheral (Blocking API)
 *
 * @details     This function transmits the specified number of data bytes
 *              from the transmit buffer over the SPI peripheral using
 *              polling. The function waits for the TXE flag before writing
 *              each data frame to the SPI data register and does not return
 *              until all data has been loaded into the transmit FIFO/data
 *              register.
 *
 *              This is a blocking API, meaning the CPU remains occupied
 *              until the entire transmission is complete.
 *
 *              The SPI peripheral must be properly initialized and enabled
 *              before calling this function.
 *
 * @param[in]   pSPIx_addr
 *              Pointer to the base address of the SPI peripheral.
 *
 * @param[in]   pTX_buffer
 *              Pointer to the transmit data buffer.
 *
 * @param[in]   data_len
 *              Number of bytes to transmit.
 *
 * @return      None
 *
 * @note        This function supports both 8-bit and 16-bit data frame
 *              formats. The data length should be consistent with the
 *              configured frame size.
 *              When the SPI peripheral is configured for 16-bit data frame format (DFF = 1), data_len must be an even number. Supplying an odd value results in undefined behavior.
 ***************************************************************************/
void spi_send_data(SPI_RegDef_t *pSPIx_addr, uint8_t *pTX_buffer, uint32_t data_len)
{
    while (data_len > 0)
    {
        /*
         * Initial logic replaced by newer better logic!
        if (!(pSPIx_addr->SR & (1 << SPI_SR_TXE))) // TXE: Transmit buffer empty & 0: Tx buffer not empty vice-versa
        {
            continue; // Never transmit while Transmit buffer is full/has some data that is being sent as it causes corruption of that data as well as current data!
        }
        */
        while (!(pSPIx_addr->SR & (1U << SPI_SR_TXE))); // Wait till TXE becomes '1'

        if (!(pSPIx_addr->CR1 & (1U << SPI_CR1_DFF))) // 8-bit mode
        {
        	*((volatile uint8_t *)&pSPIx_addr->DR) = *(pTX_buffer);
            pTX_buffer++;
            data_len--;
        }
        else // 16 bit mode
        {
            uint16_t data = ((uint16_t)pTX_buffer[0] << 8) | (uint16_t)pTX_buffer[1];
            *((volatile uint16_t *)&pSPIx_addr->DR) = data; // pSPIx_addr->DR = data;
            pTX_buffer += 2;
            data_len   -= 2;
        }  
    }

    while (pSPIx_addr->SR & (1U << SPI_SR_BSY)); // Because TXE = 1 only means the data register is empty—it does not mean the last bit has finished transmitting on the wire.
}

/***************************************************************************
 * @brief       Enables or disables the SPI peripheral
 *
 * @details     This function controls the SPI peripheral enable (SPE) bit
 *              in the SPI control register (CR1).
 *
 *              When enabled:
 *                  - The SPI peripheral starts operation
 *                  - Clock generation begins in master mode
 *                  - Data transmission/reception becomes active
 *
 *              When disabled:
 *                  - SPI communication is halted
 *                  - The peripheral stops driving the bus
 *
 *              NOTE:
 *              -----
 *              Disabling SPI while a transfer is ongoing may corrupt the
 *              transmission. Always ensure that the BSY flag is cleared
 *              before disabling the peripheral.
 *
 * @param[in]   Instance
 *              Pointer to the SPI peripheral base address.
 *
 * @param[in]   state
 *              ENABLE  -> Set SPE bit (enable SPI)
 *              DISABLE -> Clear SPE bit (disable SPI)
 *
 * @return      None
 ***************************************************************************/
void spi_set_state(SPI_RegDef_t *Instance, uint8_t state)
{
	if (state == ENABLE)
	{
		Instance->CR1 |= (1U << SPI_CR1_SPE);
	}
	else
	{
		Instance->CR1 &= ~(1U << SPI_CR1_SPE);
	}
}

/***************************************************************************
 * @brief       Enables or disables software slave management (SSM)
 *
 * @details     This function controls the SSM (Software Slave Management)
 *              bit in the SPI control register (CR1).
 *
 *              When enabled:
 *                  - NSS is controlled internally via SSI bit
 *                  - External NSS pin is ignored
 *
 *              When disabled:
 *                  - NSS is controlled by hardware (external pin)
 *
 *              Typical usage:
 *              --------------
 *              - Enable SSM when using manual GPIO-based chip select
 *              - Disable SSM when using hardware NSS pin
 *
 * @param[in]   Instance
 *              Pointer to the SPI peripheral base address.
 *
 * @param[in]   state
 *              ENABLE  -> Enable software NSS management
 *              DISABLE -> Use hardware NSS pin
 *
 * @return      None
 ***************************************************************************/
void spi_ssm_state(SPI_RegDef_t *Instance, uint8_t state)
{
    if (state == ENABLE)
    {
        Instance->CR1 |= (1U << SPI_CR1_SSM);
    }
    else
    {
        Instance->CR1 &= ~(1U << SPI_CR1_SSM);
    }
}

/***************************************************************************
 * @brief       Controls the internal NSS (SSI) signal in software mode
 *
 * @details     This function sets or clears the SSI (Internal Slave Select)
 *              bit in the SPI control register (CR1).
 *
 *              This is only relevant when Software Slave Management (SSM)
 *              is enabled.
 *
 *              Behavior:
 *              ---------
 *              - When SSM = 1:
 *                  SSI = 1 -> NSS is internally considered HIGH (not selected)
 *                  SSI = 0 -> NSS is internally considered LOW (selected)
 *
 *              In master mode, SSI must typically be set to 1 to prevent
 *              MODF (mode fault) errors when hardware NSS is not used.
 *
 *              This is commonly used when:
 *                  - Manual GPIO-based CS is implemented
 *                  - Hardware NSS pin is not used
 *
 * @param[in]   Instance
 *              Pointer to the SPI peripheral base address.
 *
 * @param[in]   state
 *              ENABLE  -> Set SSI bit
 *              DISABLE -> Clear SSI bit
 *
 * @return      None
 ***************************************************************************/
void spi_ssi_state(SPI_RegDef_t *Instance, uint8_t state)
{
	if (state == ENABLE)
	{
		Instance->CR1 |= (1U << SPI_CR1_SSI);
	}
	else
	{
		Instance->CR1 &= ~(1U << SPI_CR1_SSI);
	}
}

/***************************************************************************
 * @brief       Enables or disables hardware NSS output in master mode
 *
 * @details     This function controls the SSOE (Slave Select Output Enable)
 *              bit in the SPI control register (CR2).
 *
 *              When enabled in master mode:
 *                  - The SPI peripheral automatically drives the NSS pin
 *                  - NSS is managed by hardware based on SPI state
 *
 *              When disabled:
 *                  - NSS pin is not driven by SPI peripheral
 *                  - NSS must be managed externally (e.g., GPIO-based CS)
 *
 *              NOTE:
 *              -----
 *              - This is only meaningful when SSM = 0 (hardware NSS mode)
 *              - Should NOT be used when manual GPIO CS is implemented
 *              - Behavior of NSS may be tied to SPE and internal state,
 *                making it less flexible for custom protocols
 *
 * @param[in]   Instance
 *              Pointer to the SPI peripheral base address.
 *
 * @param[in]   state
 *              ENABLE  -> SPI controls NSS pin automatically
 *              DISABLE -> NSS pin not controlled by SPI
 *
 * @return      None
 ***************************************************************************/
void spi_ssoe_state(SPI_RegDef_t *Instance, uint8_t state)
{
    if (state == ENABLE)
    {
        Instance->CR2 |= (1U << SPI_CR2_SSOE);
    }
    else
    {
        Instance->CR2 &= ~(1U << SPI_CR2_SSOE);
    }
}

/***************************************************************************
 * @brief       Receives data over SPI in Master Mode (Blocking API)
 *
 * @details     This function receives data from an SPI slave by generating
 *              clock pulses through dummy writes to the SPI data register.
 *
 *              Since SPI is a full-duplex protocol, data reception is only
 *              possible when the master transmits data (dummy bytes).
 *
 *              This function:
 *                  - Writes dummy data to generate clock
 *                  - Waits for RXNE flag
 *                  - Reads received data into buffer
 *                  - Ensures transfer completion using BSY flag
 *
 * @param[in]   pSPIx_addr
 *              Pointer to SPI peripheral base address
 *
 * @param[out]  pRX_buffer
 *              Pointer to buffer where received data will be stored
 *
 * @param[in]   data_len
 *              Number of bytes to receive
 *
 * @return      None
 *
 * @note        This function assumes 8-bit data frame format.
 ***************************************************************************/
void spi_receive_data(SPI_RegDef_t *pSPIx_addr, uint8_t *pRX_buffer, uint32_t data_len)
{
    while (data_len > 0)
    {
        // Wait until transmit buffer is empty
        while (!(pSPIx_addr->SR & (1U << SPI_SR_TXE)));

        // Send dummy byte to generate clock over SPI as by itself the master should generate clk even if its receving data from slave module {here esp32p4}
        *((volatile uint8_t *)&pSPIx_addr->DR) = 0xFF;  // dummy write

        while (!(pSPIx_addr->SR & (1U << SPI_SR_RXNE))); // Wait until data is received (RXNE = 1)
        
        // Check data frame settings
        if (!(pSPIx_addr->CR1 & (1U << SPI_CR1_DFF)))
        {
            // 8-Bit DFF:
            *(pRX_buffer) = *((volatile uint8_t *)&pSPIx_addr->DR);
            pRX_buffer++;
            data_len--;
        }else
        {
            // 16-Bit DFF:
            *((uint16_t *)pRX_buffer) = *((volatile uint16_t *)&pSPIx_addr->DR);
            pRX_buffer += 2;
            data_len -= 2;
        }
    }

    while (pSPIx_addr->SR & (1U << SPI_SR_BSY)); // Wait until SPI is not busy!
}

/******************************************************************************
 * @brief  Transfer one byte over SPI2.
 *
 * @param  tx_byte Byte transmitted on MOSI.
 *
 * @return Byte received simultaneously from MISO.
 *
 * @note
 * SPI is full duplex. Even when we only want to receive from the PN532,
 * the STM32 must transmit a dummy byte to generate SCK pulses.
 ******************************************************************************/
uint8_t spi_transfer_data(SPI_RegDef_t *pSPIx_addr, uint8_t pTX_byte)
{
    // Wait until transmit buffer is empty
    while (!(pSPIx_addr->SR & (1U << SPI_SR_TXE)));

    // Send dummy byte to generate clock over SPI as by itself the master should generate clk even if its receving data from slave module {here esp32p4}
    // *((volatile uint8_t *)&pSPIx_addr->DR) = 0xFF;  // dummy write
    *(volatile uint8_t *)&SPI2->DR = pTX_byte; // No need for dummy bytes if module in communication has a defined spi protocol

    /* Wait until one received byte is available. */
    while (!(pSPIx_addr->SR & (1U << SPI_SR_RXNE))); 

    return *(volatile uint8_t *)&SPI2->DR;
}