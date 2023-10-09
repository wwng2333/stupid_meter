#include "N76E003.h"
#include "Common.h"
#include "Function_define.h"
#include "Delay.h"
#include "I2C.h"

void I2C_Init()
{
	P13_OpenDrain_Mode;
	P14_OpenDrain_Mode;
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

	Timer3_Delay10us(1);

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
	SDA = 1;
	SCL = 1;
	Timer3_Delay10us(1);
	SDA = 0;
	Timer3_Delay10us(1);
	SCL = 0;
}

void I2C_SendData(uint8_t dat)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		SCL = 0;
		Timer3_Delay10us(1);
		if (dat & 0x80)
			SDA = 1;
		else
			SDA = 0;
		dat <<= 1;
		SCL = 1;
		Timer3_Delay10us(1);
	}
	SCL = 0;
}

void I2C_SendACK()
{
	SCL = 0;
	Timer3_Delay10us(1);
	SDA = 0;
	SCL = 1;
	Timer3_Delay10us(1);
	SCL = 0;
	SDA = 1;
	Timer3_Delay10us(1);
}

void I2C_RecvACK()
{
	SCL = 0;
	Timer3_Delay10us(1);
	SDA = 1;
	SCL = 1;
	Timer3_Delay10us(1);
	SCL = 0;
	Timer3_Delay10us(1);
}

uint8_t I2C_RecvData()
{
	uint8_t dat, i;
	for (i = 0; i < 8; i++)
	{
		dat <<= 1;
		SCL = 0;
		Timer3_Delay10us(1);
		SCL = 1;
		Timer3_Delay10us(1);
		if (SDA)
			dat += 1;
	}
	return dat;
}

void I2C_SendNAK()
{
	SCL = 0;
	Timer3_Delay10us(1);
	SDA = 1;
	SCL = 1;
	Timer3_Delay10us(1);
	SCL = 0;
	Timer3_Delay10us(1);
}

void I2C_Stop()
{
	SDA = 0;
	SCL = 1;
	Timer3_Delay10us(1);
	SDA = 1;
	Timer3_Delay10us(1);
}
