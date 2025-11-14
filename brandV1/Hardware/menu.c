#include "menu.h"

data_t data_control = {1,1,0};

uint8_t motion_flag = 0;    // 动图控制标志位


// 按键切换界面函数
uint8_t  screen_move(uint8_t mode)
{
    static uint8_t index = 0;
    static uint8_t current[6] = {1,2,3,4,5,6};
    
    if(mode == 1)       // 加界面
    {
        index++;
        
        if(index == 6){
            index = 0;
        }
    }
    else if(mode == 2)
    {

        if(index == 0){
            index = 6;
        }
        index--;
    }
    
    return current[index];
}


// 主界面
void main_page(void)
{
    
    /*                 执行一次部分                      */
     if(data_control.page_flag == 1)
     {
         data_control.page_flag = 0;
         
        // 清屏
         OLED_Clear(BLACK);

         
         // 显示动图
         motion_flag = 1;
         
         OLED_ShowString(3,65,(uint8_t *)"Hello",16);
         
     }
     
     
    
    
    /*                 高速轮询部分                      */ 
        
     switch(data_control.keyval)
     {
         
         case KEY_EVENT_LEFT_PRESS:data_control.page_mode =  screen_move(2);    // 减界面
         data_control.page_flag = 1; motion_flag = 0;
         break;
         case KEY_EVENT_RIGHT_PRESS:data_control.page_mode =  screen_move(1);    // 加界面
         data_control.page_flag = 1; motion_flag = 0;
         break;   // 切换为时钟界面
     }
     
}

// 时钟界面
void clock_page(void)
{
    
    /*                 执行一次部分                      */
     if(data_control.page_flag == 1)
     {
         data_control.page_flag = 0;
         
        // 清屏
         OLED_Clear(BLACK);
         
         OLED_ShowString(0,0,(uint8_t *)"Clock",16);
         
     }     
    
    
    /*                 高速轮询部分                      */ 
     switch(data_control.keyval)
     {
         case KEY_EVENT_LEFT_PRESS:data_control.page_mode =  screen_move(2);    // 减界面
         data_control.page_flag = 1;
         break;
         case KEY_EVENT_RIGHT_PRESS:data_control.page_mode =  screen_move(1);    // 加界面
         data_control.page_flag = 1;         
         break;   // 切换为主页
     }   
}

// 温湿度界面
void Sensor_page(void)
{
    float wendu,shidu;
    uint8_t buff[10];
    /*                 执行一次部分                      */
     if(data_control.page_flag == 1)
     {
         
         data_control.page_flag = 0;
         
        // 清屏
         OLED_Clear(BLACK);
         OLED_ShowImage(0,0,64,64,sun); // 显示图标:晴天
                  
     }     
    
     
    /*                 高速轮询部分                      */ 
     
     if(task_code[0] >= 500)
     {
         task_code[0] = 0;
         
         // 获取数据
         SHT20_Read_Humidity(&shidu);
         SHT20_Read_Temperature(&wendu);
         
         // 显示数据
         sprintf((char *)buff,"T:%0.1f ",wendu);
         OLED_ShowString(1,65,buff,16);
         sprintf((char *)buff,"R:%0.1f%% ",shidu);
         OLED_ShowString(4,65,buff,16);         
         
     }
     
     
     
     switch(data_control.keyval)
     {
         case KEY_EVENT_LEFT_PRESS:data_control.page_mode =  screen_move(2);    // 减界面
         data_control.page_flag = 1;
         break;
         case KEY_EVENT_RIGHT_PRESS:data_control.page_mode =  screen_move(1);    // 加界面
         data_control.page_flag = 1;         
         break;   // 切换为主页
     }    
}

// mpu6050界面
void motion_page(void)
{
    mpu6050 origin_data;
    uint8_t buff[50];
    /*                 执行一次部分                      */
     if(data_control.page_flag == 1)
     {
         data_control.page_flag = 0;
         
        // 清屏
         OLED_Clear(BLACK);
     }  
     
     

    /*                 高速轮询部分                      */ 
     
     MPU6050_GetData(&origin_data);     // 获取数据
     
     printf("GYROX:%0.2f\r\n",(origin_data.GYROX) / 16.4);
     // 写入缓冲区 并 显示
     sprintf((char *)buff,"Ax:%0.1f Gx:%0.2f",(origin_data.ACCELX/ 16384.0 * 10),((float)origin_data.GYROX/16.4));
     OLED_ShowString(0,0,buff,16);
     sprintf((char *)buff,"Ay:%0.1f Gy:%0.1f",(origin_data.ACCELY/ 16384.0 * 10),((float)origin_data.GYROY/16.4));
     OLED_ShowString(2,0,buff,16);
     sprintf((char *)buff,"Az:%0.1f Gz:%0.1f",(origin_data.ACCELZ/ 16384.0 * 10),((float)origin_data.GYROZ/16.4));
     OLED_ShowString(4,0,buff,16);
     
     switch(data_control.keyval)
     {
         case KEY_EVENT_LEFT_PRESS:data_control.page_mode =  screen_move(2);    // 减界面
         data_control.page_flag = 1;
         break;
         case KEY_EVENT_RIGHT_PRESS:data_control.page_mode =  screen_move(1);    // 加界面
         data_control.page_flag = 1;         
         break;   // 切换为主页
     } 
     
}

// 心率界面
void HeartRate_page(void)
{
    
    /*                 执行一次部分                      */
     if(data_control.page_flag == 1)
     {
         data_control.page_flag = 0;
         
        // 清屏
         OLED_Clear(BLACK);
         
         OLED_ShowString(0,0,(uint8_t *)"心率",16);
                
     }
    
    /*                 高速轮询部分                      */ 
     switch(data_control.keyval)
     {
         case KEY_EVENT_LEFT_PRESS:data_control.page_mode =  screen_move(2);    // 减界面
         data_control.page_flag = 1;
         break;
         case KEY_EVENT_RIGHT_PRESS:data_control.page_mode =  screen_move(1);    // 加界面
         data_control.page_flag = 1;         
         break;   // 切换为主页
     }  
}

// 血压界面
void BP_page(void)
{
    
    /*                 执行一次部分                      */
     if(data_control.page_flag == 1)
     {
         data_control.page_flag = 0;
         
        // 清屏
         OLED_Clear(BLACK);
         
         OLED_ShowString(0,0,(uint8_t *)"血压",16);
                
     }     
    
    
    /*                 高速轮询部分                      */ 
     switch(data_control.keyval)
     {
         case KEY_EVENT_LEFT_PRESS:data_control.page_mode =  screen_move(2);    // 减界面
         data_control.page_flag = 1;
         break;
         case KEY_EVENT_RIGHT_PRESS:data_control.page_mode =  screen_move(1);    // 加界面
         data_control.page_flag = 1;         
         break;   // 切换为主页
     }        
}


