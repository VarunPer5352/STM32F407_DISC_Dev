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

/****************************************************
 * @intro:
 *      This function initializes a GPIO pin based on
 *      the configuration provided in the GPIO handle
 *      structure.
 *
 * @param[in]:
 *      GPIO_Handle_t *pGPIO_handle
 *      Pointer to a GPIO handle structure which contains:
 *          1. Pointer to the GPIO peripheral registers
 *             (GPIO_RegDef_t *pGPIOx_addr)
 *          2. Pin configuration parameters
 *             (GPIO_Config_t pin_config)
 *
 *      The configuration values stored in PinConfig are
 *      used to configure the corresponding GPIO registers
 *      such as MODER, OTYPER, OSPEEDR, PUPDR, etc.
 *
 * @return:
 *      None
 *
 * @Note:
 *      The GPIO peripheral clock must be enabled before
 *      configuring the registers.
 */
void gpio_init(GPIO_Handle_t *pGPIO_handle)
{
    /* Enable peripheral clock */
    gpio_clk_ctrl(pGPIO_handle->pGPIOx_addr, ENABLE);

    // 1. Configure the mode of a GPIO pin.
    if (pGPIO_handle->pin_config.Mode <= GPIO_MODE_ANALOG) // GPIO_MODE_ANALOG -> 3U this is non-interrupt modes
    {
        /* clear the 2 bits corresponding to the pin in MODER reg */
        pGPIO_handle->pGPIOx_addr->MODER &= ~(3U << (2 * pGPIO_handle->pin_config.Pin)); // 3U is 11b thus ANDing it will clear those bits.
        
        /* Then write the desired mode of the pin u want to set in MODER reg */
        pGPIO_handle->pGPIOx_addr->MODER |= pGPIO_handle->pin_config.Mode << (2 * pGPIO_handle->pin_config.Pin);
    }
    else
    {
        // Interrupt mode programming!
        
    }

    // 2. Configure the speed of a GPIO pin.
    pGPIO_handle->pGPIOx_addr->OSPEEDR &= ~(3U << (2 * pGPIO_handle->pin_config.Pin));
    pGPIO_handle->pGPIOx_addr->OSPEEDR |= pGPIO_handle->pin_config.Speed << (2 * pGPIO_handle->pin_config.Pin);

    // 3. Configure the internal resistor type of a GPIO pin.
    pGPIO_handle->pGPIOx_addr->PUPDR &= ~(3U << (2 * pGPIO_handle->pin_config.Pin));
    pGPIO_handle->pGPIOx_addr->PUPDR |= pGPIO_handle->pin_config.Pull << (2 * pGPIO_handle->pin_config.Pin);

    // 4. Configure the Output type of a GPIO pin.
    pGPIO_handle->pGPIOx_addr->OTYPER &= ~(1U << (1 * pGPIO_handle->pin_config.Pin));
    pGPIO_handle->pGPIOx_addr->OTYPER |= pGPIO_handle->pin_config.OPType << (1 * pGPIO_handle->pin_config.Pin);   

    // 5. Aletrnate functionality mode for a GPIO pin.
    if (pGPIO_handle->pin_config.Mode == GPIO_MODE_ALTFN)
    {
        /*
            Each GPIO pin configured for alternate function uses 4 bits in the AFR registers.

            There are two AFR registers:
            AFR[0] -> controls pins 0 to 7
            AFR[1] -> controls pins 8 to 15

            To select the correct AFR register:
            Pin / 8
            Pins 0-7  -> AFR[0]
            Pins 8-15 -> AFR[1]

            Inside each AFR register, every pin occupies 4 bits:
            Pin0 -> bits 3:0
            Pin1 -> bits 7:4
            Pin2 -> bits 11:8
            ...
            Pin7 -> bits 31:28

            To find the position of the 4-bit field within the AFR register:
            (Pin % 8) gives the pin position inside the AFR register (0 to 7)

            Multiplying by 4 gives the bit shift needed because each pin uses 4 bits:
            shift = 4 * (Pin % 8)

            Then:
            1. Clear the 4-bit field using mask ~(15U << shift)
            2. Write the new alternate function value into those bits
        */
        // Clearing 4 bit locations of AFR
        pGPIO_handle->pGPIOx_addr->AFR[pGPIO_handle->pin_config.Pin / 8] &= ~(15U << (4 * (pGPIO_handle->pin_config.Pin % 8)));

        // Then writing the desired AFR setting.
        pGPIO_handle->pGPIOx_addr->AFR[pGPIO_handle->pin_config.Pin / 8] |= (pGPIO_handle->pin_config.Alternate << (4 * (pGPIO_handle->pin_config.Pin % 8)));
    }
}

/****************************************************
 * @intro:
 *      This function resets a GPIO peripheral to its
 *      default reset state.
 *
 * @param[in]:
 *      GPIO_RegDef_t *pGPIOx_addr
 *      Pointer to the GPIO peripheral whose registers
 *      need to be reset.
 *
 *      The function uses the RCC peripheral reset
 *      registers to reset the entire GPIO port.
 *
 * @return:
 *      None
 *
 * @Note:
 *      This reset affects the entire GPIO peripheral
 *      (GPIOA, GPIOB, etc) and not individual pins.
 *      This is useful when:
 *          shutting down a driver
 *          releasing a peripheral
 *          reinitializing hardware
 *          debugging
 *     It is not used during normal pin configuration.
 */
void gpio_deinit(GPIO_RegDef_t *pGPIOx_addr)
{
    if (pGPIOx_addr == GPIOA)
    {
        GPIOA_REG_RESET();
    }
    else if (pGPIOx_addr == GPIOB)
    {
        GPIOB_REG_RESET();
    }
    else if (pGPIOx_addr == GPIOC)
    {
        GPIOC_REG_RESET();
    }
    else if (pGPIOx_addr == GPIOD)
    {
        GPIOD_REG_RESET();
    }
    else if (pGPIOx_addr == GPIOE)
    {
        GPIOE_REG_RESET();
    }
    else if (pGPIOx_addr == GPIOF)
    {
        GPIOF_REG_RESET();
    }
    else if (pGPIOx_addr == GPIOG)
    {
        GPIOG_REG_RESET();
    }
    else if (pGPIOx_addr == GPIOH)
    {
        GPIOH_REG_RESET();
    }
    else if (pGPIOx_addr == GPIOI)
    {
        GPIOI_REG_RESET();
    }
}

/****************************************************
 * @intro:
 *      This function reads the logic level present on
 *      a specific GPIO pin.
 *
 * @param[in]:
 *      GPIO_RegDef_t *pGPIOx_addr
 *      Pointer to the GPIO peripheral registers
 *      (GPIOA, GPIOB, GPIOC, etc).
 *
 * @param[in]:
 *      uint8_t pin_number
 *      Specifies the GPIO pin number whose logic
 *      level needs to be read.
 *
 * @return:
 *      uint8_t
 *      Returns the current logic level of the pin.
 *          0 -> Logic LOW
 *          1 -> Logic HIGH
 *
 * @Note:
 *      The function reads the state of the pin from
 *      the IDR (Input Data Register) of the GPIO
 *      peripheral.
 */
uint8_t gpio_get_pin_level(GPIO_RegDef_t *pGPIOx_addr, uint8_t pin_number)
{
    uint8_t pin_state = (uint8_t)(pGPIOx_addr->IDR >> pin_number) & 1U; // >> shifting will give make the reg for that pin to @bit0, ANDing with 1U will only retai that one bit rest all 0!
    return pin_state;
}