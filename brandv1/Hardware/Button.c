#include "Button.h"

// 全局按键事件变量，`volatile`关键字是必须的，因为它在中断和主循环中都被访问
static volatile Key_Event_t g_key_event = KEY_EVENT_NONE;
static AdcKeyState_t Get_ADC_Key_State(void); // 新增一个辅助函数声明
// 声明私有辅助函数
static uint16_t Get_ADC_Value(void);
/**
@brief   按键扫描
@param   
@retval  
@details PA0 按下高电平 其他四个方向用ADC判断
*/

void Key_Scan_Handler(void)
{
    // --- 摇杆方向键处理 (只在按下的瞬间触发一次) ---
    static AdcKeyState_t last_adc_state = ADC_KEY_STATE_NONE; // 保存上一次的 *逻辑状态*
    
    // 1. 获取当前的逻辑状态
    AdcKeyState_t current_adc_state = Get_ADC_Key_State();
    
    // 2. 只有当逻辑状态发生变化时才进行处理
    if (current_adc_state != last_adc_state)
    {
        // 3. 我们只关心从“无按键”到“有按键”的跳变，这才是“按下”事件
        if (last_adc_state == ADC_KEY_STATE_NONE && current_adc_state != ADC_KEY_STATE_NONE)
        {
            switch (current_adc_state)
            {
                case ADC_KEY_STATE_UP:    g_key_event = KEY_EVENT_UP_PRESS;    break;
                case ADC_KEY_STATE_DOWN:  g_key_event = KEY_EVENT_DOWN_PRESS;  break;
                case ADC_KEY_STATE_LEFT:  g_key_event = KEY_EVENT_LEFT_PRESS;  break;
                case ADC_KEY_STATE_RIGHT: g_key_event = KEY_EVENT_RIGHT_PRESS; break;
                default: break;
            }
        }
        
        // 4. 更新上一次的状态，为下一次扫描做准备
        last_adc_state = current_adc_state;
    }

    // --- OK键处理 (单击、双击、长按) ---
    // (这部分逻辑是正确的，保持不变)
    static uint8_t  ok_key_state = 0;   // 0:松开状态  1:按下状态(已消抖)
    static uint16_t ok_key_timer = 0;   // 通用计时器

    uint8_t ok_pin_level = HAL_GPIO_ReadPin(OK_KEY_PORT, OK_KEY_PIN);

    // 计时器自增
    ok_key_timer++;

    switch (ok_key_state)
    {
        case 0: // 状态0：等待按键按下
            if (ok_pin_level == GPIO_PIN_SET)
            {
                ok_key_timer = 0;
                ok_key_state = 1;
            }
            break;

        case 1: // 状态1：消抖与判断
            if (ok_pin_level == GPIO_PIN_SET)
            {
                if (ok_key_timer >= DEBOUNCE_TICKS)
                {
                    ok_key_state = 2;
                }
            }
            else
            {
                ok_key_state = 0;
            }
            break;

        case 2: // 状态2：等待释放或长按
            if (ok_pin_level == GPIO_PIN_RESET)
            {
                ok_key_state = 3;
                ok_key_timer = 0;
            }
            else if (ok_key_timer >= LONG_PRESS_TICKS)
            {
                g_key_event = KEY_EVENT_OK_LONG;
                ok_key_state = 4;
            }
            break;

        case 3: // 状态3：等待双击或单击超时
            if (ok_key_timer >= DOUBLE_CLICK_TICKS)
            {
                g_key_event = KEY_EVENT_OK_CLICK;
                ok_key_state = 0;
            }
            else if (ok_pin_level == GPIO_PIN_SET)
            {
                g_key_event = KEY_EVENT_OK_DOUBLE;
                ok_key_state = 4;
            }
            break;

        case 4: // 状态4：长按或双击后，等待按键释放
            if (ok_pin_level == GPIO_PIN_RESET)
            {
                ok_key_state = 0;
            }
            break;
    }
}
/**
 * @brief 从主循环中获取按键事件
 */
Key_Event_t Get_Key_Event(void)
{
    Key_Event_t event = g_key_event;
    if (event != KEY_EVENT_NONE)
    {
        g_key_event = KEY_EVENT_NONE; // 读取后清除，防止重复响应
    }
    return event;
}

/**
 * @brief 辅助函数：将ADC原始值转换为按键逻辑状态
 */
static AdcKeyState_t Get_ADC_Key_State(void)
{
    uint16_t adc_value = Get_ADC_Value(); // 调用之前的ADC读取函数

    if (adc_value > 3800) {
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
 * @brief 辅助函数：获取ADC转换值
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

