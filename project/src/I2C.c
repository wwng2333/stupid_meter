#include "at32f421_wk_config.h"
#include "I2C.h"

float INA226_Read_Voltage(void)
{
	return 1.25 * (float)I2C_Read_2Byte(0x02) / 1000;
}

float INA226_Read_Current(void)
{
	return (float)I2C_Read_2Byte(0x04) / 50000;
}

float INA226_Read_Power(void)
{
	return (float)I2C_Read_2Byte(0x03) * 0.02 * 25;
}

/**
  * @brief Read INA226 ID, Set Configuration 
	* Register and Calibration Register
  * @param  none
  * @retval none
  */
void INA226_Init(void)
{
	uint16_t id = 0;
	do {
		id = I2C_Read_2Byte(0xFE);
		SEGGER_RTT_printf(0, "INA226 0xFE=0x%x\r\n", id);
		delay_ms(100);
	} while(id != 0x5449);
	I2C_Write_2Byte(0x00, 0x45FF); // Configuration Register
	I2C_Write_2Byte(0x05, 0x0A00); // Calibration Register, 2560, 0.2mA
}

uint16_t I2C_Read_2Byte(uint8_t addr)
{
	uint16_t dat = 0;
	uint8_t dat1, dat2;
	I2C_Start();
	I2C_SendData(INA226);
	I2C_RecvACK();
	I2C_SendData(addr);
	I2C_RecvACK();
	I2C_Start();
	I2C_SendData(INA226 + 1);
	I2C_RecvACK();
	dat1 = I2C_RecvData();
	I2C_SendACK();
	dat2 = I2C_RecvData();
	I2C_SendNAK();
	I2C_Stop();

	dat = (dat1 << 8) + dat2;
	return dat;
}

uint8_t I2C_ReadByte(uint8_t addr)
{
	uint8_t dat = 0;
	I2C_Start();
	I2C_SendData(INA226);
	I2C_RecvACK();
	I2C_SendData(addr);
	I2C_RecvACK();

	delay_us(10);

	I2C_Start();
	I2C_SendData(INA226 + 1);
	I2C_RecvACK();
	dat = I2C_RecvData();
	I2C_SendNAK();
	I2C_Stop();
	return dat;
}

void I2C_Write_2Byte(uint8_t addr, uint16_t dat)
{
	I2C_Start();
	I2C_SendData(INA226);
	I2C_RecvACK();
	I2C_SendData(addr);
	I2C_RecvACK();
	I2C_SendData(dat >> 8);
	I2C_RecvACK();
	I2C_SendData(dat);
	I2C_RecvACK();
	I2C_Stop();
}

void I2C_WriteByte(uint8_t addr, uint8_t dat)
{
	I2C_Start();
	I2C_SendData(INA226);
	I2C_RecvACK();
	I2C_SendData(addr);
	I2C_RecvACK();
	I2C_SendData(dat);
	I2C_RecvACK();
	I2C_Stop();
}

void I2C_Start()
{
	I2C_SDA_Set();
	I2C_SCL_Set();
	delay_us(10);
	I2C_SDA_Clr();
	delay_us(10);
	I2C_SCL_Clr();
}

void I2C_SendData(uint8_t dat)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		I2C_SCL_Clr();
		delay_us(10);
		if (dat & 0x80)
			I2C_SDA_Set();
		else
			I2C_SDA_Clr();
		dat <<= 1;
		I2C_SCL_Set();
		delay_us(10);
	}
	I2C_SCL_Clr();
}

void I2C_SendACK()
{
	I2C_SCL_Clr();
	delay_us(10);
	I2C_SDA_Clr();
	I2C_SCL_Set();
	delay_us(10);
	I2C_SCL_Clr();
	I2C_SDA_Set();
	delay_us(10);
}

void I2C_RecvACK()
{
	I2C_SCL_Clr();
	delay_us(10);
	I2C_SDA_Set();
	I2C_SCL_Set();
	delay_us(10);
	I2C_SCL_Clr();
	delay_us(10);
}

uint8_t I2C_RecvData()
{
	uint8_t dat, i;
	for (i = 0; i < 8; i++)
	{
		dat <<= 1;
		I2C_SCL_Clr();
		delay_us(10);
		I2C_SCL_Set();
		delay_us(10);
		if (gpio_input_data_bit_read(GPIOF, GPIO_PINS_0) == SET)
			dat += 1;
	}
	return dat;
}

void I2C_SendNAK()
{
	I2C_SCL_Clr();
	delay_us(10);
	I2C_SDA_Set();
	I2C_SCL_Set();
	delay_us(10);
	I2C_SCL_Clr();
	delay_us(10);
}

void I2C_Stop()
{
	I2C_SDA_Clr();
	I2C_SCL_Set();
	delay_us(10);
	I2C_SDA_Set();
	delay_us(10);
}