#ifndef __MPU6050_H
#define __MPU6050_H

#include "main.h"

typedef struct{

    int16_t GYROX;
    int16_t GYROY;
    int16_t GYROZ;
    int16_t ACCELX;
    int16_t ACCELY;
    int16_t ACCELZ;

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
