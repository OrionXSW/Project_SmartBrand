#include "Timer.h"

// 说明:定时器硬延时使用Timer11

// 毫秒级延时
// 时钟来源:100MHz
//void delay_ms(uint16_t ms)
//{
//    HAL_Delay(ms);
//    htim11.Init.Period = 10*ms-1;      // 计数周期 注意这个值容易超出范围
//    htim11.Init.Prescaler = 10000-1;       // 分频系数
//    htim11.Instance->CNT = 0;   // 清空计数值
//    HAL_TIM_Base_Init(&htim11);
//    __HAL_TIM_CLEAR_FLAG(&htim11,TIM_FLAG_UPDATE);  // 清空更新标志位
//    HAL_TIM_Base_Start(&htim11);    // 启动计数
//    while(!__HAL_TIM_GET_FLAG(&htim11,TIM_FLAG_UPDATE));
//    HAL_TIM_Base_Stop(&htim11);
//    __HAL_TIM_CLEAR_FLAG(&htim11,TIM_FLAG_UPDATE);  // 清空更新标志位
//}

//// 微秒级延时
//void delay_us(uint16_t us)
//{
//    htim11.Init.Period = us-1;      // 计数周期
//    htim11.Init.Prescaler = 100-1;       // 分频系数
//    htim11.Instance->CNT = 0;   // 清空计数值
//    HAL_TIM_Base_Init(&htim11);
//    __HAL_TIM_CLEAR_FLAG(&htim11,TIM_FLAG_UPDATE);  // 清空更新标志位
//    HAL_TIM_Base_Start(&htim11);    // 启动计数
//    while(!(__HAL_TIM_GET_FLAG(&htim11,TIM_FLAG_UPDATE)));
//    HAL_TIM_Base_Stop(&htim11);
//    __HAL_TIM_CLEAR_FLAG(&htim11,TIM_FLAG_UPDATE);  // 清空更新标志位
//}

//// 1MHz 频率计数
//void delay_us(uint16_t us)
//{
//    __HAL_TIM_SET_COUNTER(&htim11, 0); // 计数器清零
//    __HAL_TIM_ENABLE(&htim11);         // 启动定时器
//    
//    // 等待计数器达到指定的us值
//    while(__HAL_TIM_GET_COUNTER(&htim11) < us);
//    
//    __HAL_TIM_DISABLE(&htim11);        // 停止定时器

//}

/***********************************************
*函数名    :tim11_delay_ms
*函数功能  :定时器11延时
*函数参数  :u16 ms
*函数返回值:无
*函数描述  :100MHZ--------------100000/ms
            10000分频-----------10/ms
************************************************/
void delay_ms(u16 ms)
{
	//定时器时钟使能
	RCC->APB2ENR |= (1<<18);
	//CR1
	TIM11->CR1 |= (1<<7);          //使能影子寄存器
	//TIM11->CR1 |= (1<<3);          //单次计数模式
	TIM11->CR1 &= ~(1<<1);         //产生更新事件
	
	//PSC分频寄存器
	TIM11->PSC = 10000-1;           //10000分频
	//ARR重载寄存器
	TIM11->ARR = 10 * ms - 1;
	//人为产生更新事件UG
	TIM11->EGR |= (1<<0);
	//清除状态寄存器更新完成位
	TIM11->SR &= ~(1<<0);
	//使能计数器
	TIM11->CR1 |= (1<<0);
	//等待计数完成
	while(!(TIM11->SR & (1<<0)));
	
	//关闭定时器
	TIM11->CR1 &= ~(1<<0);

}




/***********************************************
*函数名    :TIM11_delay_us
*函数功能  :定时器11延时微秒
*函数参数  :u16 us
*函数返回值:无
*函数描述  :100MHZ--------------100/us
            50分频-----------2/us
************************************************/
void delay_us(u16 us)
{
	//定时器时钟使能
	RCC->APB2ENR |= (1<<18);
	//CR1
	TIM11->CR1 |= (1<<7);          //使能影子寄存器
	//TIM11->CR1 |= (1<<3);          //单次计数模式
	TIM11->CR1 &= ~(1<<1);         //产生更新事件
	
	//PSC分频寄存器
	TIM11->PSC = 50-1;           //50分频
	//ARR重载寄存器
	TIM11->ARR = 2 * us - 1;
	//人为产生更新事件UG
	TIM11->EGR |= (1<<0);
	//清除状态寄存器更新完成位
	TIM11->SR &= ~(1<<0);
	//使能计数器
	TIM11->CR1 |= (1<<0);
	//等待计数完成
	while(!(TIM11->SR & (1<<0)));
	
	//关闭定时器
	TIM11->CR1 &= ~(1<<0);
}
