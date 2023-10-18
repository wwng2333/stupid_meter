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
char Calc[20] = {0};
float temp, vcc = 0.0f;
__IO uint16_t adc1_ordinary_valuetab[2] = {0};
/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */
void ADC1_ReadVCC(void)
{
	vcc = ((double)1.2 * 4095) / adc1_ordinary_valuetab[1];
	sprintf(Calc, "ADC VCC = %.2f", vcc);
	SEGGER_RTT_printf(0, "%s\r\n", Calc);
}

void ADC1_Readtemp(void)
{
	float tmp = 0.0f;
	tmp = (ADC_TEMP_BASE - (double)adc1_ordinary_valuetab[0] * vcc / 4096) / ADC_TEMP_SLOPE + 25;
	if(tmp > 0) temp = tmp;
	sprintf(Calc, "ADC Temp = %.2f", temp);
	SEGGER_RTT_printf(0, "%s\r\n", Calc);
}
/* add user code end 0 */

/**
  * @brief main function.
  * @param  none
  * @retval none
  */
	
int main(void)
{
  /* add user code begin 1 */
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

  /* init dma1 channel1 */
  wk_dma1_channel1_init();
  /* config dma channel transfer parameter */
  /* user need to modify parameters memory_base_addr and buffer_size */
  wk_dma_channel_config(DMA1_CHANNEL1, (uint32_t)&ADC1->odt, (uint32_t)adc1_ordinary_valuetab, 2);
  dma_channel_enable(DMA1_CHANNEL1, TRUE);
	adc_ordinary_software_trigger_enable(ADC1, TRUE);
	
  /* init spi1 function. */
  wk_spi1_init();

  /* init adc1 function. */
  wk_adc1_init();

  /* init exint function. */
  wk_exint_config();

  /* init gpio function. */
  wk_gpio_config();

  /* init tmr15 function. */
  wk_tmr15_init();

  /* add user code begin 2 */
	INA226_Init();
	LCD_Init();	
	LCD_Init_Printline();	
// 	volatile crm_clocks_freq_type crm_clk_freq = {};
//	crm_clocks_freq_get((crm_clocks_freq_type*)&crm_clk_freq);
  /* add user code end 2 */
  while(1)
  {
		adc_ordinary_software_trigger_enable(ADC1, TRUE);
		ADC1_Readtemp();
		ADC1_ReadVCC();
		
    /* add user code begin 3 */
		Voltage = INA226_Read_Voltage();
		Current = INA226_Read_Current();
		Power = Voltage * Current;
		
		if(Voltage < 10) sprintf(Calc, "%.3fV", Voltage);
		else sprintf(Calc, "%.2fV", Voltage);
		LCD_ShowString2416(0, 2, Calc, LIGHTBLUE, BLACK);
		SEGGER_RTT_printf(0, "%s\r\n", Calc);
		
		if(Current == 0) sprintf(Calc, "%.2fmA", Current * 1000);
		else if(Current < 0.1) sprintf(Calc, "%.1fmA", Current * 1000);
		else if(Current < 1) sprintf(Calc, "%.0fmA", Current * 1000);
		else sprintf(Calc, "%.3fA", Current);
		LCD_ShowString2416(0, 29, Calc, BLUE, BLACK);
		SEGGER_RTT_printf(0, "%s\r\n", Calc);
		
		if(Power < 10) sprintf(Calc, "%.3fW", Power);
		else if(Power < 100) sprintf(Calc, "%.2fW", Power);
		else sprintf(Calc, "%.1fW", Power);
		LCD_ShowString2416(0, 56, Calc, GBLUE, BLACK);
		SEGGER_RTT_printf(0, "%s\r\n", Calc);
		
		sprintf(Calc, "Core:%.1fC", temp);
		LCD_ShowString(92, 0, Calc, GBLUE, BLACK, 12, 0);
		sprintf(Calc, "Vcc: %.2fV", vcc);
		LCD_ShowString(92, 16, Calc, GBLUE, BLACK, 12, 0);
		delay_ms(500);
    /* add user code end 3 */
  }
}
