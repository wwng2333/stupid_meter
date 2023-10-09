#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "math.h"

#include "lcd.h"
#include "I2C.h"

void main (void) 
{
	char Voltage[8] = 0;
	char Power[8] = 0;
	char Current[8] = 0;
	float V = 0.0f;
	float A = 0.0f;
	float P = 0.0f;
	
	LCD_GPIO_Init();
	I2C_Init();
	P16_PushPull_Mode; //Debug
	P16=0;
	
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	//LCD_ShowString(1,1,"hello world",WHITE,BLACK,24,0);
	
	I2C_Write_2Byte(0x00, 0x4527);
	I2C_Write_2Byte(0x05, 0x1400); //5120, 0.1mA
	LCD_DrawLine(101,5,101,75,WHITE);
	LCD_DrawLine(102,5,102,75,WHITE);
	while(1)
	{
		//LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
		V = 1.25 * (float)I2C_Read_2Byte(0x02) / 1000; //Voltage
		if(V > 10.0) sprintf(Voltage, "%.2fV", V);
		else sprintf(Voltage, "%.3fV", V);
		LCD_ShowString2416(0,2,Voltage,LIGHTBLUE,BLACK);
		
		A = ((float)abs(I2C_Read_2Byte(0x04))) / 10000; //Current
		sprintf(Current, "%.3fA", A);
		LCD_ShowString2416(0,29,Current,BLUE,BLACK);

		P = V*A; //Power
		if(P > 10.0) sprintf(Power, "%.2fW", P);
		else if(P > 100.0) sprintf(Power, "%.1fW", P);
		else sprintf(Power, "%.3fW", P);
		LCD_ShowString2416(0,56, Power,DARKBLUE,BLACK);
		P16 = ~P16;
	}
}

