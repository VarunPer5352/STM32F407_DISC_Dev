/*
 * gpio_driver.h
 *
 *  Created on: Mar 6, 2026
 *      Author: varun.s.patil
 */

#ifndef INC_GPIO_DRIVER_H_
#define INC_GPIO_DRIVER_H_

#include "stm32f407xx.h"

/**
 * @GPIO_pins_define: MACROs for pin number's.
 */
#define GPIO_PIN_0                 ((uint32_t)0U)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint32_t)1U)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint32_t)2U)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint32_t)3U)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint32_t)4U)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint32_t)5U)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint32_t)6U)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint32_t)7U)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint32_t)8U)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint32_t)9U)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint32_t)10U)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint32_t)11U)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint32_t)12U)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint32_t)13U)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint32_t)14U)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint32_t)15U)  /* Pin 15 selected   */
#define GPIO_PIN_MASK              0x0000FFFFU /* PIN mask for assert test */

/**
 * @GPIO_mode_define: MACRO's for different modes.
 */
#define GPIO_MODE_INPUT                 0U
#define GPIO_MODE_OUTPUT                1U
#define GPIO_MODE_ALTFN                 2U
#define GPIO_MODE_ANALOG                3U
#define GPIO_MODE_IT_FALLING            4U
#define GPIO_MODE_IT_RISING             5U
#define GPIO_MODE_IT_RISING_FALLING     6U

/**
 * @Output_type_define: Macros for diff Output types settings
 */
#define GPIO_OP_PP 0U
#define GPIO_OP_OD 1U

/**
 * @Speed_mode_define: GPIO speed define
 */
#define  GPIO_SPEED_FREQ_LOW         0U  /*!< IO works at 2 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_MEDIUM      1U  /*!< range 12,5 MHz to 50 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_HIGH        2U  /*!< range 25 MHz to 100 MHz, please refer to the product datasheet  */
#define  GPIO_SPEED_FREQ_VERY_HIGH   3U  /*!< range 50 MHz to 200 MHz, please refer to the product datasheet  */

/**
 * @Pull_up_don_defines: Possible Internal Resistor config Macros.
 */
#define  GPIO_NOPULL        0U   /*!< No Pull-up or Pull-down activation  */
#define  GPIO_PULLUP        1U   /*!< Pull-up activation                  */
#define  GPIO_PULLDOWN      2U   /*!< Pull-down activation                */

/**
 * @Alternate_pin_mode_define: These are the MACROs for Alternate function of GPIO's
 */
