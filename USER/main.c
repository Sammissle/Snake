#include "sys.h"  //系统头文件
#include "delay.h"
#include "usart.h" //串口通信
#include "led.h"
#include "lcd.h"
#include "beep.h"    
#include "key.h"    //按键
#include "remote.h" //红外遥控
#include "snake.h"

int main(void)
{ 
  u8 key=0;
	u8 j=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);      
	uart_init(115200);
	LED_Init();
	LCD_Init();
	KEY_Init();
	Remote_Init();
	Display_Begin();
	BEEP_Init();
	while(1)       //循环中通过按键扫描判断是否进行游戏
	{
		j=Remote_Scan();
		POINT_COLOR=RED;
		LCD_ShowString(10,230,100,16,16,"3200708011");
		key=KEY_Scan(0);
		if(key==1||key==2||key==3||key==4||j==2)
		{
		  Play_Snake();		
		}
	}
}
