#ifndef __SNAKE_H
#define __SNAKE_H

#include "sys.h"
void Display_Begin(void);
void Display_Dead(void);
void Display_Pass(void);
void Turnon_point(u8 x,u8 y);
void Turnoff_point(u8 x,u8 y);
void Run_Snake(void);
void Play_Snake(void);
void Display_Select(void);
void Draw_Wall(void);
u8 Judge_Life(u8 i);
#endif

