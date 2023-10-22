/* add user code begin Header */
/**
  **************************************************************************
  * @file     at32f421_int.c
  * @brief    main interrupt service routines.
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

/* includes ------------------------------------------------------------------*/
#include "at32f421_int.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "at32f421_wk_config.h"
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
extern __IO uint16_t key_state;
extern __IO uint8_t TMR16_Main_Flag;
/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/* external variables ---------------------------------------------------------*/
/* add user code begin external variables */
/* add user code end external variables */

/**
  * @brief  this function handles nmi exception.
  * @param  none
  * @retval none
  */
void NMI_Handler(void)
{
  /* add user code begin NonMaskableInt_IRQ 0 */

  /* add user code end NonMaskableInt_IRQ 0 */

  /* add user code begin NonMaskableInt_IRQ 1 */

  /* add user code end NonMaskableInt_IRQ 1 */
}

/**
  * @brief  this function handles hard fault exception.
  * @param  none
  * @retval none
  */
void HardFault_Handler(void)
{
  /* add user code begin HardFault_IRQ 0 */

  /* add user code end HardFault_IRQ 0 */
  /* go to infinite loop when hard fault exception occurs */
  while (1)
  {
    /* add user code begin W1_HardFault_IRQ 0 */

    /* add user code end W1_HardFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles memory manage exception.
  * @param  none
  * @retval none
  */
void MemManage_Handler(void)
{
  /* add user code begin MemoryManagement_IRQ 0 */

  /* add user code end MemoryManagement_IRQ 0 */
  /* go to infinite loop when memory manage exception occurs */
  while (1)
  {
    /* add user code begin W1_MemoryManagement_IRQ 0 */

    /* add user code end W1_MemoryManagement_IRQ 0 */
  }
}

/**
  * @brief  this function handles bus fault exception.
  * @param  none
  * @retval none
  */
void BusFault_Handler(void)
{
  /* add user code begin BusFault_IRQ 0 */

  /* add user code end BusFault_IRQ 0 */
  /* go to infinite loop when bus fault exception occurs */
  while (1)
  {
    /* add user code begin W1_BusFault_IRQ 0 */

    /* add user code end W1_BusFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles usage fault exception.
  * @param  none
  * @retval none
  */
void UsageFault_Handler(void)
{
  /* add user code begin UsageFault_IRQ 0 */

  /* add user code end UsageFault_IRQ 0 */
  /* go to infinite loop when usage fault exception occurs */
  while (1)
  {
    /* add user code begin W1_UsageFault_IRQ 0 */

    /* add user code end W1_UsageFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles svcall exception.
  * @param  none
  * @retval none
  */
void SVC_Handler(void)
{
  /* add user code begin SVCall_IRQ 0 */

  /* add user code end SVCall_IRQ 0 */
  /* add user code begin SVCall_IRQ 1 */

  /* add user code end SVCall_IRQ 1 */
}

/**
  * @brief  this function handles debug monitor exception.
  * @param  none
  * @retval none
  */
void DebugMon_Handler(void)
{
  /* add user code begin DebugMonitor_IRQ 0 */

  /* add user code end DebugMonitor_IRQ 0 */
  /* add user code begin DebugMonitor_IRQ 1 */

  /* add user code end DebugMonitor_IRQ 1 */
}

/**
  * @brief  this function handles pendsv_handler exception.
  * @param  none
  * @retval none
  */
void PendSV_Handler(void)
{
  /* add user code begin PendSV_IRQ 0 */

  /* add user code end PendSV_IRQ 0 */
  /* add user code begin PendSV_IRQ 1 */

  /* add user code end PendSV_IRQ 1 */
}

/**
  * @brief  this function handles EXINT Line 1 & 0 handler.
  * @param  none
  * @retval none
  */
void EXINT1_0_IRQHandler(void)
{
	SEGGER_RTT_printf(0, "EXINT1 IRQ\r\n");
  /* add user code begin EXINT1_0_IRQ 0 */
	if((key_state & 0x8000) == 0)
	{
		key_state |= 0x4000;
	}
	exint_flag_clear(EXINT_LINE_0);
  /* add user code end EXINT1_0_IRQ 0 */
  /* add user code begin EXINT1_0_IRQ 1 */

  /* add user code end EXINT1_0_IRQ 1 */
}

/**
  * @brief  this function handles DMA1 Channel 3 & 2 handler.
  * @param  none
  * @retval none
  */
void DMA1_Channel3_2_IRQHandler(void)
{
  /* add user code begin DMA1_Channel3_2_IRQ 0 */
	if(dma_flag_get(DMA1_FDT3_FLAG) == SET)
	{
		dma_channel_enable(DMA1_CHANNEL3, FALSE);
		while (spi_i2s_flag_get(SPI1, SPI_I2S_TDBE_FLAG) == RESET || 
			spi_i2s_flag_get(SPI1, SPI_I2S_BF_FLAG) == SET);
		LCD_CS_Set();
		dma_flag_clear(DMA1_FDT3_FLAG);
	}
	
  /* add user code end DMA1_Channel3_2_IRQ 0 */
  /* add user code begin DMA1_Channel3_2_IRQ 1 */

  /* add user code end DMA1_Channel3_2_IRQ 1 */
}

/**
  * @brief  this function handles TMR3 handler.
  * @param  none
  * @retval none
  */
void TMR3_GLOBAL_IRQHandler(void)
{
	SEGGER_RTT_printf(0, "TMR3 IRQ\r\n");
  /* add user code begin TMR3_GLOBAL_IRQ 0 */
	TMR3_Stop();
	if(key_state & 0x0200) //700ms delay
	{
		if(gpio_input_data_bit_read(GPIOB, GPIO_PINS_0) == RESET)
		{
			key_state = 0x8003;
		}
		else
		{
			key_state = 0x8000;
		}
	}
	else //300ms delay
	{
		if(gpio_input_data_bit_read(GPIOB, GPIO_PINS_0) == RESET)
		{
			key_state |= 0x2000; //700ms delay flag
			TMR3_Start(70000); //Start 700ms timer
		}
		else
		{
			if((key_state & 0x1FFF) == 1) key_state = 0x8001;
			else if((key_state & 0x1FFF) == 2) key_state = 0x8002;
			//else if((key_state & 0x1FFF) > 2) key_state = 0x8003;
			else key_state = 0;
		}
	}
  /* add user code end TMR3_GLOBAL_IRQ 0 */
  /* add user code begin TMR3_GLOBAL_IRQ 1 */

  /* add user code end TMR3_GLOBAL_IRQ 1 */
}

/**
  * @brief  this function handles TMR16 handler.
  * @param  none
  * @retval none
  */
void TMR16_GLOBAL_IRQHandler(void)
{
  /* add user code begin TMR16_GLOBAL_IRQ 0 */
	//SEGGER_RTT_printf(0, "TMR16 IRQ\r\n");
	tmr_flag_clear(TMR16, TMR_OVF_FLAG);
	TMR16_Main_Flag++;
  /* add user code end TMR16_GLOBAL_IRQ 0 */
  /* add user code begin TMR16_GLOBAL_IRQ 1 */

  /* add user code end TMR16_GLOBAL_IRQ 1 */
}

//extern uint8_t usart1_rx_buffer[64];
//extern volatile uint8_t usart1_rx_counter;
/**
  * @brief  this function handles usart1 handler.
  * @param  none
  * @retval none
  */
//void USART1_IRQHandler(void)
//{
//  if(USART1->ctrl1_bit.rdbfien != RESET)
//  {
//    if(usart_flag_get(USART1, USART_RDBF_FLAG) != RESET)
//    {
//      /* read one byte from the receive data register */
//      usart1_rx_buffer[usart1_rx_counter++] = usart_data_receive(USART1);

////      if(usart1_rx_counter == usart2_tx_buffer_size)
////      {
////        /* disable the usart1 receive interrupt */
////        usart_interrupt_enable(USART1, USART_RDBF_INT, FALSE);
////      }
//    }
//  }
//  
////  if(USART1->ctrl1_bit.tdbeien != RESET)
////  {
////    if(usart_flag_get(USART1, USART_TDBE_FLAG) != RESET)
////    {
////      /* write one byte to the transmit data register */
////      usart_data_transmit(USART1, usart1_tx_buffer[usart1_tx_counter++]);
////    }
////  }
//}

/* add user code begin 1 */

/* add user code end 1 */
