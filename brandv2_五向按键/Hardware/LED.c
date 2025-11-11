#include "LED.h"


void LED_Set(uint8_t state)
{
    if(state == 1)
    {
        HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,1);
    }
    if(state == 0)
    {
        HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,0);
    }
    if(state == 2)
    {
        HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
    }
}
