#include "SHT20.h"

// SHT20 I2C地址和命令
#define SHT20_I2C_ADDRESS_WRITE     0x80
#define SHT20_I2C_ADDRESS_READ      0x81
#define SHT20_CMD_TRIG_TEMP_NOHOLD  0xF3
#define SHT20_CMD_TRIG_HUMI_NOHOLD  0xF5
#define SHT20_CMD_SOFT_RESET        0xFE


/**
@brief   初始化SHT200--执行一次软复位
@param   void
@retval  返回执行状态
@details 
*/

uint8_t SHT20_Init(void)
{
    Soft_I2C_Init();
    
    uint8_t ack_status = 0;
    
    // 通信开始
    Soft_I2C_Start();
    
    // 发送器件地址(写)
    Soft_I2C_Send_Byte(SHT20_I2C_ADDRESS_WRITE);
    ack_status = Soft_I2C_Rec_Ack();
    if(ack_status == 1)
    {
        Soft_I2C_Stop();
        return 1;   // 设备无应答
    }
    
    // 发送软复位指令
    Soft_I2C_Send_Byte(SHT20_CMD_SOFT_RESET);
    ack_status = Soft_I2C_Rec_Ack();
    if(ack_status == 1)
    {
        Soft_I2C_Stop();
        return 2;   // 命令无应答
    }
    
    Soft_I2C_Stop();
    
    delay_ms(20);   // 软复位后最多需要等待15ms

    return 0;
}

/**
@brief   读取原始数据
@param   cmd--命令 raw_value--存储数据的地方
@retval  1表示设备无应答 2表示命令无应答 0表示成功
@details 
*/

static uint8_t SHT20_Read_Raw(uint8_t cmd, uint16_t* raw_value)
{
    uint8_t ack_status = 0;
    
    // 通信开始
    Soft_I2C_Start();
    
    // 发送器件地址(写)
    Soft_I2C_Send_Byte(SHT20_I2C_ADDRESS_WRITE);
    ack_status = Soft_I2C_Rec_Ack();
    if(ack_status == 1)
    {
        Soft_I2C_Stop();
        return 1;   // 设备无应答
    }  

    // 发送测量指令
    Soft_I2C_Send_Byte(cmd);
    ack_status = Soft_I2C_Rec_Ack();
    if(ack_status == 1)
    {
        Soft_I2C_Stop();
        return 2;   // 命令无应答
    }
    
    Soft_I2C_Stop();

    // 14位分辨率,测量时间最多要85ms
    delay_ms(100);
    
    // 读取结果
    Soft_I2C_Start();
    Soft_I2C_Send_Byte(SHT20_I2C_ADDRESS_READ); // 读
    ack_status = Soft_I2C_Rec_Ack();
    if(ack_status == 1)
    {
        Soft_I2C_Stop();
        return 1;   // 设备无应答
    }   

    uint8_t msb,lsb;        // 存储读取数据
    msb = Soft_I2C_Rec_Byte();
    Soft_I2C_Send_Ack(0);   // 应答
    lsb = Soft_I2C_Rec_Byte();
    Soft_I2C_Send_Ack(1);   // 不应答

    Soft_I2C_Stop();    // 停止通信
    
    *raw_value = (msb << 8) | lsb;
    
    return 0;
}
    

/**
 * @brief 读取温度
 */
uint8_t SHT20_Read_Temperature(float *temperature)
{
    uint16_t raw_temp;
    
    // 1. 读取原始数据
    if (SHT20_Read_Raw(SHT20_CMD_TRIG_TEMP_NOHOLD, &raw_temp) != 0)
    {
        return 1; // 读取失败
    }
    
    // 2. 清除最低两位状态位 (这是必须的!)
    raw_temp &= 0xFFFC;
    
    // 3. 应用转换公式
    *temperature = -46.85f + 175.72f * (float)raw_temp / 65536.0f;
    
    return 0; // 读取成功
}

/**
 * @brief 读取湿度
 */
uint8_t SHT20_Read_Humidity(float *humidity)
{
    uint16_t raw_humi;

    // 1. 读取原始数据
    if (SHT20_Read_Raw(SHT20_CMD_TRIG_HUMI_NOHOLD, &raw_humi) != 0)
    {
        return 1; // 读取失败
    }
    
    // 2. 清除最低两位状态位
    raw_humi &= 0xFFFC;

    // 3. 应用转换公式
    *humidity = -6.0f + 125.0f * (float)raw_humi / 65536.0f;
    
    return 0; // 读取成功
}

