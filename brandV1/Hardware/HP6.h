#ifndef __HP6_H
#define __HP6_H

#include "main.h"

#define Hp6_ADDR_W  (0x66<<1)
#define Hp6_ADDR_R  (0x66<<1 | 0x01)

// 供电引脚宏定义
#define HP6_Power0N     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0)
#define HP6_Power0FF     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,1)

// 时钟线宏定义
#define HP6_IIC_SCL_L       HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,0)
#define HP6_IIC_SCL_H       HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,1)

// 数据线宏定义
#define HP6_IIC_SDA_OUT_L   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0)
#define HP6_IIC_SDA_OUT_H   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,1)

// 输入
#define HP6_IIC_SDA_IN      HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)


void HP6_Init(void);
u8 HP_6_OpenRate(void);
u8 HP_6_CloseRate(void);
void HP_6_GetRateResult(u8 *result);


u8 HP_6_Openbp(void);
u8 HP_6_Closebp(void);
void HP_6_Get_bpResult(u8 *result);


#endif
