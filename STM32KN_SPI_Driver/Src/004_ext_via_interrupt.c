/*
 * 004_exti_via_interrupt.c
 *
 * Created on : Mar 8, 2026
 * Author     : varun.s.patil
 *
 * Experiment number : 004
 * Topic             : GPIO interrupt using EXTI
 * Objective         : Toggle LED when on-board push button interrupt occurs
 * Hardware used     : STM32F407 Discovery board
 *
 * GPIO used:
 * LED        -> PD12
 * Push button-> PA0 (User button B1 on board)
 *
 * NOTE:
 * The STM32F407 Discovery board has a built-in user button (B1)
 * connected to PA0. This experiment configures PA0 to generate an
 * external interrupt using the EXTI controller.
 */

/*
 * Experiment description
 *
 * This experiment demonstrates how to use GPIO interrupts instead
 * of polling to detect a push button press.
 *
 * The on-board user button connected to PA0 generates an interrupt
 * when the configured edge is detected (rising edge when the button
 * is pressed).
 *
 * When the interrupt occurs:
 *
 *      - EXTI detects the trigger
 *      - NVIC forwards the interrupt to the CPU
 *      - The ISR (EXTI0_IRQHandler) executes
 *      - The LED connected to PD12 toggles
 *
 * This experiment demonstrates:
 *
 * - GPIO interrupt mode configuration
 * - SYSCFG EXTI port mapping
 * - EXTI trigger configuration
 * - NVIC interrupt enable
 * - Writing a basic Interrupt Service Routine (ISR)
 */

#include "main.h"
#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

GPIO_Handle_t led;

volatile uint8_t btn_flag = 0;

int main(void)
{
    GPIO_Init(&led);

    /* Loop forever */
      while (1)
      {
    	  if(btn_flag)
    	  {
    	      btn_flag = 0;

    	      if(gpio_get_pin_level(GPIOA, GPIO_PIN_0))
    	      {
    	          gpio_set_pin_level(GPIOD, GPIO_PIN_12, ENABLE);
    	      }
    	      else
    	      {
    	          gpio_set_pin_level(GPIOD, GPIO_PIN_12, DISABLE);
    	      }
    	      delay_blk(30);   // debounce
    	  }
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

    /* configure PA0 as interrupt pin */
    pGPIOxHandle->pGPIOx_addr = GPIOA;
    pGPIOxHandle->pin_config.Mode = GPIO_MODE_IT_RISING_FALLING;
    pGPIOxHandle->pin_config.Pin = GPIO_PIN_0;
    pGPIOxHandle->pin_config.Pull = GPIO_NOPULL;
    gpio_init(pGPIOxHandle);
    gpio_irq_config(EXTI0_IRQn, 2, ENABLE);
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

void EXTI0_IRQHandler(void)
{
    /* clear pending interrupt */
    gpio_irq_handle(GPIO_PIN_0);

    btn_flag = 1; // Set flag to indicate btn press!
}
