#ifndef __BUTTON_H
#define __BUTTON_H

#include "main.h"

/**********五向按键枚举部分**************/

// 四向状态
typedef enum {
    ADC_KEY_STATE_NONE,
    ADC_KEY_STATE_UP,
    ADC_KEY_STATE_DOWN,
    ADC_KEY_STATE_LEFT,
    ADC_KEY_STATE_RIGHT
} AdcKeyState_t;

// 按键事件
typedef enum {
    KEY_EVENT_NONE = 0,         // 无事件
    KEY_EVENT_UP_PRESS,         // 上
    KEY_EVENT_DOWN_PRESS,       // 下
    KEY_EVENT_LEFT_PRESS,       // 左
    KEY_EVENT_RIGHT_PRESS,      // 右
    KEY_EVENT_CENTER_CLICK,     // OK键 单击
    KEY_EVENT_CENTER_DOUBLE,    // OK键 双击
    KEY_EVENT_CENTER_LONG       // OK键 长按    
} Key_Event_t;

// OK键的GPIO端口和引脚 (按下为高电平)
#define OK_KEY_PORT             GPIOA
#define OK_KEY_PIN              GPIO_PIN_0

#define KEY_SCAN_PERIOD_MS  10

// 时间参数，单位：毫秒
#define DEBOUNCE_TIME_MS        20      // 消抖时间
#define LONG_PRESS_TIME_MS      700     // 长按判断时间
#define DOUBLE_CLICK_GAP_MS     300     // 双击最大间隔时间

// 将时间转换为扫描次数 (内部使用):转换得到实际时间
#define DEBOUNCE_TICKS          (DEBOUNCE_TIME_MS / KEY_SCAN_PERIOD_MS)
#define LONG_PRESS_TICKS        (LONG_PRESS_TIME_MS / KEY_SCAN_PERIOD_MS)
#define DOUBLE_CLICK_TICKS      (DOUBLE_CLICK_GAP_MS / KEY_SCAN_PERIOD_MS)

void Key_Scan(void);
Key_Event_t Get_Key_Event(void);

#endif
