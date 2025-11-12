#ifndef __OLED_H
#define __OLED_H

#include "main.h"

// 发送数据/命令宏定义
#define OLED_CMD   0
#define OLED_DATE   1

// 颜色选择
#define WHITE   1
#define BLACK   0

// 命令数据线定义
#define OLED_DC_H   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,1)
#define OLED_DC_L   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,0)

// 片选线宏定义
#define OLED_CS_H   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,1)
#define OLED_CS_L   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,0)

// 复位线宏定义
#define OLED_RST_H   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,1)
#define OLED_RST_L   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0) 


void OLED_Init(void);
void OLED_Clear(uint8_t color);
void OLED_ClearArea(uint8_t x,uint8_t y,uint8_t width,uint8_t height,uint8_t color);

void OLED_ShowChar(uint8_t page,uint8_t col,uint8_t ch,uint8_t size);
void OLED_ShowChinese(uint8_t page,uint8_t col,uint8_t *ch,uint8_t size);
void OLED_ShowString(uint8_t page,uint8_t col,uint8_t *str,uint8_t size);
void OLED_ShowImage(uint8_t page,uint8_t col,uint8_t w,uint8_t h,const uint8_t *image);
#endif
