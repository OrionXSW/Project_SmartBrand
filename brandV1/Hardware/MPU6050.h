#ifndef __MPU6050_H
#define __MPU6050_H

#include "main.h"

#define ACCEL_SENSITIVITY 2048.0f  // ±2g时的灵敏度
#define GYRO_SENSITIVITY  16.4f    // ±250°/s时的灵敏度

typedef struct{

    int16_t GYROX;
    int16_t GYROY;
    int16_t GYROZ;
    int16_t ACCELX;
    int16_t ACCELY;
    int16_t ACCELZ;

    // 处理后的数据（直接使用这些值）
    float accel_x;  // 加速度X轴 (g)
    float accel_y;  // 加速度Y轴 (g)
    float accel_z;  // 加速度Z轴 (g)
    float gyro_x;   // 角速度X轴 (°/s)
    float gyro_y;   // 角速度Y轴 (°/s)
    float gyro_z;   // 角速度Z轴 (°/s)    
 
} mpu6050;





typedef struct{

    float pitch;
    float roll;
    float yaw;
    uint8_t update_flag;

} mpu6050_motion_t;

extern mpu6050_motion_t motion_data;

void MPU6050_Init(void);
void MPU6050_DMPInit(void);

// 连续写多字节
uint8_t MPU6050_Write_Len(uint8_t Addr, uint8_t Reg, uint8_t Len, uint8_t *Buf);
// 连续读多字节
uint8_t MPU6050_Read_Len(uint8_t Addr, uint8_t Reg, uint8_t Len, uint8_t *Buf);

// 获取原始6轴数据
void MPU6050_GetData(mpu6050* data);
// 获取DMP姿态解算后的俯仰角pitch,横滚角roll,偏航角数值Yaw
uint8_t MPU6050_ReadDMP(float *Pitch, float *Roll, float *Yaw);
#endif
