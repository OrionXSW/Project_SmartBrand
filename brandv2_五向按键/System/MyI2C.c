#include "MyI2C.h"

#define I2C_DELAY_US        5   // 定义半个时钟周期的时间, 5us -> 约100kHz

void Soft_I2C_Init(void)
{
    // 初始化时，确保总线是空闲状态
    I2C_SCL(1);
    I2C_SDA(1);
}

// 起始信号: SCL为高时, SDA从高变低
void Soft_I2C_Start(void)
{
    I2C_SDA(1);
    I2C_SCL(1);
    delay_us(I2C_DELAY_US);
    I2C_SDA(0);
    delay_us(I2C_DELAY_US);
    I2C_SCL(0);     // 拉低电平
    delay_us(I2C_DELAY_US);
}

// 结束信号: SCL为高时, SDA从低变高
void Soft_I2C_Stop(void)
{
    I2C_SDA(0);
    delay_us(I2C_DELAY_US);
    I2C_SCL(1);
    delay_us(I2C_DELAY_US);
    I2C_SDA(1);
    delay_us(I2C_DELAY_US);
}

// 发送应答: 主机在接收完数据后，在第9个时钟周期控制SDA线
// 参数: ack=0(ACK,拉低SDA), ack=1(NACK,释放SDA)
void Soft_I2C_Send_Ack(uint8_t ack)
{
    I2C_SCL(0);
    delay_us(I2C_DELAY_US);
    
    (ack) ? I2C_SDA(1) : I2C_SDA(0);
    
    I2C_SCL(1);
    delay_us(I2C_DELAY_US);
    
    I2C_SCL(0); // 拉低时钟，完成应答周期
    I2C_SDA(1); // 主机释放SDA
    delay_us(I2C_DELAY_US);
    
}

// 接收应答: 主机在第9个时钟周期读取SDA线
// 返回值: 0=ACK(低电平), 1=NACK(高电平)
uint8_t Soft_I2C_Rec_Ack(void)
{
    uint8_t ack_status;
    
    I2C_SCL(0);
    I2C_SDA(1); // 主机释放SDA，等待从机应答
    delay_us(I2C_DELAY_US);
    
    I2C_SCL(1);
    delay_us(I2C_DELAY_US);
    
    ack_status = I2C_SDA_IN(); // 读取SDA电平
    
    I2C_SCL(0);
    delay_us(I2C_DELAY_US);
    
    return ack_status;
}


// 发送一字节: 从高位(MSB)开始, SCL低电平时SDA变化, SCL高电平时SDA稳定
void Soft_I2C_Send_Byte(uint8_t data)
{
    for(uint8_t i = 0; i < 8; i++)
    {
        I2C_SCL(0);
        delay_us(I2C_DELAY_US);
        
        (data & 0x80) ? I2C_SDA(1): I2C_SDA(0);
        data <<= 1;
        delay_us(I2C_DELAY_US);
        
        I2C_SCL(1);
        delay_us(I2C_DELAY_US);
    }
    I2C_SCL(0);
    delay_us(I2C_DELAY_US);
}

// 接收一字节: 从高位(MSB)开始, SCL高电平时读取SDA
uint8_t Soft_I2C_Rec_Byte(void)
{
    uint8_t data = 0;
    
    I2C_SDA(1); // 主机释放SDA总线，由从机控制
    
    for(uint8_t i = 0; i < 8; i++)
    {
        data <<= 1;
        
        I2C_SCL(0);
        delay_us(I2C_DELAY_US); // 给时间让从机改变
        I2C_SCL(1);
        delay_us(I2C_DELAY_US); // 有效值获取

        if(I2C_SDA_IN())
        {
            data |= 0x01;
        }
    }
    I2C_SCL(0);
    delay_us(I2C_DELAY_US);

    return data;
}
