#include "Timer.h"

// 说明:定时器硬延时使用Timer11

// 毫秒级延时
// 时钟来源:100MHz
void delay_ms(uint16_t ms)
{
    htim11.Init.Period = 10*ms-1;      // 计数周期 注意这个值容易超出范围
    htim11.Init.Prescaler = 10000-1;       // 分频系数
    htim11.Instance->CNT = 0;   // 清空计数值
    HAL_TIM_Base_Init(&htim11);
    __HAL_TIM_CLEAR_FLAG(&htim11,TIM_FLAG_UPDATE);  // 清空更新标志位
    HAL_TIM_Base_Start(&htim11);    // 启动计数
    while(!__HAL_TIM_GET_FLAG(&htim11,TIM_FLAG_UPDATE));
    HAL_TIM_Base_Stop(&htim11);
    __HAL_TIM_CLEAR_FLAG(&htim11,TIM_FLAG_UPDATE);  // 清空更新标志位
}

// 微秒级延时
void delay_us(uint16_t us)
{
    htim11.Init.Period = us-1;      // 计数周期
    htim11.Init.Prescaler = 100-1;       // 分频系数
    htim11.Instance->CNT = 0;   // 清空计数值
    HAL_TIM_Base_Init(&htim11);
    __HAL_TIM_CLEAR_FLAG(&htim11,TIM_FLAG_UPDATE);  // 清空更新标志位
    HAL_TIM_Base_Start(&htim11);    // 启动计数
    while(!(__HAL_TIM_GET_FLAG(&htim11,TIM_FLAG_UPDATE)));
    HAL_TIM_Base_Stop(&htim11);
    __HAL_TIM_CLEAR_FLAG(&htim11,TIM_FLAG_UPDATE);  // 清空更新标志位
}
