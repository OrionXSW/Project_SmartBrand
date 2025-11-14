#ifndef __MENU_H
#define __MENU_H

#include "main.h"


#define MAIN_PAGE   1           // 主页
#define CLOCK_PAGE  2           // 时钟界面
#define CLOCK_SET_PAGE  21      // 时钟设置界面
#define TEMP_PAGE   3           // 温湿度界面
#define MOTION_PAGE 4           // MPU6050界面
#define HR_PAGE     5           // 心率界面
#define BP_PAGE     6           // 血压界面

typedef struct{

    uint8_t page_flag;  // 只显示一次标志位
    uint8_t page_mode;  // 页面
    uint8_t keyval; // 按键值
    


}data_t;

extern data_t data_control;
extern uint8_t motion_flag;


void main_page(void);
void clock_page(void);
void Sensor_page(void);
void motion_page(void);
void HeartRate_page(void);
void BP_page(void);


#endif
