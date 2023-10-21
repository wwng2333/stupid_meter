#include "at32f421_wk_config.h"
#include "w25qxx.h"

uint8_t W25Q_Buffer[4096] = {0};

void W25Q_Write(uint8_t *pBuffer, uint32_t addr, uint16_t size)
{
	uint32_t sec_pos;
	uint16_t sec_offset, sec_remain, i;
	uint8_t *RamBuffer;
	RamBuffer = W25Q_Buffer;
	sec_pos = addr / 4096;
	sec_offset = addr % 4096;
	sec_remain = 4096 - sec_offset;
	SEGGER_RTT_printf(0, "Write 1st addr: 0x%x, size: %d\r\n", addr, size);
	if (size < sec_remain)
		sec_remain = size;
	while (1)
	{
		W25Q_Read(RamBuffer, sec_pos * 4096, 4096);
		for (i = 0; i < sec_remain; i++)
		{
			if (RamBuffer[sec_offset + i] != 0xFF)
				break; // need erase
		}
		if (i < sec_remain) // need erase
		{
			W25Q_EraseSector(sec_pos);
			for (i = 0; i < sec_remain; i++) // copy
			{
				RamBuffer[i] = pBuffer[i];
			}
			W25Q_NoCheckWrite(RamBuffer, sec_pos * 4096, 4096); // full sector write
		}
		else
		{
			W25Q_NoCheckWrite(pBuffer, addr, sec_remain);
		}
		if (size == sec_remain) // write done.
		{
			break;
		}
		else // still need write
		{
			sec_pos++;
			sec_offset = 0;
			pBuffer += sec_remain;
			addr += sec_remain;
			size -= sec_remain;
			if (size > 4096)
			{
				sec_remain = 4096; // need more sectors
			}
			else
			{
				sec_remain = size; // last sector finish
			}
		}
	}
	SEGGER_RTT_printf(0, "W25Q32 write finish!\r\n");
}

void W25Q_NoCheckWrite(uint8_t *pBuffer, uint32_t addr, uint16_t size)
{
	uint16_t page_remain;
	page_remain = 256 - addr % 256;
	if (size <= page_remain)
		page_remain = size;
	while (1)
	{
		W25Q_WritePage(pBuffer, addr, size);
		if (page_remain == size)
		{
			break;
		}
		else
		{
			pBuffer += page_remain;
			addr += page_remain;
			size -= page_remain;
			if (size > 256)
			{
				page_remain = 256;
			}
			else
			{
				page_remain = size;
			}
		}
	}
}

void W25Q_WritePage(uint8_t *pBuffer, uint32_t addr, uint16_t size)
{
	SEGGER_RTT_printf(0, "WritePage addr: 0x%x\r\n", addr);
	uint16_t i;
	W25Q_EnableWrite();
	W25Q_CS_Clr();
	W25Q_ReadWriteByte(W25X_PageProgram);
	W25Q_ReadWriteByte((uint8_t)(addr >> 16));
	W25Q_ReadWriteByte((uint8_t)(addr >> 8));
	W25Q_ReadWriteByte((uint8_t)addr);
	for (i = 0; i < size; i++)
	{
		W25Q_ReadWriteByte(pBuffer[i]);
	}
	W25Q_CS_Set();
	W25Q_Wait();
}

void W25Q_EraseSector(uint32_t addr)
{
	SEGGER_RTT_printf(0, "EraseSector addr: 0x%x\r\n", addr);
	addr *= 4096;
	W25Q_EnableWrite();
	W25Q_Wait();
	W25Q_CS_Clr();
	W25Q_ReadWriteByte(W25X_SectorErase);
	W25Q_ReadWriteByte((uint8_t)(addr >> 16));
	W25Q_ReadWriteByte((uint8_t)(addr >> 8));
	W25Q_ReadWriteByte((uint8_t)addr);
	W25Q_CS_Set();
	W25Q_Wait();
}

void W25Q_EraseChip(void)
{
	W25Q_EnableWrite();
	W25Q_Wait();
	W25Q_CS_Clr();
	W25Q_ReadWriteByte(W25X_ChipErase);
	W25Q_CS_Set();
	W25Q_Wait();
}

void W25Q_EnableWrite(void)
{
	W25Q_CS_Clr();
	W25Q_ReadWriteByte(W25X_WriteEnable);
	W25Q_CS_Set();
}

void W25Q_DisableWrite(void)
{
	W25Q_CS_Clr();
	W25Q_ReadWriteByte(W25X_WriteDisable);
	W25Q_CS_Set();
}

void W25Q_Wait(void)
{
	while ((W25Q_ReadReg(W25X_ReadStatusReg) & 0x01) == 0x01)
		;
}

uint8_t W25Q_ReadWriteByte(uint8_t dat)
{
	while (spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET)
		;
	spi_i2s_data_transmit(SPI2, dat);

	while (spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET)
		;
	return spi_i2s_data_receive(SPI2);
}

uint16_t W25Q_ReadReg(uint8_t addr)
{
	uint16_t temp = 0;
	W25Q_CS_Clr();
	W25Q_ReadWriteByte(addr);
	W25Q_ReadWriteByte(0x00);
	W25Q_ReadWriteByte(0x00);
	W25Q_ReadWriteByte(0x00);

	temp = W25Q_ReadWriteByte(0xFF) << 8;
	temp |= W25Q_ReadWriteByte(0xFF);
	// SEGGER_RTT_SetTerminal(1);
	// SEGGER_RTT_printf(0, "SPI2 25qxx reg 0x%02x=0x%04x\r\n", addr, temp);
	// SEGGER_RTT_SetTerminal(0);
	W25Q_CS_Set();
	return temp;
}

void W25Q_Read(uint8_t *pBuffer, uint32_t addr, uint16_t size)
{
	uint16_t i;
	W25Q_CS_Clr();
	W25Q_ReadWriteByte(W25X_ReadData);
	W25Q_ReadWriteByte((uint8_t)(addr >> 16));
	W25Q_ReadWriteByte((uint8_t)(addr >> 8));
	W25Q_ReadWriteByte((uint8_t)addr);
	for (i = 0; i < size; i++)
	{
		pBuffer[i] = W25Q_ReadWriteByte(0xFF);
	}
	W25Q_CS_Set();
}