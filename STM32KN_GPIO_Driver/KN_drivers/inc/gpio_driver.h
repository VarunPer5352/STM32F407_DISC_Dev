/*
 * gpio_driver.h
 *
 *  Created on: Mar 6, 2026
 *      Author: varun.s.patil
 */

#ifndef INC_GPIO_DRIVER_H_
#define INC_GPIO_DRIVER_H_

#include "stm32f407xx.h"

/************************************************************************/
/* A Configuration Structure for GPIO pin holding all its settings */
/************************************************************************/
typedef struct{
    uint32_t Pin;       /* Specifies the GPIO pin number to be configured.*/
    uint32_t Mode;      /* Specifies the operating mode for the selected pins.*/
    uint32_t Pull;      /* Specifies the Pull-up or Pull-Down activation for the selected pins.*/
    uint32_t OPType;    /* Specifies the OutPut type settings Open drain, push pull etc.*/
    uint32_t Speed;     /* Specifies the speed for the selected pins.*/
    uint32_t Alternate; /* Peripheral to be connected to the selected pins.*/
}GPIO_Config_t;

/************************************************************************/
/* A Handle Structure for GPIO pin */
/************************************************************************/
typedef struct{
    GPIO_RegDef_t *pGPIOx_addr; // Pointer to hold the addresss of a GPIO{A-I} periph which has the pin.
    GPIO_Config_t pin_config; // This has all the settings attributed to that GPIO, this is used to store settings to be written into Reg's
}GPIO_Handle_t;

/************************************************************************/
/* Function prototypes for GPIO specific API's */
/************************************************************************/
void gpio_clk_ctrl(void);
void gpio_init(void);
void gpio_deinit(void);
void gpio_get_pin_level(void);
void gpio_set_pin_level(void);
void gpio_get_port_level(void);
void gpio_set_port_level(void);
void gpio_toggle_pin(void);
void gpio_irq_config(void);
void gpio_irq_handle(void);

#endif /* INC_GPIO_DRIVER_H_ */
