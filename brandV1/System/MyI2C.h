#ifndef __MYI2C_H
#define __MYI2C_H

#include "main.h"

#define I2C_SCL(x)          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,(GPIO_PinState)x)
#define I2C_SDA(x)          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,(GPIO_PinState)x)
#define I2C_SDA_IN()          HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)


void Soft_I2C_Init(void);
void Soft_I2C_Start(void);
void Soft_I2C_Stop(void);
void Soft_I2C_Send_Ack(uint8_t ack);
uint8_t Soft_I2C_Rec_Ack(void);
void Soft_I2C_Send_Byte(uint8_t data);
uint8_t Soft_I2C_Rec_Byte(void);


#endif
