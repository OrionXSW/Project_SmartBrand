#include "HP6.h"

/*crc校验表
*/
const u16 crc16_tab[256] =
{
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,   
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,   
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,   
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,   
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,   
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,   
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,   
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,   
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,   
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,   
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,   
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,   
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,   
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,   
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,   
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,   
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,  
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,   
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,   
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,   
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,   
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,   
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,   
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,   
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,   
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,   
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,   
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,   
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,   
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,   
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,   
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 
};




//开启血压测量
u8 cmd_bp_open[]=
{0xc8,0xd7,0xb6,0xa5,0x90,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//关闭血压测量
u8 cmd_bp_close[]=
{0xc8,0xd7,0xb6,0xa5,0x90,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//获取血压测量结果
u8 cmd_bp_result[]=
{0xc8,0xd7,0xb6,0xa5,0x90,0x02,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


//开启心率测量
u8 cmd_rate_open[]=
{0xc8,0xd7,0xb6,0xa5,0xD0,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//关闭心率测量
u8 cmd_rate_close[]=
{0xc8,0xd7,0xb6,0xa5,0xD0,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//获取心率测量结果
u8 cmd_rate_result[]=
{0xc8,0xd7,0xb6,0xa5,0xD0,0x02,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};



//获取ADC 数据
u8 cmd_get_adc[]=
{0xc8,0xd7,0xb6,0xa5,0x91,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//设置低功耗
u8 cmd_set_powersaving[]=
{0xc8,0xd7,0xb6,0xa5,0x70,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//获取版本信息
u8 cmd_get_version[]=
{0xc8,0xd7,0xb6,0xa5,0xa2,0x02,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


/***********************************************
*函数名    :hp6_iic_star
*函数功能  :IIC起始信号函数
*函数参数  :无
*函数返回值:无
*函数描述  :
************************************************/
void hp6_iic_star(void) 
{
	//时钟线拉低     为了可以动数据线
	HP6_IIC_SCL_L;
	
	//数据线拉高     为了可以产生下降沿
	HP6_IIC_SDA_OUT_H;
	//时钟线拉高     为了产生起始信号
	HP6_IIC_SCL_H;
    delay_us(10);
	//数据线拉低     产生下降沿从而产生起始信号
	HP6_IIC_SDA_OUT_L;
	delay_us(10);
	
	//时钟线拉低     安全作用
	HP6_IIC_SCL_L;
}

/***********************************************
*函数名    :hp6_iic_stop
*函数功能  :IIC停止信号函数
*函数参数  :无
*函数返回值:无
*函数描述  :
************************************************/
void hp6_iic_stop(void)
{
	HP6_IIC_SCL_L;       //为了可以动数据线
	HP6_IIC_SDA_OUT_L;   //为了可以产生上升沿
	
	HP6_IIC_SCL_H;       //为了产生停止信号
	delay_us(10);
	HP6_IIC_SDA_OUT_H;   //产生了上升沿，从而停止信号
	delay_us(10);
	
}


/***********************************************
*函数名    :hp6_iic_send_ack
*函数功能  :IIC发送应答/不应答信号
*函数参数  :u8 ack
*函数返回值:无
*函数描述  :ack ： 0    应答
            ack ： 1    不应答
************************************************/
void hp6_iic_send_ack(u8 ack)
{
	HP6_IIC_SCL_L;             //为了可以动数据线
	delay_us(10);
	if(ack==0)
	{
		HP6_IIC_SDA_OUT_L;       //为了保持低电平  应答
	}
	else
	{
		HP6_IIC_SDA_OUT_H;       //为了保持高电平  不应答
	}
	delay_us(10);
	
	HP6_IIC_SCL_H;             //产生应答信号
	delay_us(10);
	
	
	//安全作用
	HP6_IIC_SCL_L;
	
}


/***********************************************
*函数名    :hp6_iic_get_ack
*函数功能  :IIC检测应答/不应答信号
*函数参数  :无
*函数返回值:u8
*函数描述  :返回0   检测到的是应答信号
            返回1   检测到的是不应答信号
************************************************/
u8 hp6_iic_get_ack(void)
{
	u8 ack;
	
	/*将数据线切换为输入模式*/
	HP6_IIC_SCL_L;
	HP6_IIC_SDA_OUT_H; 
	/*获取应答/不应答信号*/
	HP6_IIC_SCL_L;        //帮助从机拉低时钟线，从机就可以改变数据线了
	delay_us(10);
	
	HP6_IIC_SCL_H;        //从机产生了应答/不应答信号， 主机可以读数据线
	delay_us(10);
	if(HP6_IIC_SDA_IN)
	{
		ack = 1;        //不应答
	}
	else
	{
		ack = 0;        //应答
	}
	
	
	
	//安全作用
	HP6_IIC_SCL_L;
	
	
	return ack;
	
}







/***********************************************
*函数名    :hp6_iic_send_byte
*函数功能  :IIC发送一个字节函数
*函数参数  :u8 data
*函数返回值:无
*函数描述  :
************************************************/
void hp6_iic_send_byte(u8 data)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		HP6_IIC_SCL_L;          //可以写数据
		delay_us(10);
		if(data & 0x80)
		{
			HP6_IIC_SDA_OUT_H;    //表示数据对应位是1
		}
		else
		{
			HP6_IIC_SDA_OUT_L;    //表示数据对应位是0
		}
		delay_us(10);
		
		HP6_IIC_SCL_H;        //从机可以读数据
		delay_us(10);
		
		//发送次高位
		data = data << 1;
	}
	
	//安全作用
	HP6_IIC_SCL_L; 
	
}


/***********************************************
*函数名    :hp6_iic_rec_byte
*函数功能  :IIC接收一个字节函数
*函数参数  :无
*函数返回值:u8
*函数描述  :
************************************************/
u8 hp6_iic_rec_byte(void)
{
	u8 i;
	u8 data;
	/*将数据线切换输入模式*/
	HP6_IIC_SCL_L; 
	HP6_IIC_SDA_OUT_H; 
	/*接收一个字节数据*/
	for(i=0;i<8;i++)
	{
		HP6_IIC_SCL_L;       //帮助从机拉低时钟线，可以写数据
		delay_us(10);
		HP6_IIC_SCL_H;	     //主机可以读数据
		delay_us(10);
		data = data << 1;
		if(HP6_IIC_SDA_IN)
		{
			data = data | 0x01;
		}
	
	}
	
	//安全
	HP6_IIC_SCL_L;
	
	
	return data;
	
}




/************************************************
*函数名    ：Hp_6_I2CWrite
*函数功能  ：主机通过IIC协议发送24字节数据到Hp6
*函数参数  ：u8 *pData
*函数返回值：无
*函数描述  ：主机发送命令数组给Hp6				 
***************************************************/
void Hp_6_I2CWrite(u8 *pData)
{
	u8 i;
	u8 ack;
	hp6_iic_star();
	hp6_iic_send_byte(Hp6_ADDR_W);  
	hp6_iic_get_ack();
	
	//发送24字节数据
	for(i=0;i<24;i++)
	{
		hp6_iic_send_byte(*(pData+i)); 
		hp6_iic_get_ack();
		
	}
	hp6_iic_stop();
}




/************************************************
*函数名    ：Hp_6_I2CRead
*函数功能  ：主机通过IIC协议从hp6读24字节数据
*函数参数  ：u8 *rData      接收到的数据存放地址
*函数返回值：无
*函数描述  ：HP6回传的数据			 
***************************************************/
void Hp_6_I2CRead(u8 *rData)
{
	u8 i;
	hp6_iic_star();
	hp6_iic_send_byte(Hp6_ADDR_R);  
	hp6_iic_get_ack();
	//读24字节数据
	for(i=0;i<24;i++)
	{
		 *(rData+i) = hp6_iic_rec_byte(); 
		 if(i == 23)
		 {
			 hp6_iic_send_ack(1);   //不再接收数据
		 }
		 else
		 {
			  hp6_iic_send_ack(0);  //继续接收下一个字节数据
		 }
	}
	
	hp6_iic_stop();
}









/************************************************
*函数名    ：Crc16
*函数功能  ：CRC16校验函数
*函数参数  ：u8 *data,u8 len    
*函数返回值：无
*函数描述  ：u8 *data  ：校验的数据的起始位置
             u8 len    ：数据的个数
***************************************************/
u16 Crc16(u8 *data,u8 len)
{
    u16 crc16 = 0xFFFF;
    u32 uIndex ; //CRC查询表索引
    while (len --)
    {
        uIndex = (crc16&0xff) ^ ((*data) & 0xff) ; //计算CRC
				data = data + 1;
        crc16 = ((crc16>>8) & 0xff) ^ crc16_tab[uIndex];
				
    }
    return crc16 ;//返回CRC校验值
}


/************************************************
*函数名    ：HP_6_SendCmd_CRC
*函数功能  ：发送命令数组到HP6,HP6返回数组,并且校验
*函数参数  ：u8 *tx_buff,u8 *rx_buff
*函数返回值：u8 
*函数描述  ：返回值位1表示校验正确
             返回值位0表示校验错误
***************************************************/
u8 HP_6_SendCmd_CRC(u8 *tx_buff,u8 *rx_buff)
{
	u16 crc;
	u16 crc_r;
	u8 sta = 1;
	
	/*发送命令组*/
	//计算要发送的数据的校验值
	crc = Crc16(&tx_buff[4],18);
	//将计算后的校验值赋值到命令数组中22 23
	*(u16 *)&tx_buff[22] = crc;
	//将24个字节数据发送给HP6
	Hp_6_I2CWrite(tx_buff);
	
	
	/*接收命令组*/
	//接收到24个字节数据
	Hp_6_I2CRead(rx_buff);
	//计算接收回来的数组校验值
	crc = Crc16(&rx_buff[4],18);      //校验值    主机自己计算的校验
	//提出接收回来的校验值
	crc_r = *(u16 *)&rx_buff[22];     //校验值     从机计算的校验
	//对比计算校验值与传感器校验值
	if(crc != crc_r)
	{
		sta=0;
	}
	
	
	return sta;
}

// HP6初始化
void HP6_Init(void)
{
    HP6_IIC_SCL_H;
    HP6_IIC_SDA_OUT_H;
    
    // 开启电源
    HP6_Power0N;
    
    HAL_Delay(50);
}


/************************************************
*函数名    ：HP_6_OpenRate
*函数功能  ：开启心率测量函数
*函数参数  ：无
*函数返回值：u8
*函数描述  ：此函数的返回值如果是1，就证明发送的命令已经执行，开启成功
						 如果是0  传感器5s开启失败
***************************************************/
u8 HP_6_OpenRate(void)
{
	u8 sta;
	u8 rx_buff[24]= {0};
	
	task_code[4] = 0;     //开始计时
	do
	{
		//5s开启失败就跳出
		if(task_code[4] >= 5000)
		{
			return 0;
		}
		sta = HP_6_SendCmd_CRC(cmd_rate_open,rx_buff);
	}while(!sta || !rx_buff[6]);
	
	return 1;
}






/************************************************
*函数名    ：HP_6_CloseRate
*函数功能  ：关闭心率测量函数
*函数参数  ：无
*函数返回值：u8
*函数描述  ：此函数的返回值如果是1，就证明发送的命令已经执行，关闭成功
						 如果是0 关闭失败    传感器5s超时处理
***************************************************/
u8 HP_6_CloseRate(void)
{
	u8 sta ;
	u8 rx_buff[24];
	
	task_code[4] = 0;    //开始计时
	do
	{
		
		//5s关闭失败就跳出
		if(task_code[4] >= 5000)
		{
			return 0;
		}
		//发送心率测量命令
		sta = HP_6_SendCmd_CRC(cmd_rate_close,rx_buff);
		
		
	}while(!sta || !rx_buff[6]);
	
	
	return 1;
}




/*********************************************
*函数名    ：HP_6_GetRateResult
*函数功能  ：获取心率测量结果函数
*函数参数  ：u8 *result      //存放心率结果地址
*函数返回值：无
*函数描述  ：						
**********************************************/
void HP_6_GetRateResult(u8 *result)
{
	u8 rx_buff[24] = {0};
	u8 sta;
	
	//发送心率测量命令
	sta = HP_6_SendCmd_CRC(cmd_rate_result,rx_buff);
	if(sta == 1)
	{
		*result = rx_buff[7];
	}
}


/************************************************
*函数名    ：HP_6_Openbp
*函数功能  ：开启血压测量函数
*函数参数  ：无
*函数返回值：u8
*函数描述  ：此函数的返回值如果是1，就证明发送的命令已经执行，开启成功
						 如果是0  传感器5s开启失败
***************************************************/
u8 HP_6_Openbp(void)
{
	u8 sta;
	u8 rx_buff[24]= {0};
	
	task_code[4] = 0;    //开始计时
	do
	{
		//5s开启失败就跳出
		if(task_code[4] >= 5000)
		{
			return 0;
		}
		
		sta = HP_6_SendCmd_CRC(cmd_bp_open,rx_buff);
		
	}while(!sta || !rx_buff[6]);
	
	return 1;
}






/************************************************
*函数名    ：HP_6_Closebp
*函数功能  ：关闭血压测量函数
*函数参数  ：无
*函数返回值：u8
*函数描述  ：此函数的返回值如果是1，就证明发送的命令已经执行，关闭成功
						 如果是0 关闭失败    传感器5s超时处理
***************************************************/
u8 HP_6_Closebp(void)
{
	u8 sta ;
	u8 rx_buff[24];
	
	task_code[4] = 0;    //开始计时
	do
	{
		
		//5s关闭失败就跳出
		if(task_code[4] >= 5000)
		{
			return 0;
		}
		//发送心率测量命令
		sta = HP_6_SendCmd_CRC(cmd_bp_close,rx_buff);
		
	}while(!sta || !rx_buff[6]);
	
	
	return 1;
}







/*********************************************
*函数名    ：HP_6_Get_bpResult
*函数功能  ：获取血压测量结果函数
*函数参数  ：u8 *result   接收到血压数据数组
*函数返回值：void
*函数描述  ：						
**********************************************/
void HP_6_Get_bpResult(u8 *result)
{
	u8 state;
	u8 r_buff[24] = {0};

	state = HP_6_SendCmd_CRC(cmd_bp_result,r_buff);
	if(state==1 && r_buff[7]==1)     //测量成功
	{
		*result = r_buff[10];
		*(result+1) = r_buff[11];
		HP_6_Closebp();
		HP_6_Openbp();
		
	}
	else if(state==1 && r_buff[7]==2)  //测量失败
	{
		*result = 0;
		*(result+1) = 0;
		HP_6_Closebp();
		HP_6_Openbp();
	}
	
}
