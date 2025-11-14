#include "MPU6050.h"
#include <math.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

/* MPU6050命令 */

#define MPU6050_ADDR            0xD0    // 写地址 8位的状态


#define MPU6050_SMPLRT_DIV      0x19
#define MPU6050_CONFIG          0x1A
#define MPU6050_GYRO_CONFIG     0x1B
#define MPU6050_ACCEL_CONFIG    0x1C

#define MPU6050_ACCEL_XOUT_H    0x3B
#define MPU6050_ACCEL_XOUT_L    0x3C
#define MPU6050_ACCEL_YOUT_H    0x3D
#define MPU6050_ACCEL_YOUT_L    0x3E
#define MPU6050_ACCEL_ZOUT_H    0x3F
#define MPU6050_ACCEL_ZOUT_L    0x40
#define MPU6050_TEMP_OUT_H      0x41
#define MPU6050_TEMP_OUT_L      0x42
#define MPU6050_GYRO_XOUT_H     0x43
#define MPU6050_GYRO_XOUT_L     0x44
#define MPU6050_GYRO_YOUT_H     0x45
#define MPU6050_GYRO_YOUT_L     0x46
#define MPU6050_GYRO_ZOUT_H     0x47
#define MPU6050_GYRO_ZOUT_L     0x48

#define MPU6050_PWR_MGMT_1      0x6B
#define MPU6050_PWR_MGMT_2      0x6C
#define MPU6050_WHO_AM_I        0x75


#define MPU6050_ADDRESS        0xD0             //写地址
#define DEFAULT_MPU_HZ         (100)            //定义输出速度
#define q30                    1073741824.0f    //q30格式，long转float时的除数

//陀螺仪方向设置
static signed char gyro_orientation[9] = { 1,  0,  0,
                                           0,  1,  0,
                                           0,  0,  1};
float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
unsigned long sensor_timestamp;
short gyro[3], accel[3], sensors;
unsigned char more;
long quat[4];

uint8_t MPU6050_WriteReg(uint8_t reg, uint8_t data)
{
    uint8_t ack ;
    
    Soft_I2C_Start();
    
    // 发送设备地址（写）
    Soft_I2C_Send_Byte(MPU6050_ADDR);
    ack = Soft_I2C_Rec_Ack();
    if(ack) {
        Soft_I2C_Stop();
        return 1; // NACK received
    }
    // 发送寄存器地址
    Soft_I2C_Send_Byte(reg);
    ack = Soft_I2C_Rec_Ack();
    if(ack) {
        Soft_I2C_Stop();
        return 2;
    }
    // 发送数据
    Soft_I2C_Send_Byte(data);
    ack = Soft_I2C_Rec_Ack();
    if(ack) {
        Soft_I2C_Stop();
        return 3;
    }
    
    Soft_I2C_Stop();
    return 0; // Success
}

uint8_t MPU6050_ReadReg(uint8_t reg)
{
    uint8_t data;
    uint8_t ack;
    
    Soft_I2C_Start();
    
    // 发送设备地址（写）
    Soft_I2C_Send_Byte(MPU6050_ADDR);
    ack = Soft_I2C_Rec_Ack();
    if(ack) {
        Soft_I2C_Stop();
        return 1; // NACK received
    }
    // 发送寄存器地址
    Soft_I2C_Send_Byte(reg);
    ack = Soft_I2C_Rec_Ack();
    if(ack) {
        Soft_I2C_Stop();
        return 2;
    }
    // 重新开始
    Soft_I2C_Start();
    
    // 发送设备地址（读）
    Soft_I2C_Send_Byte(MPU6050_ADDR | 0x01);
    ack = Soft_I2C_Rec_Ack();
    if(ack) {
        Soft_I2C_Stop();
        return 1; // NACK received
    }
    // 接收数据
    data = Soft_I2C_Rec_Byte();
    // 发送NACK（停止读取）
    Soft_I2C_Send_Ack(1);
    
    Soft_I2C_Stop();
    return data;
}


// MPU6050 初始化
// 解除睡眠模式
// 6个寄存器启动 满量程
// 退出休眠->六轴上电->2量程,采样率,低通滤波

