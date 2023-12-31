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
__IO uint8_t Status = 0;
__IO uint8_t USE_HORIZONTAL = 3;
__IO uint8_t TMR16_Main_Flag = 0;
struct Queue Voltage_queue = { .front = 0, .rear = 0};
struct Queue Current_queue = { .front = 0, .rear = 0};
struct Queue Power_queue = { .front = 0, .rear = 0};
uint8_t SavedPoint[SIZE] = {0};
float Voltage = 0.0f;
float Current = 0.0f;
float Power = 0.0f;

#define SFUD_DEMO_TEST_BUFFER_SIZE                     1024
static uint8_t sfud_demo_test_buf[SFUD_DEMO_TEST_BUFFER_SIZE];
static void sfud_demo(uint32_t addr, size_t size, uint8_t *data);
/*
 * key_state bit:
 *
 * bit[15]: Key event completion flag
 * bit[14]: Key interrupt trigger flag bit
 * bit[13]: Long press event triggering requires 
 * starting 2nd timer flag
 * bit[12~0]: Key count=1:single,=2:double,=3:long
 */
__IO uint16_t key_state = 0;

//uint8_t spi1_tx_buffer[1];
/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/**
 * SFUD demo for the first flash device test.
 *
 * @param addr flash start address
 * @param size test flash size
 * @param size test flash data buffer
 */
static void sfud_demo(uint32_t addr, size_t size, uint8_t *data) {
    sfud_err result = SFUD_SUCCESS;
    const sfud_flash *flash = sfud_get_device_table() + 0;
    size_t i;
    /* prepare write data */
    for (i = 0; i < size; i++) {
        data[i] = i;
    }
    /* erase test */
    result = sfud_erase(flash, addr, size);
     if (result == SFUD_SUCCESS) {
        SEGGER_RTT_printf(0, "Erase the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", flash->name, addr,
                size);
    } else {
        SEGGER_RTT_printf(0, "Erase the %s flash data failed.\r\n", flash->name);
        return;
    }
    /* write test */
    result = sfud_write(flash, addr, size, data);
    if (result == SFUD_SUCCESS) {
        SEGGER_RTT_printf(0, "Write the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", flash->name, addr,
                size);
    } else {
        SEGGER_RTT_printf(0, "Write the %s flash data failed.\r\n", flash->name);
        return;
    }
    /* read test */
    result = sfud_read(flash, addr, size, data);
    if (result == SFUD_SUCCESS) {
        SEGGER_RTT_printf(0, "Read the %s flash data success. Start from 0x%08X, size is %ld. The data is:\r\n", flash->name, addr,
                size);
        SEGGER_RTT_printf(0, "Offset (h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n");
        for (i = 0; i < size; i++) {
            if (i % 16 == 0) {
                SEGGER_RTT_printf(0, "[%08X] ", addr + i);
            }
            SEGGER_RTT_printf(0, "%02X ", data[i]);
            if (((i + 1) % 16 == 0) || i == size - 1) {
                SEGGER_RTT_printf(0,"\r\n");
            }
        }
        SEGGER_RTT_printf(0,"\r\n");
    } else {
        SEGGER_RTT_printf(0,"Read the %s flash data failed.\r\n", flash->name);
    }
    /* data check */
    for (i = 0; i < size; i++) {
        if (data[i] != i % 256) {
            SEGGER_RTT_printf(0,"Read and check write data has an error. Write the %s flash data failed.\r\n", flash->name);
			break;
        }
    }
    if (i == size) {
        SEGGER_RTT_printf(0,"The %s flash test is success.\r\n", flash->name);
    }
}

void ADC1_ReadVCC(void)
{
	vcc = ((float)1.2 * 4095) / adc1_ordinary_valuetab[1];
	//sprintf(Calc, "ADC VCC = %.2f", vcc);
	//SEGGER_RTT_printf(0, "%s\r\n", Calc);
}

void ADC1_Readtemp(void)
{
	float tmp = 0.0f;
	tmp = (ADC_TEMP_BASE - (float)adc1_ordinary_valuetab[0] * vcc / 4096) / ADC_TEMP_SLOPE + 25;
	if(tmp > 0) temp = tmp;
	//sprintf(Calc, "ADC Temp = %.2f", temp);
	//SEGGER_RTT_printf(0, "%s\r\n", Calc);
}
/* add user code end 0 */

