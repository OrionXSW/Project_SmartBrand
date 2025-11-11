#include "Motor.h"

/**
@brief   马达振动
@param   void
@retval  void
@details 这个马达只需高低电平就能控制了,高电平导通,低电平关断,典型的三极管开关功能
*/

void motor_action(void)
{
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,1);
  HAL_Delay(200);
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,0);
}
