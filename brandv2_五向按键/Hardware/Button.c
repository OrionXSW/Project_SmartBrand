#include "Button.h"

volatile Key_Event_t g_key_event = KEY_EVENT_NONE;

static uint16_t Get_ADC_Value(void);
static AdcKeyState_t Get_ADC_Key_State(void);

// 按键轮询部分

void Key_Scan(void)
{
    // 摇杆方向键处理
    static AdcKeyState_t last_adc_state = ADC_KEY_STATE_NONE;  // 保留上一次的电压状态
    
    AdcKeyState_t current_adc_state = Get_ADC_Key_State(); // 获取摇杆当前按键状态
    
    // 状态变化时才处理
    if(current_adc_state != last_adc_state)
    {   // 发生状态变化:只对无按键到有按键的跳变进行处理
        if(last_adc_state == ADC_KEY_STATE_NONE && current_adc_state != ADC_KEY_STATE_NONE)
        {
            switch(current_adc_state)   // 轮询获取
            {
                case ADC_KEY_STATE_UP: g_key_event = KEY_EVENT_UP_PRESS;       break;
                case ADC_KEY_STATE_DOWN:  g_key_event = KEY_EVENT_DOWN_PRESS;  break;
                case ADC_KEY_STATE_LEFT:  g_key_event = KEY_EVENT_LEFT_PRESS;  break;
                case ADC_KEY_STATE_RIGHT: g_key_event = KEY_EVENT_RIGHT_PRESS; break;
                default: break;
            }
        }
        
        // 更新上一次的状态
        last_adc_state = current_adc_state;
    }
    
    // 对于按键状况进行处理(单击,双击,长按)
    static uint8_t center_key_state = 0;   // 0:松开 1:按下状态
    static uint16_t center_key_timer = 0;   // 按下计数器
    
    uint8_t center_pin_level = HAL_GPIO_ReadPin(OK_KEY_PORT,OK_KEY_PIN);    // 获取按键电平
    
    center_key_timer++;
    
    switch(center_key_state)
    {
        case 0: // 状态0：等待按键按下或按键空闲
            if(center_pin_level == GPIO_PIN_SET)
            {
                center_key_timer = 0;
                center_key_state = 1;   // 进入消抖状态
            }
            break;
        case 1: // 状态1:消抖和判断
            if(center_pin_level == GPIO_PIN_SET)
            {
                if(center_key_timer >= DEBOUNCE_TICKS)
                {   // 超过消抖时间,进入状态2
                    center_key_state = 2;
                }
            }
            else
            {
                center_key_state = 0;   // 抖动
            }
            break;
        case 2: // 状态2:等待释放或长按
            if(center_pin_level == GPIO_PIN_RESET)
            {
                center_key_timer = 0;   // 清空计时器
                center_key_state = 3;   // 状态3
            }
            else if(center_key_timer >= LONG_PRESS_TICKS)
            {
                center_key_state = 4;   // 状态4
                g_key_event = KEY_EVENT_CENTER_LONG;  // 长按事件
            }
            break;
        case 3: // 状态3:等待双击或单击超时
            if(center_key_timer > DOUBLE_CLICK_TICKS)
            {   // 超过双击时间范围,判断为单击
                g_key_event = KEY_EVENT_CENTER_CLICK;
                center_key_state = 0;
            }
            else if(center_pin_level == GPIO_PIN_SET)
            {   // 检查电平再次按下判断为双击
                g_key_event = KEY_EVENT_CENTER_DOUBLE;
                center_key_state = 4;
            }
            break;
        case 4: // 状态4:双击或长按后等待按键释放
            if(center_pin_level == GPIO_PIN_RESET)
            {
                center_key_state = 0;
            }
            break;       
    }
}

// 轮询获取按键时间

Key_Event_t Get_Key_Event(void)
{
    Key_Event_t event = g_key_event;
    if (event != KEY_EVENT_NONE)
    {
        g_key_event = KEY_EVENT_NONE; // 读取后清除，防止重复响应
    }
    return event;
}

// 根据ADC情况返回按键状态
static AdcKeyState_t Get_ADC_Key_State(void)
{
    uint16_t adc_value = Get_ADC_Value(); // 调用之前的ADC读取函数

    if (adc_value > 4000) {
        return ADC_KEY_STATE_LEFT;
    } else if (adc_value > 1900 && adc_value < 2200) {
        return ADC_KEY_STATE_RIGHT;
    } else if (adc_value > 900 && adc_value < 1200) {
        return ADC_KEY_STATE_UP;
    } else if (adc_value > 1250 && adc_value < 1550) {
        return ADC_KEY_STATE_DOWN;
    }
    
    // 如果不符合任何一个按键的范围，则认为是未按下
    return ADC_KEY_STATE_NONE;
}



/**
 * @brief 取ADC转换值
 */
static uint16_t Get_ADC_Value(void)
{
    uint16_t adc_value = 0;
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) // 等待转换，超时时间设短一些
    {
        adc_value = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);
    return adc_value;
}