/** 
  * @brief   AF 0 selection  
  */ 
 #define GPIO_AF0_RTC_50Hz      ((uint8_t)0x00)  /* RTC_50Hz Alternate Function mapping                       */
 #define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO (MCO1 and MCO2) Alternate Function mapping            */
 #define GPIO_AF0_TAMPER        ((uint8_t)0x00)  /* TAMPER (TAMPER_1 and TAMPER_2) Alternate Function mapping */
 #define GPIO_AF0_SWJ           ((uint8_t)0x00)  /* SWJ (SWD and JTAG) Alternate Function mapping             */
 #define GPIO_AF0_TRACE         ((uint8_t)0x00)  /* TRACE Alternate Function mapping                          */
 
 /** 
   * @brief   AF 1 selection  
   */ 
 #define GPIO_AF1_TIM1          ((uint8_t)0x01)  /* TIM1 Alternate Function mapping */
 #define GPIO_AF1_TIM2          ((uint8_t)0x01)  /* TIM2 Alternate Function mapping */
 
 /** 
   * @brief   AF 2 selection  
   */ 
 #define GPIO_AF2_TIM3          ((uint8_t)0x02)  /* TIM3 Alternate Function mapping */
 #define GPIO_AF2_TIM4          ((uint8_t)0x02)  /* TIM4 Alternate Function mapping */
 #define GPIO_AF2_TIM5          ((uint8_t)0x02)  /* TIM5 Alternate Function mapping */
 
 /** 
   * @brief   AF 3 selection  
   */ 
 #define GPIO_AF3_TIM8          ((uint8_t)0x03)  /* TIM8 Alternate Function mapping  */
 #define GPIO_AF3_TIM9          ((uint8_t)0x03)  /* TIM9 Alternate Function mapping  */
 #define GPIO_AF3_TIM10         ((uint8_t)0x03)  /* TIM10 Alternate Function mapping */
 #define GPIO_AF3_TIM11         ((uint8_t)0x03)  /* TIM11 Alternate Function mapping */
 
 /** 
   * @brief   AF 4 selection  
   */ 
 #define GPIO_AF4_I2C1          ((uint8_t)0x04)  /* I2C1 Alternate Function mapping */
 #define GPIO_AF4_I2C2          ((uint8_t)0x04)  /* I2C2 Alternate Function mapping */
 #define GPIO_AF4_I2C3          ((uint8_t)0x04)  /* I2C3 Alternate Function mapping */
 
 /** 
   * @brief   AF 5 selection  
   */ 
 #define GPIO_AF5_SPI1          ((uint8_t)0x05)  /* SPI1 Alternate Function mapping        */
 #define GPIO_AF5_SPI2          ((uint8_t)0x05)  /* SPI2/I2S2 Alternate Function mapping   */
 #define GPIO_AF5_I2S3ext       ((uint8_t)0x05)  /* I2S3ext_SD Alternate Function mapping  */
 
 /** 
   * @brief   AF 6 selection  
   */ 
 #define GPIO_AF6_SPI3          ((uint8_t)0x06)  /* SPI3/I2S3 Alternate Function mapping  */
 #define GPIO_AF6_I2S2ext       ((uint8_t)0x06)  /* I2S2ext_SD Alternate Function mapping */
 
 /** 
   * @brief   AF 7 selection  
   */ 
 #define GPIO_AF7_USART1        ((uint8_t)0x07)  /* USART1 Alternate Function mapping     */
 #define GPIO_AF7_USART2        ((uint8_t)0x07)  /* USART2 Alternate Function mapping     */
 #define GPIO_AF7_USART3        ((uint8_t)0x07)  /* USART3 Alternate Function mapping     */
 #define GPIO_AF7_I2S3ext       ((uint8_t)0x07)  /* I2S3ext_SD Alternate Function mapping */
 
 /** 
   * @brief   AF 8 selection  
   */ 
 #define GPIO_AF8_UART4         ((uint8_t)0x08)  /* UART4 Alternate Function mapping  */
 #define GPIO_AF8_UART5         ((uint8_t)0x08)  /* UART5 Alternate Function mapping  */
 #define GPIO_AF8_USART6        ((uint8_t)0x08)  /* USART6 Alternate Function mapping */
 
 /** 
   * @brief   AF 9 selection 
   */ 
 #define GPIO_AF9_CAN1          ((uint8_t)0x09)  /* CAN1 Alternate Function mapping  */
 #define GPIO_AF9_CAN2          ((uint8_t)0x09)  /* CAN2 Alternate Function mapping  */
 #define GPIO_AF9_TIM12         ((uint8_t)0x09)  /* TIM12 Alternate Function mapping */
 #define GPIO_AF9_TIM13         ((uint8_t)0x09)  /* TIM13 Alternate Function mapping */
 #define GPIO_AF9_TIM14         ((uint8_t)0x09)  /* TIM14 Alternate Function mapping */
 
 /** 
   * @brief   AF 10 selection  
   */ 
 #define GPIO_AF10_OTG_FS        ((uint8_t)0x0A)  /* OTG_FS Alternate Function mapping */
 #define GPIO_AF10_OTG_HS        ((uint8_t)0x0A)  /* OTG_HS Alternate Function mapping */
 
 /** 
   * @brief   AF 11 selection  
   */ 
 #define GPIO_AF11_ETH           ((uint8_t)0x0B)  /* ETHERNET Alternate Function mapping */
 
 /** 
   * @brief   AF 12 selection  
   */ 
 #define GPIO_AF12_FSMC          ((uint8_t)0x0C)  /* FSMC Alternate Function mapping                     */
 #define GPIO_AF12_OTG_HS_FS     ((uint8_t)0x0C)  /* OTG HS configured in FS, Alternate Function mapping */
 #define GPIO_AF12_SDIO          ((uint8_t)0x0C)  /* SDIO Alternate Function mapping                     */
 
 /** 
   * @brief   AF 13 selection  
   */ 
 #define GPIO_AF13_DCMI          ((uint8_t)0x0D)  /* DCMI Alternate Function mapping */
 
 /** 
   * @brief   AF 15 selection  
   */ 
 #define GPIO_AF15_EVENTOUT      ((uint8_t)0x0F)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief  HAL Status structures definition  
  */  
 typedef enum 
 {
   HAL_OK       = 0x00U,
   HAL_ERROR    = 0x01U,
   HAL_BUSY     = 0x02U,
   HAL_TIMEOUT  = 0x03U
 } HAL_StatusTypeDef;

/************************************************************************/
/* A Configuration Structure for GPIO pin holding all its settings */
/************************************************************************/
typedef struct{
    uint32_t Pin;       /* Specifies the GPIO pin number to be configured. Refer to @GPIO_pins_define */
    uint32_t Mode;      /* Specifies the operating mode for the selected pins. Refer to @GPIO_mode_define */
    uint32_t Pull;      /* Specifies the Pull-up or Pull-Down activation for the selected pins. Refer to @Pull_up_don_defines */
    uint32_t OPType;    /* Specifies the OutPut type settings Open drain, push pull etc. Refer to @Output_type_define */
    uint32_t Speed;     /* Specifies the speed for the selected pins. Refer to @Speed_mode_define */
    uint32_t Alternate; /* Peripheral to be connected to the selected pins. Refer to @Alternate_pin_mode_define */
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
/**
 * API to control clock enabling & disabling for a GPIO peripheral{A-I} 
 */
void gpio_clk_ctrl(GPIO_RegDef_t *pGPIOx_addr, uint8_t state);

/**
 * API's to init/deinit a gpio pin as per settings!
 */
void gpio_init(GPIO_Handle_t *pGPIO_handle);
void gpio_deinit(GPIO_RegDef_t *pGPIOx_addr);

/**
 * API's to read/write onto/from a gpio pin/port!
 */
uint8_t gpio_get_pin_level(GPIO_RegDef_t *pGPIOx_addr, uint8_t pin_number);
void gpio_set_pin_level(GPIO_RegDef_t *pGPIOx_addr, uint8_t pin_number, uint8_t pin_state);
uint16_t gpio_get_port_level(GPIO_RegDef_t *pGPIOx_addr);
void gpio_set_port_level(GPIO_RegDef_t *pGPIOx_addr, uint16_t port_state);
void gpio_toggle_pin(GPIO_RegDef_t *pGPIOx_addr, uint8_t pin_number);

/**
 * API's for Interrupt Handling
 */
void gpio_irq_config(IRQn_Type IRQ_number, uint8_t IRQ_priority, uint8_t state);
void gpio_irq_handle(uint8_t pin_number);

#endif /* INC_GPIO_DRIVER_H_ */
