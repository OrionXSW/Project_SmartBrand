#include "MyRTC.h"


RTC_DateTime_t date = {25,11,13,4,3,45,50};

/**
  * @brief  设置RTC的日期和时间.
  * @param  dt: 指向包含日期和时间信息的 RTC_DateTime_t 结构体的指针.
  * @retval HAL status (HAL_OK: 成功, HAL_ERROR: 失败).
  */
HAL_StatusTypeDef RTC_SetDateTime(RTC_DateTime_t* dt)
{
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    HAL_StatusTypeDef status = HAL_ERROR;

    // 1. 配置时间
    sTime.Hours = dt->Hours;
    sTime.Minutes = dt->Minutes;
    sTime.Seconds = dt->Seconds;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    status = HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    
    if (status != HAL_OK)
    {
        return status; // 如果设置时间失败, 直接返回错误
    }

    // 2. 配置日期
    sDate.WeekDay = dt->WeekDay;
    sDate.Month = dt->Month;
    sDate.Date = dt->Date;
    sDate.Year = dt->Year;
    status = HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    
    return status; // 返回设置日期的最终状态
}

/**
  * @brief  获取RTC的当前日期和时间.
  * @param  dt: 指向用于存储日期和时间信息的 RTC_DateTime_t 结构体的指针.
  * @retval None.
  */
void RTC_GetDateTime(RTC_DateTime_t* dt)
{
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    // **重要**: 必须先获取时间, 再获取日期.
    // HAL_RTC_GetTime() 会锁存日期寄存器, 防止在读取过程中发生日期跳变.
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    // 将从HAL库读取的数据填充到我们自定义的结构体中
    dt->Year = sDate.Year + 2000;
    dt->Month = sDate.Month;
    dt->Date = sDate.Date;
    dt->WeekDay = sDate.WeekDay;
    dt->Hours = sTime.Hours;
    dt->Minutes = sTime.Minutes;
    dt->Seconds = sTime.Seconds;
}
