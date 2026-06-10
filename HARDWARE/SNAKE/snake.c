#include "snake.h"
#include "delay.h"
#include "lcd.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "remote.h"
#include <stdio.h>
#include <stdlib.h>

#define Snake_Maxlong 8
u8 Key_Value;
u8 Key_Vtemp;
struct
{
	u16 X;
	u16 Y;
	u8 Yes;
}Food;

struct
{
	u16 X[Snake_Maxlong];
	u16 Y[Snake_Maxlong];
	u8 Long;
	u8 Life;
	u8 Score;
	u8 Level;
}Snake;

u8 flag = 1;

void Display_Begin(void) //开始界面
{	
	LCD_Clear(BLACK);
	POINT_COLOR=RED;
	LCD_DrawRectangle(10, 10, 200, 200);
	LCD_ShowString(30,40,200,16,16," HELLO ");
	LCD_ShowString(30,60,200,16,16," SNAKE GAME ");
	LCD_ShowString(30,90,200,16,16,"Are you ready");
}

void Display_Select(void) //难度选择
{
	LCD_Clear(BLACK);
	POINT_COLOR=RED;
	LCD_DrawRectangle(10, 10, 200, 200);
	LCD_ShowString(30,40,100,16,16,"Level listed:");
	LCD_ShowString(30,60,200,16,16,"KEY0 || 1 Simple");
	LCD_ShowString(30,80,200,16,16,"KEY1 || 2 Medium");
	LCD_ShowString(30,100,200,16,16,"KEY2 || 3 Difficult");
}

void Display_Dead(void) //结束界面
{
	LCD_Clear(BLACK);
	POINT_COLOR=RED;
	LCD_DrawRectangle(10, 10, 200, 200);
	LCD_ShowString(30,70,200,16,16," GAME OVER ");
	LCD_ShowString(30,90,210,16,16,"Press to continue");
	BEEP=1;
	delay_ms(100);
	BEEP=0;
	delay_ms(100);
	BEEP=1;
	delay_ms(100);
	BEEP=0;
}

void Display_Pass(void) //通关界面
{
	LCD_Clear(BLACK);
	POINT_COLOR=RED;
	LCD_DrawRectangle(10, 10, 200, 200);
	LCD_ShowString(30,70,200,16,16,"You are win!");
	LCD_ShowString(30,90,210,16,16,"Press to continue");
	BEEP=1;
	delay_ms(100);
	BEEP=0;
	delay_ms(100);
	BEEP=1;
	delay_ms(100);
	BEEP=0;
	delay_ms(100);
	BEEP=1;
	delay_ms(100);
	BEEP=0;
}

void Turnon_point(u8 x,u8 y) 
{
  LCD_Fill(x,y,x+4,y+4,RED);
}

void Turnoff_point(u8 x,u8 y)
{
	LCD_Fill(x,y,x+4,y+4,BLACK);
}
//控制点阵LED灯

void Draw_Wall(void) //边界
{
  switch(Snake.Level)
	{
		case 1:LCD_DrawLine(50, 80, 50, 150); break;
		case 2:LCD_DrawLine(100, 40, 100, 150); break;
		case 3:LCD_DrawLine(90, 80, 90, 170);
		       LCD_DrawLine(160, 80, 160, 170); break;
	}
}

u8 Judge_Life(u8 i) //判断游戏是否结束
{
	u8 j=0;
	u8 k=0;
	switch(i)
	{
		case 1:
		  {
			for(j=0;j<70;j++)
		  {
				for(k=0;k<Snake.Long;k++)
				{
				  if((Snake.X[Snake.Long-1]==48||Snake.X[Snake.Long-1]==52)&&Snake.Y[Snake.Long-1]==80+j)
				  return 0;break;
				}
		  }
			if(j>=70)
			  return 1;
		   }
   	   break;
		case 2:
		   {
			 for(j=0;j<110;j++)
		    {
				 for(k=0;k<Snake.Long;k++)
				 {
				   if((Snake.X[Snake.Long-1]==98||Snake.X[Snake.Long-1]==102)&&Snake.Y[Snake.Long-1]==40+j)
				    return 0; break;
				 }
		    }
			 if(j>=110)
				 return 1;
		   }
			break;
		case 3:
		  {
			 for(j=0;j<90;j++)
		   {
				for(k=0;k<Snake.Long;k++)
				{
				  if((Snake.X[Snake.Long-1]==88||Snake.X[Snake.Long-1]==92||Snake.X[Snake.Long-1]==158||Snake.X[Snake.Long-1]==162)&&Snake.Y[Snake.Long-1]==80+j)
				   return 0;break;
				}
		   }
			 if(j>=90)
			    return 1;
	     }
			 break;
		}
	return 2;
}