void LCD_ChartPrint(char flag, char unit, struct Queue* queue)
{
	LCD_DrawLine(0, 14, SIZE, 14, GBLUE);
	LCD_DrawLine(0, 46, SIZE, 46, GBLUE);
	LCD_DrawLine(0, 78, SIZE, 78, GBLUE);
	sprintf(Calc, "%.1fV %.2fA %.2fW %.1fC   ", Voltage, Current, Power, temp);
	LCD_ShowString(1, 1, Calc, GBLUE, BLACK, 12, 0);
	LCD_ShowChar(150, 1, flag, GBLUE, BLACK, 12, 0);
	sprintf(Calc, "0.0%c", unit);
	LCD_ShowString(SIZE+2, 70, Calc, GBLUE, BLACK, 12, 0);
	if((queue->max)/2 > 10) 
	{
		sprintf(Calc, "%.0f", (queue->max)/2);
		LCD_ShowString(SIZE+2, 40, Calc, GBLUE, BLACK, 12, 0);
	}
	else 
	{
		sprintf(Calc, "%.2f", (queue->max)/2);
		LCD_ShowString(SIZE+2, 40, Calc, GBLUE, BLACK, 12, 0);
	}
	if(queue->max > 10) 
	{
		sprintf(Calc, "%.0f", queue->max);
		LCD_ShowString(SIZE+2, 13, Calc, GBLUE, BLACK, 12, 0);
	}
	else 
	{
		sprintf(Calc, "%.2f", queue->max);
		LCD_ShowString(SIZE+2, 13, Calc, GBLUE, BLACK, 12, 0);
	}
	ClearPrint();
	printQueue(queue);
}

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
  //wk_spi2_init();
	
  /* init adc1 function. */
  wk_adc1_init();

  /* init exint function. */
  wk_exint_config();

  /* init gpio function. */
  wk_gpio_config();
//	while(W25Q_ReadReg(W25X_ManufactDeviceID) != W25Q32)
//	{
//		SEGGER_RTT_printf(0, "W25Q32 Check Failed!\r\n");
//	}
//  W25Q_Write((uint8_t*)TEXT_Buffer, 0x1000, 64);
//  W25Q_EraseChip();
//	W25Q_Read(datatemp, 0x0, 64);
//	W25Q_Read(datatemp, 0x40, 64);
//	W25Q_Read(datatemp, 0x80, 64);
	
  /* init tmr3 function. */
  wk_tmr3_init();
	TMR3_Stop();
	
  /* init tmr15 function. */
  wk_tmr15_init();
	
  /* init tmr16 function. */
  wk_tmr16_init();
	
  /* add user code begin 2 */
	INA226_Init();
	LCD_Init();	
	LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
