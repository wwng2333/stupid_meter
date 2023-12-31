#ifndef __I2C_H__
#define __I2C_H__

#define INA226 0x80

#include "at32f421_wk_config.h"

#define I2C_SCL_Set() gpio_bits_set(GPIOF, GPIO_PINS_1) //PF1
#define I2C_SCL_Clr() gpio_bits_reset(GPIOF, GPIO_PINS_1)

#define I2C_SDA_Set() gpio_bits_set(GPIOF, GPIO_PINS_0) //PF0
#define I2C_SDA_Clr() gpio_bits_reset(GPIOF, GPIO_PINS_0)

void INA226_Init(void);
float INA226_Read_Voltage(void);
float INA226_Read_Current(void);
float INA226_Read_Power(void);

uint16_t I2C_Read_2Byte(uint8_t addr);
void I2C_Write_2Byte(uint8_t addr, uint16_t dat);
uint8_t I2C_ReadByte(uint8_t addr);
void I2C_WriteByte(uint8_t addr, uint8_t dat);
uint16_t I2C_Read_12Bit(uint8_t addr);
uint16_t I2C_Read_13Bit(uint8_t addr);
uint32_t I2C_Read_32Bit(uint8_t addr);

void I2C_Start();
void I2C_SendData(uint8_t dat);
void I2C_RecvACK();
void I2C_SendACK();
uint8_t I2C_RecvData();
void I2C_SendNAK();
void I2C_Stop();

#endif