/*
 * 002_push_btn_press.c
 *
 * Created on : Mar 7, 2026
 * Author     : varun.s.patil
 *
 * Experiment number : 002
 * Topic             : GPIO input using on-board user button
 * Objective         : Turn ON an LED when the user button is pressed
 * Hardware used     : STM32F407 Discovery board
 */

/*
 * Experiment description
 *
 * This experiment demonstrates reading a GPIO input pin and controlling
 * an LED based on the button state.
 *
 * The STM32F407 Discovery board has a built-in user push button connected
 * to pin PA0. This button can be used directly without any external wiring.
 *
 * When the button is pressed, the MCU reads the pin level and turns ON
 * an LED connected to PD12. When the button is released, the LED turns OFF.
 */

/*
 * On-board button connection (already wired on the board)
 *
 * PA0 ----> User button (B1)
 *
 * The button connects PA0 to VCC when pressed.
 *
 * Button released -> PA0 reads LOW
 * Button pressed  -> PA0 reads HIGH
 *
 * GPIO configuration required:
 *
 * Mode : Input
 * Pull : Pull-down
 */

/*
 * On-board LED connection
 *
 * PD12 ----> Green LED (LD4)
 *
 * The Discovery board LEDs are active-high in push-pull mode:
 *
 * PD12 = HIGH -> LED ON
 * PD12 = LOW  -> LED OFF
 */

/*
 * Note about button bounce
 *
 * Mechanical push buttons generate rapid signal fluctuations
 * when pressed or released. This phenomenon is called "switch
 * bouncing". For this basic experiment bouncing is ignored.
 *
 * Later experiments will demonstrate software debouncing
 * techniques.
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
        if (gpio_get_pin_level(GPIOA, GPIO_PIN_0))
        {
            gpio_set_pin_level(GPIOD, GPIO_PIN_12, 1);
        }
        else
        {
        	gpio_set_pin_level(GPIOD, GPIO_PIN_12, 0);
		}
        delay_blk(50);
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
    pGPIOxHandle->pin_config.OPType = GPIO_OP_PP;
    pGPIOxHandle->pin_config.Pull = GPIO_NOPULL;
    pGPIOxHandle->pin_config.Speed = GPIO_SPEED_FREQ_HIGH;

    /* configure PD12 */
    pGPIOxHandle->pin_config.Pin = GPIO_PIN_12;
    gpio_init(pGPIOxHandle);

    /* configure PA0 */
    pGPIOxHandle->pGPIOx_addr = GPIOA;
    pGPIOxHandle->pin_config.Mode = GPIO_MODE_INPUT;
    pGPIOxHandle->pin_config.Pin = GPIO_PIN_0;
    pGPIOxHandle->pin_config.Pull = GPIO_NOPULL;
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