void Init_Snake() //初始化贪吃蛇和食物的位置
{
	u8 i = 0;
	LCD_Clear(BLACK);
	POINT_COLOR=RED;
	BACK_COLOR=BLACK;
	LCD_DrawRectangle(10, 10, 200, 200);
	Snake.Long=3;
	Snake.Life=0;
	Snake.Score=0;
	Food.Yes=0;
	for(i=0;i<Snake.Long;i++)
	{
		Snake.X[i]=14+i;
	  Snake.Y[i]=17;
	}
  for(i=0;i<Snake.Long;i++)
  {
     	Turnon_point(Snake.X[i],Snake.Y[i]);
  }
	Food.X=rand()%(28)+60;
	Food.Y=rand()%(161)+10;
	Turnon_point(Food.X,Food.Y);
}

void Run_Snake() //运行逻辑
{
	char Score[50];
	u8 i=0;
	u8 j=0;
	u8 k=0;
	Key_Vtemp=1;
	while(1)
	{
		sprintf(Score,"Score is %d",Snake.Score);
		POINT_COLOR=RED;
		LCD_ShowString(10,250,100,16,16,(u8*)Score);
		LCD_ShowString(10,270,100,16,16,"50 WIN!");
	  LCD_DrawRectangle(10, 10, 200, 200);
		Draw_Wall();
		/******食物的产生******/
	  if(Food.Yes==1)
	  {
		 while(1)
		 {
		 BEEP=1;
		 LED0=0;
		 delay_ms(100);
		 BEEP=0;
		 LED0=1;
		 Food.X=rand()%(180)+18;
		 Food.Y=rand()%(180)+10;
		 for(i=0;i<Snake.Long;i++)
		 {
			 if(Food.X==Snake.X[i]&&Food.Y==Snake.Y[i]==Food.Y)
		    break;
		 }
		 k=Judge_Life(Snake.Level);
		 if(i==Snake.Long||k==1)
		 {
			 Food.Yes=0;
			 break;
		 }
		 }
	  }
	  if(Food.Yes==0)
	  {
		  Turnon_point(Food.X,Food.Y);
	  }
		/******控制蛇的移动*****/
	  Key_Value=KEY_Scan(0);
		j=Remote_Scan();
	  if(((Key_Value==1||j==194)&&Key_Vtemp!=3)||Key_Vtemp==1)
	  {
		Turnoff_point(Snake.X[0],Snake.Y[0]);
		for(i=0;i<Snake.Long-1;i++)
		{
			Snake.X[i]=Snake.X[i+1];
			Snake.Y[i]=Snake.Y[i+1];
		}
	  Snake.X[Snake.Long-1]=Snake.X[Snake.Long-2]+1;
		Snake.Y[Snake.Long-1]=Snake.Y[Snake.Long-2]+0;
		for(i=0;i<Snake.Long;i++)
		{
			Turnon_point(Snake.X[i],Snake.Y[i]);
		}
		Key_Vtemp=1;
		
	  }
	  if(((Key_Value==3||j==34)&&Key_Vtemp!=1)||Key_Vtemp==3)
	  {
		Turnoff_point(Snake.X[0],Snake.Y[0]);
	
	  for(i=0;i<Snake.Long-1;i++)
	  {
		  Snake.X[i]=Snake.X[i+1];
		  Snake.Y[i]=Snake.Y[i+1];
	  }
	  Snake.X[Snake.Long-1]=Snake.X[Snake.Long-2]-1;
	  Snake.Y[Snake.Long-1]=Snake.Y[Snake.Long-2]+0;
	  for(i=0;i<Snake.Long;i++)
	  {
		  Turnon_point(Snake.X[i],Snake.Y[i]);
	  }
		  Key_Vtemp=3;
    }
	  if(((Key_Value==2||j==168)&&Key_Vtemp!=4)||(Key_Vtemp==2))
	  {
		Turnoff_point(Snake.X[0],Snake.Y[0]);
	  for(i=0;i<Snake.Long-1;i++)
	  {
		   Snake.X[i]=Snake.X[i+1];
		   Snake.Y[i]=Snake.Y[i+1];
	  }
		Snake.X[Snake.Long-1]=Snake.X[Snake.Long-2]+0;
		Snake.Y[Snake.Long-1]=Snake.Y[Snake.Long-2]+1;
   	for(i=0;i<Snake.Long;i++)
	  {
		  Turnon_point(Snake.X[i],Snake.Y[i]);
	  }
		Key_Vtemp=2;
	}
	  if(((Key_Value==4||j==98)&&Key_Vtemp!=2)||(Key_Vtemp==4))
	  {
		Turnoff_point(Snake.X[0],Snake.Y[0]);
	  for(i=0;i<Snake.Long-1;i++)
	  {
		   Snake.X[i]=Snake.X[i+1];
		   Snake.Y[i]=Snake.Y[i+1];
	  }
		Snake.X[Snake.Long-1]=Snake.X[Snake.Long-2]+0;
		Snake.Y[Snake.Long-1]=Snake.Y[Snake.Long-2]-1;
   	for(i=0;i<Snake.Long;i++)
	  {
		  Turnon_point(Snake.X[i],Snake.Y[i]);
	  }
		Key_Vtemp=4;
	}
	  /******判断蛇的死亡******/
	  if(Snake.X[Snake.Long-1]<=11||Snake.X[Snake.Long-1]>=198||Snake.Y[Snake.Long-1]<=11||Snake.Y[Snake.Long-1]>=198)
	  { //蛇碰到边界
		 Snake.Life=1;
	  }
	  for(i=3;i<Snake.Long;i++)
	  {
		 if(Snake.X[i]==Snake.X[0]&&Snake.Y[i]==Snake.Y[0])
		 {
			 Snake.Life=1;
		 }
	  }
		if(Judge_Life(Snake.Level)==0) //难度判定
		{
			Snake.Life=1;
		}
	  if(Snake.Life==1) 		
	  {
			u8 Select=0;
			u8 j=0;
		  LCD_Clear(BLACK);
		  Display_Dead();
		  while(1)
		  { 
			 Select=KEY_Scan(0);
			 j=Remote_Scan(); 
			 delay_ms(100);
			 if(Select==1||Select==2||Select==3||Select==4||j==2) 
			 {
				 delay_ms(100);
				 Play_Snake();
			 }
		  }
	  }

	 if(Snake.Long==Snake_Maxlong) //蛇碰到身体
	 {
		 u8 j=0;
		 u8 select=0;
		 Display_Pass();
		 while(1)
		 {
		   j=Remote_Scan(); 
			 select=KEY_Scan(0);
		   if(select==1||select==2||select==3||select==4||j==2)
		   {
				 delay_ms(100);
			   Play_Snake();
		   }
	   }
	 }
	 /******判断蛇的吃食物******/
	 if(Snake.X[Snake.Long-1]==Food.X&&Snake.Y[Snake.Long-1]==Food.Y) //吃食物情况
	 {
		 Turnoff_point(Food.X,Food.Y);
		 Snake.Long++;  //长度加一
		 Snake.X[Snake.Long-1]=Food.X;
		 Snake.Y[Snake.Long-1]=Food.Y;
		 Snake.Score+=10; //分数加10
		 
		 Food.Yes=1;		 
	 }
	 if(Snake.X[Snake.Long-1]==Food.X&&Snake.Y[Snake.Long-1]==Food.Y+1)
	 {
		 Turnoff_point(Food.X,Food.Y);
		 Snake.Long++;
		 Snake.X[Snake.Long-1]=Food.X;
		 Snake.Y[Snake.Long-1]=Food.Y;
		 Snake.Score+=10;
		 
		 Food.Yes=1;		 
	 }
	 if(Snake.X[Snake.Long-1]==Food.X&&Snake.Y[Snake.Long-1]==Food.Y-1)
	 {
		 Turnoff_point(Food.X,Food.Y);
		 Snake.Long++;
		 Snake.X[Snake.Long-1]=Food.X;
		 Snake.Y[Snake.Long-1]=Food.Y;
		 Snake.Score+=10;
		 
		 Food.Yes=1;		 
	 }
	 if(Snake.X[Snake.Long-1]==Food.X+1&&Snake.Y[Snake.Long-1]==Food.Y)
	 {
		 Turnoff_point(Food.X,Food.Y);
		 Snake.Long++;
		 Snake.X[Snake.Long-1]=Food.X;
		 Snake.Y[Snake.Long-1]=Food.Y;
		 Snake.Score+=10;
		 
		 Food.Yes=1;		 
	 }
	 if(Snake.X[Snake.Long-1]==Food.X-1&&Snake.Y[Snake.Long-1]==Food.Y)
	 {
		 Turnoff_point(Food.X,Food.Y);
		 Snake.Long++;
		 Snake.X[Snake.Long-1]=Food.X;
		 Snake.Y[Snake.Long-1]=Food.Y;
		 Snake.Score+=10;
		 
		 Food.Yes=1;		 
	 }
	 if(Snake.Level==1) {delay_ms(50);} //不同难度 不同速度
	 if(Snake.Level==2) {delay_ms(30);}
	 if(Snake.Level==3) {delay_ms(10);}
 }	 
}

void Play_Snake(void)
{
	u8 i=0;
	u8 j=0;	
	while(1)
	{ 
		Display_Select();
			while(1)
			{
				i=KEY_Scan(0);
				j=Remote_Scan(); 
				if(i==1||i==2||i==3||i==4||j==104||j==152||j==176)
				{
					switch(i)
					{
						case 1:Snake.Level=1;break;
						case 2:Snake.Level=2;break;
						case 3:Snake.Level=3;break;
						case 4:Snake.Level=4;break;
					}
					switch(j)
					{
						case 104:Snake.Level=1;break;
						case 152:Snake.Level=2;break;
						case 176:Snake.Level=3;break;
					}
				  Init_Snake();
			    Run_Snake();
				}
			}
	}
}
	
	


