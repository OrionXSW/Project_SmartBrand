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
         
        HP_6_CloseRate();       // 关闭心率测量
        HP_6_Closebp();         // 关闭血压测量         
        // 清屏
         OLED_Clear(BLACK);

         motor_action();        // 马达振动
         
         // 显示动图
         motion_flag = 1;
         
         OLED_ShowString(3,80,(uint8_t *)"Hello",16);
         
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
    uint8_t buff[30];
    /*                 执行一次部分                      */
     if(data_control.page_flag == 1)
     {
         data_control.page_flag = 0;
         
        // 清屏
         OLED_Clear(BLACK);
         
         task_code[5]=1000;    // 更新时间
         
         printf("Clock:%d-%d-%d %02d:%02d:%02d\r\n",date.Year+2000,date.Month,date.Date,date.Hours,date.Minutes,date.Seconds);
         
     }     
    
    
    /*                 高速轮询部分                      */ 
     
     if(task_code[5] >= 1000)
     {
        task_code[5] = 0;
         
        RTC_GetDateTime(&date);    // 获取时间
		sprintf((char *)buff,"%d.%02d.%02d",date.Year+2000,date.Month,date.Date);
		OLED_ShowString(0,25,buff,16); 
        sprintf((char *)buff,"%02d:%02d:%02d",date.Hours,date.Minutes,date.Seconds);
		OLED_ShowString(2,0,buff,24);
		sprintf((char *)buff,"week:%d",date.WeekDay);
		OLED_ShowString(6,40,buff,16);         
         
     }
     
     
     
     switch(data_control.keyval)
     {
         case KEY_EVENT_LEFT_PRESS:data_control.page_mode =  screen_move(2);    // 减界面
         data_control.page_flag = 1;
         break;    // 左移右移切换界面
         case KEY_EVENT_RIGHT_PRESS:data_control.page_mode =  screen_move(1);    // 加界面
         data_control.page_flag = 1;         
         break;   // 切换为主页
         case KEY_EVENT_CENTER_LONG:data_control.page_mode =  CLOCK_SET_PAGE;    // 修改时间界面
         data_control.page_flag = 1;         
         break;   // 长按切换为设置时间界面
     }   
}