void MPU6050_Init(void)
{
    printf("MPU6050 Initialization\r\n");
    
    uint8_t id = MPU6050_ReadReg(MPU6050_WHO_AM_I);
    printf("ID:%#x\r\n",id);
    
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x00);  // 退出睡眠模式
    delay_ms(50);   // 延时一段时间确认状态
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);  // 唤醒6个轴传感器
    
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18);     // 陀螺仪配置:  ±2000°/s
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);    // 加速度计配置:±16g 最大量程
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x07);      // 采样率: 125Hz
    MPU6050_WriteReg(MPU6050_CONFIG,0x06);          // 低通滤波器:5Hz    
}

/**
  * @brief  DMP初始化函数
  * @param  无
  * @retval 无
  */
void MPU6050_DMPInit(void)
{
	uint8_t res = 0;
    
	Soft_I2C_Init();
    res = mpu_init();
    if(!res)
    {
        printf("mpu initialization complete ......\r\n");
        
		//设置所需要的传感器
        res = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
        if(!res)
            printf("mpu_set_sensor complete ......\r\n");
        else
            printf("mpu_set_sensor come across error ......\r\n");
		
        //设置FIFO
        res = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
        if(!res)
            printf("mpu_configure_fifo complete ......\r\n");
        else
            printf("mpu_configure_fifo come across error ......\r\n");
        
		//设置采样率
        res = mpu_set_sample_rate(DEFAULT_MPU_HZ);
        if(!res)
            printf("mpu_set_sample_rate complete ......\r\n");
        else
            printf("mpu_set_sample_rate come across error ......\r\n");
        
		//加载DMP固件
        __disable_irq(); // 关闭所有中断
        res = dmp_load_motion_driver_firmware();
        __enable_irq();
        
        if(!res)
            printf("dmp_load_motion_driver_firmware complete ......\r\n");
        else
            printf("dmp_load_motion_driver_firmware come across error ......\r\n");
        
		//设置陀螺仪方向
        res = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));
        if(!res)
            printf("dmp_set_orientation complete ......\r\n");
        else
            printf("dmp_set_orientation come across error ......\r\n");
        
		//设置DMP功能
        res = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |	              
              DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
              DMP_FEATURE_GYRO_CAL);
        if(!res)
            printf("dmp_enable_feature complete ......\r\n");
        else
            printf("dmp_enable_feature come across error ......\r\n");
        
		//设置DMP输出速率(最大不超过200Hz)
        res = dmp_set_fifo_rate(DEFAULT_MPU_HZ);
        if(!res)
            printf("dmp_set_fifo_rate complete ......\r\n");
        else
            printf("dmp_set_fifo_rate come across error ......\r\n");
        
		//自检
        res = run_self_test();

        if(!res)
            printf("mpu_run_self_test complete ......\r\n");
        else
            printf("mpu_run_self_test come across error ......\r\n");
        
		//使能DMP
        res = mpu_set_dmp_state(1);
        if(!res)
            printf("mpu_set_dmp_state complete ......\r\n");
        else
            printf("mpu_set_dmp_state come across error ......\r\n");
    }
    else
    {
        printf("mpu initialization come across error ......\r\n");
        while(1);
    }
    
}


void MPU6050_GetData_function(mpu6050* data)
{
    uint8_t buffer[14];
    
    // 一次性读取从 0x3B 开始的14个字节（加速度+温度+陀螺仪）
    // 注意：你的 MPU6050_ADDR 是0xD0(8位地址)，MPU6050_Read_Len需要7位地址(0x68)
    MPU6050_Read_Len(MPU6050_ADDR >> 1, MPU6050_ACCEL_XOUT_H, 14, buffer);

    // 将高低字节合并成16位有符号数
    data->ACCELX = (int16_t)((buffer[0] << 8) | buffer[1]);
    data->ACCELY = (int16_t)((buffer[2] << 8) | buffer[3]);
    data->ACCELZ = (int16_t)((buffer[4] << 8) | buffer[5]);
    // buffer[6]和[7]是温度，这里跳过
    data->GYROX  = (int16_t)((buffer[8] << 8) | buffer[9]);
    data->GYROY  = (int16_t)((buffer[10] << 8) | buffer[11]);
    data->GYROZ  = (int16_t)((buffer[12] << 8) | buffer[13]);     
    
}

