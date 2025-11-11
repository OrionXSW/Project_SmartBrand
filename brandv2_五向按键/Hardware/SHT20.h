#ifndef __SHT20_H
#define __SHT20_H

#include "main.h"

uint8_t SHT20_Init(void);
uint8_t SHT20_Read_Temperature(float *temperature);
uint8_t SHT20_Read_Humidity(float *humidity);



#endif
