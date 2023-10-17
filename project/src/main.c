/* add user code begin Header */
/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "at32f421_wk_config.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */

/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/**
  * @brief main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* add user code begin 1 */

  /* add user code end 1 */

  /* system clock config. */
  wk_system_clock_config();

  /* config periph clock. */
  wk_periph_clock_config();

  /* nvic config. */
  wk_nvic_config();

  /* init spi1 function. */
  wk_spi1_init();

  /* init i2c1 function. */
  //wk_i2c1_init();

  /* init adc1 function. */
  //wk_adc1_init();

  /* init exint function. */
  //wk_exint_config();

  /* init gpio function. */
  wk_gpio_config();
  /* add user code begin 2 */
	SEGGER_RTT_printf(0, "Hello world AT32F421!\r\n");
	delay_init();
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	LCD_ShowString(10,20,"LCD_W:",RED,WHITE,16,0);
  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
		gpio_bits_set(GPIOA, GPIO_PINS_2);
		delay_ms(100);
		gpio_bits_reset(GPIOA, GPIO_PINS_2);
		LCD_ShowString(10,20,"LCD_W:",RED,WHITE,16,0);
    /* add user code end 3 */
  }
}