// 输出处理后的数据
void MPU6050_GetData(mpu6050* data)
{
    MPU6050_GetData_function(data);     // 获取初始值
    
    // 处理数据
    data->accel_x = (float)data->ACCELX / ACCEL_SENSITIVITY;
    data->accel_y = (float)data->ACCELY / ACCEL_SENSITIVITY;
    data->accel_z = (float)data->ACCELZ / ACCEL_SENSITIVITY;
    
    data->gyro_x = (float)data->GYROX / GYRO_SENSITIVITY;
    data->gyro_y = (float)data->GYROY / GYRO_SENSITIVITY;
    data->gyro_z = (float)data->GYROZ / GYRO_SENSITIVITY;    
    
    
}




/**
  * @brief  MPU6050指定地址连续写字节函数
  * @param  Addr 器件地址
  * @param  Reg  寄存器地址
  * @param  Len  要写入的数据长度
  * @param  Buf  写入数据的存储区
  * @retval 返回0表示正常，其他表示失败
  */
uint8_t MPU6050_Write_Len(uint8_t Addr, uint8_t Reg, uint8_t Len, uint8_t *Buf)
{
    Soft_I2C_Start();
	Soft_I2C_Send_Byte((Addr << 1) | 0x00);    //发送器件地址+写命令
//    Soft_I2C_Send_Byte(Addr);
	if(Soft_I2C_Rec_Ack())    //等待应答
	{
		Soft_I2C_Stop();
		return 1;
	}
    Soft_I2C_Send_Byte(Reg);	  //写寄存器地址
    if(Soft_I2C_Rec_Ack())    //等待应答
	{
		Soft_I2C_Stop();
		return 1;
	}
	while(Len--)
	{
		Soft_I2C_Send_Byte(*Buf++);    //发送数据
		if(Soft_I2C_Rec_Ack())     //等待ACK
		{
			Soft_I2C_Stop();
			return 1;
		}
	}
    Soft_I2C_Stop();
	return 0;
} 

/**
  * @brief  MPU6050指定地址连续读字节函数
  * @param  Addr 器件地址
  * @param  Reg  寄存器地址
  * @param  Len  要读取的数据长度
  * @param  Buf  读取数据的存储区
  * @retval 返回0表示正常，其他表示失败
  */
uint8_t MPU6050_Read_Len(uint8_t Addr, uint8_t Reg, uint8_t Len, uint8_t *Buf)
{
	Soft_I2C_Start();
	Soft_I2C_Send_Byte((Addr << 1) | 0x00);    //发送器件地址+写命令
//	Soft_I2C_Send_Byte(Addr);
    if(Soft_I2C_Rec_Ack())
	{
		Soft_I2C_Stop();
		return 1;
	}
	Soft_I2C_Send_Byte(Reg);      //写寄存器地址
	if(Soft_I2C_Rec_Ack())    //等待应答
	{
		Soft_I2C_Stop();
		return 1;
	}
	Soft_I2C_Start();
	Soft_I2C_Send_Byte((Addr << 1) | 0x01);    //发送器件地址+读命令
//    Soft_I2C_Send_Byte(Addr| 0x01);
    
	if(Soft_I2C_Rec_Ack())    //等待应答
	{
		Soft_I2C_Stop();
		return 1;
	}
	while(Len--)
	{
		*Buf++ = Soft_I2C_Rec_Byte();    //读数据
		if(Len)  {Soft_I2C_Send_Ack(0);}     //发送ACK
		else     {Soft_I2C_Send_Ack(1);}     //发送nACK	
	}
	Soft_I2C_Stop();
	return 0;
}

/**
  * @brief  获取DMP处理后数据函数
  * @param  Pitch 俯仰角，精度:0.1°，范围:-90.0° ~ +90.0°
  * @param  Roll  横滚角，精度:0.1°，范围:-180.0°~ +180.0°
  * @param  yaw   偏航角，精度:0.1°，范围:-180.0°~ +180.0°
  * @retval 返回0表示正常，其他表示失败
  */
uint8_t MPU6050_ReadDMP(float *Pitch, float *Roll, float *Yaw)
{	
	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more))  return 1;	
	if(sensors & INV_WXYZ_QUAT)
	{    
        q0 = quat[0] / q30;    //q30格式转换为浮点数
        q1 = quat[1] / q30;
        q2 = quat[2] / q30;
        q3 = quat[3] / q30;
        //计算得到俯仰角、横滚角和偏航角
        *Pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3; 	
        *Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;
        *Yaw   = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.3;
    }else  return 2;
    return 0;
}


