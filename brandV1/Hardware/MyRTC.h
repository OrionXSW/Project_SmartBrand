#ifndef __MYRTC_H
#define __MYRTC_H

#include "main.h"

// 定义一个结构体, 用于一次性处理日期和时间
typedef struct {
    uint16_t Year;      // 年份, 0-99 (例如: 25 代表 2025年)
    uint8_t Month;      // 月份, 1-12
    uint8_t Date;       // 日期, 1-31
    uint8_t WeekDay;    // 星期, 1-7 (参考 HAL 库的 RTC_WEEKDAY_MONDAY 等宏)
    uint8_t Hours;      // 小时, 0-23
    uint8_t Minutes;    // 分钟, 0-59
    uint8_t Seconds;    // 秒, 0-59
} RTC_DateTime_t;

extern RTC_DateTime_t date;

// 函数声明
HAL_StatusTypeDef RTC_SetDateTime(RTC_DateTime_t* dt);
void RTC_GetDateTime(RTC_DateTime_t* dt);


#endif
