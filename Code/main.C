#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "math.h"

#include "lcd.h"
#include "I2C.h"    

UINT8 u8TH0_Tmp,u8TL0_Tmp;
uint16_t T0_Calc;

void Timer0_ISR (void) interrupt 1  //interrupt address is 0x000B
{
    TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;    
    T0_Calc++;
    //P16 = ~P16;                     // GPIO1 toggle when interrupt
}

void Timer0_Init(void)
{
	TIMER0_MODE1_ENABLE;
	clr_T0M;
	u8TH0_Tmp = TIMER_DIV12_VALUE_40ms >>8;
	u8TL0_Tmp = TIMER_DIV12_VALUE_40ms;
	TH0 = u8TH0_Tmp;
	TL0 = u8TL0_Tmp;
	set_ET0;
	set_EA;
	set_TR0;
	T0_Calc = 0;
}

void main(void)
{
	//uint16_t RGB = 0;
	float mAh = 0;
	float mWh = 0;
	uint32_t V_fixed = 0;
	uint32_t A_fixed = 0;
	uint32_t P_fixed = 0;
	char Voltage[8] = {0};
	char Power[8] = {0};
	char Current[8] = {0};
	char Calc[12] = {0};

	LCD_GPIO_Init();
	LCD_Init();
	I2C_Init();
	P16_PushPull_Mode; // Debug
	P16 = 0;
	Timer0_Init();

	LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);	
	LCD_DrawLine(88, 2, 88, 78, WHITE);
	LCD_DrawLine(89, 2, 89, 78, WHITE);

	I2C_Write_2Byte(0x00, 0x45FF);
	I2C_Write_2Byte(0x05, 0x0A00); // 2560, 0.2mA
	while (1)
	{
		V_fixed = 1.25 * (uint32_t)I2C_Read_2Byte(0x02);
		if (V_fixed < 10000) {
				sprintf(Voltage, "%d.%03dV", (uint16_t)(V_fixed/1000), (uint16_t)(V_fixed%1000));
		} else {
				sprintf(Voltage, "%d.%02dV", (uint16_t)(V_fixed/1000), (uint16_t)(V_fixed%1000));
		LCD_ShowString2416(0, 2, Voltage, LIGHTBLUE, BLACK);
		}
		
		A_fixed = (uint32_t)I2C_Read_2Byte(0x04) / 50;
		if (A_fixed < 1000) {
			sprintf(Current, "%d.%03dA", (uint16_t)(A_fixed/1000), (uint16_t)(A_fixed%1000));
		} else {
			sprintf(Current, "%d.%02dA", (uint16_t)(A_fixed/1000), (uint16_t)(A_fixed%1000));
		}
		LCD_ShowString2416(0, 29, Current, BLUE, BLACK);

		P_fixed = (uint32_t)(V_fixed * A_fixed / 1000); // Power
		if (P_fixed < 10000) {
			sprintf(Power, "%d.%03dW", (uint16_t)(P_fixed/1000), (uint16_t)(P_fixed%1000));
		}	else if (P_fixed > 10000) {
			sprintf(Power, "%d.%02dW", (uint16_t)(P_fixed/1000), (uint16_t)(P_fixed%1000));
		} else {
			sprintf(Power, "%d.%01dW", (uint16_t)(P_fixed/1000), (uint16_t)(P_fixed%1000));
		}
		//sprintf(Power, "%lu", P_fixed);
		LCD_ShowString2416(0, 56, Power, GBLUE, BLACK);
		
		sprintf(Calc, "T0:%d", T0_Calc);
		LCD_ShowString(92, 0, Calc, GBLUE, BLACK, 12, 0);
		mAh += (float)T0_Calc * 40 / 1000 * (float)A_fixed / 3600;
		mWh += (float)T0_Calc * 40 / 1000 * (float)P_fixed / 3600;
		T0_Calc = 0;
		sprintf(Calc, "%.2fmAh", mAh);
		LCD_ShowString(92, 16, Calc, GBLUE, BLACK, 12, 0);
		sprintf(Calc, "%.2fmWh", mWh);
		LCD_ShowString(92, 32, Calc, GBLUE, BLACK, 12, 0);
		//sprintf(Calc, "%d", RGB);
		//LCD_ShowString(92, 48, Calc, GBLUE, BLACK, 12, 0);
		//RGB += 0x01;
		P16 = ~P16;
	}
}
