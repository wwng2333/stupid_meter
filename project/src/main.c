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
const uint8_t TEXT_Buffer[64]={"Crazy AT32F421G8U7 W25Q32 SPI TEST!"};
uint8_t datatemp[64];
uint8_t usart1_rx_buffer[64];
volatile uint8_t usart1_rx_counter = 0x00;
char Calc[32] = {0};
float temp, vcc = 0.0f;
__IO uint16_t adc1_ordinary_valuetab[2] = {0};
__IO uint8_t EXINT_Counter = 0;
__IO uint8_t Status = 0;
struct Queue queue = { .front = 0, .rear = 0 , .max = 0};
uint8_t SavedPoint[SIZE] = {0};
//uint8_t spi1_tx_buffer[1];
/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */
void ADC1_ReadVCC(void)
{
	vcc = ((float)1.2 * 4095) / adc1_ordinary_valuetab[1];
	sprintf(Calc, "ADC VCC = %.2f", vcc);
	SEGGER_RTT_printf(0, "%s\r\n", Calc);
}

void ADC1_Readtemp(void)
{
	float tmp = 0.0f;
	tmp = (ADC_TEMP_BASE - (float)adc1_ordinary_valuetab[0] * vcc / 4096) / ADC_TEMP_SLOPE + 25;
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

  /* init dma1 channel3 */
  wk_dma1_channel3_init();
  /* config dma channel transfer parameter */
  /* user need to modify parameters memory_base_addr and buffer_size */

  /* init usart1 function. */
  //wk_usart1_init();
	//usart_interrupt_enable(USART1, USART_RDBF_INT, TRUE);

  /* init spi1 function. */
  wk_spi1_init();
	
  /* init spi2 function. */
  wk_spi2_init();
	
  /* init adc1 function. */
  wk_adc1_init();

  /* init exint function. */
  wk_exint_config();

  /* init gpio function. */
  wk_gpio_config();
	while(W25Q_ReadReg(W25X_ManufactDeviceID) != W25Q32)
	{
		SEGGER_RTT_printf(0, "W25Q32 Check Failed!\r\n");
	}
	//W25Q_Write((uint8_t*)TEXT_Buffer, 0x1000, 64);
	//W25Q_EraseChip();
	W25Q_Read(datatemp, 0x0, 64);
	W25Q_Read(datatemp, 0x40, 64);
	W25Q_Read(datatemp, 0x80, 64);

  /* init tmr15 function. */
  wk_tmr15_init();

  /* add user code begin 2 */
	INA226_Init();
	LCD_Init();	
	//LCD_Init_Printline();	
	LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
// 	volatile crm_clocks_freq_type crm_clk_freq = {};
//	crm_clocks_freq_get((crm_clocks_freq_type*)&crm_clk_freq);
  /* add user code end 2 */
	//int a, i=0, count=0;
  while(1)
  {
//    if ((a = SEGGER_RTT_WaitKey()) > 0) 
//    {
//			SEGGER_RTT_printf(0, "%c", a);
//			datatemp[i] = a;
//			i++;
//    }
//		if(i == sizeof(datatemp))
//		{
//			SEGGER_RTT_printf(0, "\r\n");
//			i = 0;
//			W25Q_Write((uint8_t*)datatemp, count*64, 64);
//			count++;
//		}
		
    /* add user code begin 3 */
		adc_ordinary_software_trigger_enable(ADC1, TRUE);
		ADC1_Readtemp();
		ADC1_ReadVCC();
		
		Voltage = INA226_Read_Voltage();
		Current = INA226_Read_Current();
		Power = Voltage * Current;
		enqueue(&queue, Current);
		mAh += 0.5 * (float)Current / 3.6;
		mWh += 0.5 * (float)Power / 3.6;
		
		if(EXINT_Counter)
		{
			Status = ~Status;
			EXINT_Counter = 0;
			LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
		}
		
		if(!Status)
		{
			LCD_DrawLine(88, 2, 88, 78, WHITE);
			LCD_DrawLine(89, 2, 89, 78, WHITE);
			if(Voltage < 10) sprintf(Calc, "%.3fV", Voltage);
			else sprintf(Calc, "%.2fV", Voltage);
			LCD_ShowString2416(0, 2, Calc, LIGHTBLUE, BLACK);
			SEGGER_RTT_printf(0, "%s\r\n", Calc);
			
			if(Current < 0.1) sprintf(Calc, "%.1fmA", Current * 1000);
			else if(Current < 1) sprintf(Calc, "%.0fmA", Current * 1000);
			else if(Current > 10) sprintf(Calc, "%.2fA", Current);
			else sprintf(Calc, "%.3fA", Current);
			LCD_ShowString2416(0, 29, Calc, BLUE, BLACK);
			SEGGER_RTT_printf(0, "%s\r\n", Calc);
			
			if(Power < 10) sprintf(Calc, "%.3fW", Power);
			else if(Power < 100) sprintf(Calc, "%.2fW", Power);
			else sprintf(Calc, "%.1fW", Power);
			LCD_ShowString2416(0, 56, Calc, GBLUE, BLACK);
			SEGGER_RTT_printf(0, "%s\r\n", Calc);
			
			sprintf(Calc, "MCU:%.1fC", temp);
			LCD_ShowString(96, 2, Calc, GBLUE, BLACK, 12, 0);
			sprintf(Calc, "Vcc:%.2fV", vcc);
			LCD_ShowString(96, 18, Calc, GBLUE, BLACK, 12, 0);
			sprintf(Calc, "%.2fmAh", mAh);
			LCD_ShowString(96, 34, Calc, GBLUE, BLACK, 12, 0);
			if(mWh < 10000) sprintf(Calc, "%.2fmWh", mWh);
			else sprintf(Calc, "%.1fmWh", mWh);
			LCD_ShowString(96, 50, Calc, GBLUE, BLACK, 12, 0);
			LCD_ShowString(96, 62, "<------", GBLUE, BLACK, 16, 0);
		}
		else 
		{
			LCD_DrawLine(0, 14, SIZE, 14, GBLUE);
			LCD_DrawLine(0, 46, SIZE, 46, GBLUE);
			LCD_DrawLine(0, 78, SIZE, 78, GBLUE);
			sprintf(Calc, "%.1fV %.3fA %.1fW %.1fC   ", Voltage, Current, Power, temp);
			LCD_ShowString(1, 1, Calc, GBLUE, BLACK, 12, 0);
			LCD_ShowString(SIZE+2, 70, "0.0A", GBLUE, BLACK, 12, 0);
			sprintf(Calc, "%.2f", queue.max/2);
			LCD_ShowString(SIZE+2, 40, Calc, GBLUE, BLACK, 12, 0);
			sprintf(Calc, "%.2f", queue.max);
			LCD_ShowString(SIZE+2, 13, Calc, GBLUE, BLACK, 12, 0);
			ClearPrint();
			printQueue(&queue);
		}
		delay_ms(500);
    /* add user code end 3 */
  }
}
