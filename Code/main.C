#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

#include "lcd_init.h"
#include "lcd.h"
#include "I2C.h"

void main (void) 
{
	uint16_t id;
	char Voltage[8] = 0;
	char Power[8] = 0;
	char Current[16] = 0;
	char str[8] = 0;
	float V = 0.0f;
	float A = 0.0f;
	float P = 0.0f;
	
	LCD_GPIO_Init();
	I2C_Init();
	
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	//LCD_ShowString(1,1,"hello world",WHITE,BLACK,24,0);
	
	I2C_Write_2Byte(0x00, 0x4527);
	I2C_Write_2Byte(0x05, 0x1400); //5120, 0.1mA
	
	while(1)
	{
		//LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
		V = 1.25 * (float)I2C_Read_2Byte(0x02) / 1000; //Voltage
		if(V > 10.0) sprintf(Voltage, "%.2fV", V);
		else sprintf(Voltage, "%.3fV", V);
		LCD_ShowString(0,-4,Voltage,BLUE,BLACK,32, 0);
		
		id = I2C_Read_2Byte(0x04);
		A = ((float)id) / 10000; //Current
		sprintf(Current, "%.3fA", A);
		LCD_ShowString(0,21,Current,GREEN,BLACK,32, 0);

		P = V*A; //Power
		if(P > 10.0) sprintf(Power, "%.2fW", P);
		else if(P > 100.0) sprintf(Power, "%.1fW", P);
		else sprintf(Power, "%.3fW", P);
		LCD_ShowString(0,48, Power,GBLUE,BLACK,32, 0);
	}
}