// 设置时间界面
void Clock_Set_pate(void)
{
    uint8_t buff[30];
    static RTC_DateTime_t time;     // 时间变量
    static uint8_t set_list = 0;
    
    /*                 执行一次部分                      */
     if(data_control.page_flag == 1)
     {
         data_control.page_flag = 0;
         
        // 清屏
         OLED_Clear(BLACK);
         
         set_list = 0;
         time = date;   // 备份时间   
     } 

    /*                 高速轮询部分                      */ 
    sprintf((char *)buff,"%02d:%02d:%02d",time.Hours,time.Minutes,time.Seconds);
    OLED_ShowString(2,0,buff,24);
    OLED_ShowString(5,set_list,"--",24);    // 设置横杠
     
     // 右移
    if(data_control.keyval == KEY_EVENT_RIGHT_PRESS)
    {
        OLED_ShowString(5,set_list,"..",24);
        set_list+= 48;
        (set_list>96)?(set_list=0):(set_list);
    }
    // 左移
    else if(data_control.keyval == KEY_EVENT_LEFT_PRESS)
    {
        OLED_ShowString(5,set_list,"..",24);
        (set_list>=48)?(set_list-=48):(set_list = 96);
    }
    
    // 上下调节
    if(data_control.keyval == KEY_EVENT_UP_PRESS)
    {
        switch(set_list)
        {
            case 0:  time.Hours++; (time.Hours > 23) ? (time.Hours = 0) : (time.Hours);break;
            case 48: time.Minutes++; (time.Minutes > 59) ? (time.Minutes = 0) : (time.Minutes);break;
            case 96: time.Seconds++; (time.Seconds > 59) ? (time.Seconds = 0) : (time.Seconds);break;
        }
    }
    else if(data_control.keyval == KEY_EVENT_DOWN_PRESS)
    {
        switch(set_list)
        {
            case 0:  (time.Hours > 0) ? (time.Hours--) : (time.Hours=23);break;
            case 48: (time.Minutes > 0) ? (time.Minutes--) : (time.Minutes = 59);break;
            case 96: (time.Seconds > 0) ? (time.Seconds--) : (time.Seconds = 59);break;
        }      
    }
    
    // 长按退出
    if(data_control.keyval == KEY_EVENT_CENTER_DOUBLE)
    {
        // 是否修改时间
        if(time.Hours != date.Hours || time.Minutes != date.Minutes || time.Seconds != date.Seconds)
        {
            printf("Set:%d-%d-%d %02d:%02d:%02d\r\n",time.Year+2000,time.Month,time.Date,time.Hours,time.Minutes,time.Seconds);
            date = time; 
            RTC_SetDateTime(&date); // 设置时间
            printf("Set:%d-%d-%d %02d:%02d:%02d\r\n",date.Year+2000,date.Month,date.Date,date.Hours,date.Minutes,date.Seconds);
        }
        data_control.page_flag = 1;
        data_control.page_mode = CLOCK_PAGE;
        
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
         
        HP_6_CloseRate();       // 关闭心率测量
        HP_6_Closebp();         // 关闭血压测量
        // 清屏
         OLED_Clear(BLACK);
     }  
     
     

    /*                 高速轮询部分                      */ 
     
     MPU6050_GetData(&origin_data);     // 获取数据
     

     // 写入缓冲区 并 显示
     sprintf((char *)buff,"Ax:%0.1f Gx:%0.1f   ",(origin_data.accel_x),(origin_data.gyro_x));
     OLED_ShowString(0,2,buff,16);
     sprintf((char *)buff,"Ay:%0.1f Gy:%0.1f   ",(origin_data.accel_y),(origin_data.gyro_y));
     OLED_ShowString(2,2,buff,16);
     sprintf((char *)buff,"Az:%0.1f Gz:%0.1f   ",(origin_data.accel_z),(origin_data.gyro_z));
     OLED_ShowString(4,2,buff,16);
     
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
    static u8 err;
	u8 result = 0;
	static u8 old_val;
	u8 buff[20];
    
    /*                 执行一次部分                      */
     if(data_control.page_flag == 1)
     {
         data_control.page_flag = 0;
         
        // 清屏
        OLED_Clear(BLACK);

        HP_6_Closebp();         // 关闭血压测量
		//显示界面提示
		OLED_ShowImage(0,0,54,54,heart);
		OLED_ShowString(1,80,(uint8_t *)"心率",16);
		OLED_ShowString(4,80,(uint8_t *)"test",16);
		//开启心率测量
		err = HP_6_OpenRate();
		//超时启动报告传感器错误
		if(err == 0)
		{
			OLED_ShowString(4,80,(uint8_t *)"err!",16);
		}
                
     }
    
    /*                 高速轮询部分                      */ 
     
     
	HP_6_GetRateResult(&result);  
	if(result!=0 && err && old_val!=result)     
	{
		old_val = result;
		sprintf((char *)buff,"%03d",result);
		OLED_ShowString(4,70,buff,24);
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

// 血压界面
void BP_page(void)
{
    
    /*                 执行一次部分                      */
     if(data_control.page_flag == 1)
     {
         data_control.page_flag = 0;
         
        // 清屏
         OLED_Clear(BLACK);
         HP_6_CloseRate();
            //显示测试量血压图片
         OLED_ShowImage(0,0,54,54,bmp);
         OLED_ShowString(1,80,(uint8_t *)"血压",16);         
        //开启血压
         uint8_t err = HP_6_Openbp();
		//超时启动报告传感器错误
		if(err == 0)
		{
			OLED_ShowString(3,80,(uint8_t *)"err!",16);
		}         
         
     }     
    
    
    /*                 高速轮询部分                      */ 
     
    u8 bpResult[2]={0};
    u8 buff[20];

	HP_6_Get_bpResult(bpResult);
	if(bpResult[0]!=0 && bpResult[1]!=0)
	{
		sprintf((char *)buff,"H:%3dPa",bpResult[0]);
		OLED_ShowString(3,70,buff,16);
		sprintf((char *)buff,"L:%3dPa",bpResult[1]);
		OLED_ShowString(5,70,buff,16);
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


