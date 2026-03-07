/*
 * 001_led_open_drain_mode.c
 *
 *  Created on: Mar 7, 2026
 *  Author: varun.s.patil
 * 	Experiment number : 001
 * 	Topic             : GPIO open drain mode
 * 	Objective         : Demonstrate open drain output behaviour on PD12 LED
 * 	Hardware used     : STM32F407 Discovery board
 */

/*
 * Open drain demo connection notes
 *
 * Why external circuit is needed:
 * Open drain output can pull the line LOW strongly, but it cannot drive HIGH.
 * When output data is written as 1, the pin becomes high impedance (Hi-Z).
 * Therefore a pull-up path is required to define the HIGH state.
 *
 * The internal pull-up is weak, so it is not suitable for clearly driving an LED.
 * Use an external resistor and LED for proper open drain demonstration.
 *
 * Recommended connection using PD12:
 *
 * 3.3V or 5V
 *   |
 *   |
 *  [R] 330 ohm to 470 ohm
 *   |
 *   +----->|----- PD12
 *          LED
 *
 * LED anode  -> resistor side
 * LED cathode -> PD12
 *
 * Open drain behavior in this circuit:
 *
 * When PD12 output = 0:
 * - MCU actively pulls PD12 to GND
 * - Current flows from VCC through resistor and LED into PD12
 * - LED turns ON
 *
 * When PD12 output = 1:
 * - PD12 becomes Hi-Z
 * - No current path to GND
 * - LED turns OFF
 *
 * GPIO configuration for this demo:
 * - Mode   : Output
 * - OType  : Open drain
 * - Pull   : No pull
 * - Speed  : Any valid output speed
 *
 * Internal pull-up or pull-down is not needed here because the external
 * resistor and LED path already create the required behavior for the demo.
 */

/*
 * Common use cases of open drain outputs
 *
 * 1. I2C communication
 *    SDA and SCL lines use open drain with pull-up resistors.
 *
 * 2. Shared signal lines between multiple devices
 *    Many devices can safely connect to the same line without fighting
 *    each other because no device drives the line HIGH directly.
 *
 * 3. Wired-AND / wired-OR style logic
 *    Any device can pull the line LOW, while the pull-up keeps it HIGH
 *    when no device is active.
 *
 * 4. Level shifting and interfacing
 *    Open drain outputs can be pulled up to another voltage domain
 *    through an external resistor, if the pin is voltage tolerant and
 *    the MCU datasheet allows it.
 *
 * 5. Interrupt or fault lines shared by multiple devices
 *    Multiple devices can signal an event by pulling the same line LOW.
 */

#include "main.h"
#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

GPIO_Handle_t led;

int main(void)
{
    GPIO_Init(&led);

    /* Loop forever */
	  while (1)
	  {
		  gpio_set_pin_level(GPIOD, GPIO_PIN_12, 1);
		  delay_blk(25);
		  gpio_set_pin_level(GPIOD, GPIO_PIN_12, 0);
	  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

void GPIO_Init(GPIO_Handle_t *pGPIOxHandle)
{
    pGPIOxHandle->pGPIOx_addr = GPIOD;
    pGPIOxHandle->pin_config.Mode = GPIO_MODE_OUTPUT;
    pGPIOxHandle->pin_config.OPType = GPIO_OP_OD;
    pGPIOxHandle->pin_config.Pull = GPIO_NOPULL; // Use external pull-up so that open-drain HIGH state is defined & not internal R as its very large 40KOhms
    pGPIOxHandle->pin_config.Speed = GPIO_SPEED_FREQ_HIGH;

    /* configure PD12 */
    pGPIOxHandle->pin_config.Pin = GPIO_PIN_12;
    gpio_init(pGPIOxHandle);
}

/*
 * Simple blocking delay.
 * Assumes system clock = 16 MHz (HSI default after reset).
 *
 * One loop iteration roughly takes ~4 CPU cycles.
 * 16 MHz / 4 = ~4,000,000 iterations per second.
 *
 * Therefore:
 * 4000 iterations ≈ 1 ms delay.
 */

void delay_blk(uint32_t ms)
{
  for (uint32_t time_ms = 0; time_ms < ms; time_ms++)
  {
    for(volatile uint32_t j = 0; j < 4000; j++)
    {
        __asm__("nop");
    }
  }
}