// 	volatile crm_clocks_freq_type crm_clk_freq = {};
//	crm_clocks_freq_get((crm_clocks_freq_type*)&crm_clk_freq);
  /* add user code end 2 */
	//int a, i=0, count=0;
	
	if (sfud_init() == SFUD_SUCCESS) {
		sfud_demo(0, sizeof(sfud_demo_test_buf), sfud_demo_test_buf);
	}
	
  while(1)
  {
		if(key_state & 0x4000)
		{
			key_state &= ~0x4000;
			//delay_ms(60);
			if((key_state & 0x1FFF) == 0) TMR3_Start(30000);
			key_state++;
		}
		else if(key_state & 0x8000)
		{
			switch(key_state)
			{
				case 0x8001: 
					SEGGER_RTT_printf(0, "single\r\n");
					if(Status < 4) Status++;
					else Status = 0;
					TMR16_Main_Flag++;
					LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
				break;
				case 0x8002:  
					SEGGER_RTT_printf(0, "double\r\n");
					if(USE_HORIZONTAL == 2) USE_HORIZONTAL = 3;
					else if (USE_HORIZONTAL == 3) USE_HORIZONTAL = 2;
					LCD_Init();
					LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
					TMR16_Main_Flag++;
				break;
				case 0x8003: 
					SEGGER_RTT_printf(0, "long\r\n");
				break;
			}
			key_state = 0;
		}
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
		
		if(TMR16_Main_Flag)
		{
			TMR16_Main_Flag = 0;
			ADC1_Readtemp();
			ADC1_ReadVCC();
			adc_ordinary_software_trigger_enable(ADC1, TRUE);
			
			Voltage = INA226_Read_Voltage();
			Current = INA226_Read_Current();
			Power = Voltage * Current;
			enqueue(&Voltage_queue, Voltage);
			enqueue(&Current_queue, Current);
			enqueue(&Power_queue, Power);
			mAh += 0.5 * Current / 3.6;
			mWh += 0.5 * Power / 3.6;
			
			switch(Status)
			{
				case 0: 
					LCD_DrawLine(88, 2, 88, 78, WHITE);
					LCD_DrawLine(89, 2, 89, 78, WHITE);
					if(Voltage < 10) sprintf(Calc, "%.3fV", Voltage);
					else sprintf(Calc, "%.2fV", Voltage);
					LCD_ShowString2416(0, 2, Calc, LIGHTBLUE, BLACK);
					//SEGGER_RTT_printf(0, "%s\r\n", Calc);
					
					sprintf(Calc, "%.3fA", Current);
					LCD_ShowString2416(0, 29, Calc, BLUE, BLACK);
					//SEGGER_RTT_printf(0, "%s\r\n", Calc);
					
					if(Power < 10) sprintf(Calc, "%.3fW", Power);
					else if(Power < 100) sprintf(Calc, "%.2fW", Power);
					else sprintf(Calc, "%.1fW", Power);
					LCD_ShowString2416(0, 56, Calc, GBLUE, BLACK);
					//SEGGER_RTT_printf(0, "%s\r\n", Calc);
					
					sprintf(Calc, "MCU:%.1fC", temp);
					LCD_ShowString(96, 2, Calc, GBLUE, BLACK, 12, 0);
					sprintf(Calc, "Vcc:%.2fV", vcc);
					LCD_ShowString(96, 18, Calc, GBLUE, BLACK, 12, 0);
					sprintf(Calc, "%.2fmAh", mAh);
					LCD_ShowString(96, 34, Calc, GBLUE, BLACK, 12, 0);
					if(mWh < 10000) sprintf(Calc, "%.2fmWh", mWh);
					else sprintf(Calc, "%.1fmWh", mWh);
					LCD_ShowString(96, 50, Calc, GBLUE, BLACK, 12, 0);
					if(USE_HORIZONTAL == 3) 
						LCD_ShowString(96, 62, "<------", GBLUE, BLACK, 16, 0);
					else 
						LCD_ShowString(96, 62, "------>", GBLUE, BLACK, 16, 0);
				break;
				
				case 1:
					LCD_ChartPrint('V', 'V', &Voltage_queue);
				break;
				
				case 2:
					LCD_ChartPrint('A', 'A', &Current_queue);
				break;
				
				case 3:
					LCD_ChartPrint('P', 'W', &Power_queue);
				break;
				
				case 4:
					LCD_DrawLine(0, 14, 160, 14, GBLUE);
					sprintf(Calc, "%.1fV %.3fA %.1fW %.1fC   ", Voltage, Current, Power, temp);
					LCD_ShowString(1, 1, Calc, GBLUE, BLACK, 12, 0);
					sprintf(Calc, "Max  Avg  Min");
					LCD_ShowString(18, 14, Calc, GBLUE, BLACK, 16, 1);
					sprintf(Calc, "%c %.2f %.2f %.2f", 'V', Voltage_queue.max, Voltage_queue.avg, Voltage_queue.min);
					LCD_ShowString(1, 30, Calc, GBLUE, BLACK, 16, 0);
					sprintf(Calc, "%c %.2f %.2f %.2f", 'A', Current_queue.max, Current_queue.avg, Current_queue.min);
					LCD_ShowString(1, 46, Calc, GBLUE, BLACK, 16, 0);
					sprintf(Calc, "%c %.2f %.2f %.2f", 'W', Power_queue.max, Power_queue.avg, Power_queue.min);
					LCD_ShowString(1, 62, Calc, GBLUE, BLACK, 16, 0);

				break;
			}
			//delay_ms(500);
		}
  }
}
