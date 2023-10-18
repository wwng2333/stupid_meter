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
	char Calc[12] = {0};
	float mAh = 0.0f;
	float mWh = 0.0f;
	float Voltage = 0.0f;
	float Current = 0.0f;
	float Power = 0.0f;
	
	delay_init();
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
	INA226_Init();
	LCD_Init();
	LCD_Init_Printline();	
// 	volatile crm_clocks_freq_type crm_clk_freq = {};
//	crm_clocks_freq_get((crm_clocks_freq_type*)&crm_clk_freq);
  /* add user code end 2 */
  while(1)
  {
    /* add user code begin 3 */
		Voltage = INA226_Read_Voltage();
		Current = INA226_Read_Current();
		Power = Voltage * Current;
		
		if(Voltage < 10) sprintf(Calc, "%.3fV", Voltage);
		else sprintf(Calc, "%.2fdV", Voltage);
		LCD_ShowString2416(0, 2, Calc, LIGHTBLUE, BLACK);
		
		if(Current < 0.1) sprintf(Calc, "%.1fmA", Current * 1000);
		else if(Current < 1) sprintf(Calc, "%.0fmA", Current * 1000);
		else sprintf(Calc, "%.3fA", Current);
		LCD_ShowString2416(0, 29, Calc, BLUE, BLACK);
		
		if(Power < 10) sprintf(Calc, "%.3fW", Power);
		else if(Power < 100) sprintf(Calc, "%.2fW", Power);
		else sprintf(Calc, "%.1fW", Power);
		LCD_ShowString2416(0, 56, Calc, GBLUE, BLACK);
    /* add user code end 3 */
  }
}
