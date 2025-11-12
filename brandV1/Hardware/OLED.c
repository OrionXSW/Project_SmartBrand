#include "OLED.h"


void OLED_SetCursor(uint8_t page,uint8_t col);


// spi发送一字节函数
void SPI1_SendByte(uint8_t byte)
{
    HAL_SPI_Transmit(&hspi1,&byte,sizeof(byte),100);
}


// OLED 硬件复位
void OLED_RST(void)
{
	OLED_RST_L;
	delay_ms(100);
	OLED_RST_H;
	delay_ms(100);    
}

// 主控发送命令/数据到OLED函数
// 要发送命令,cmd_data 传 0    OLED_CMD
// 要发送数据,cmd_data 传 1    OLED_DAT 

void OLED_WriteByte(uint8_t data,uint8_t mode)
{
    OLED_CS_L;
    (mode) ? OLED_DC_H:OLED_DC_L;
    SPI1_SendByte(data);
    OLED_CS_H;
}

// OLED全局清屏函数,color = WHITE/BLACK
void OLED_Clear(uint8_t color)
{
    for(uint8_t page = 0; page < 8; page++)
    {
        OLED_WriteByte(0xB0+page,OLED_CMD); // 确认页地址
        // 确认每页的起始地址
        OLED_WriteByte(0x00,OLED_CMD);
        OLED_WriteByte(0x10,OLED_CMD);
        
        for(uint8_t list = 0; list < 132; list++)
        {
            if(color == 0)
            {
                OLED_WriteByte(0x00,OLED_DATE);
            }
            else{
                OLED_WriteByte(0xFF,OLED_DATE);
            }
            
        }
    }
}

// 区域清屏
void OLED_ClearArea(uint8_t x,uint8_t y,uint8_t width,uint8_t height,uint8_t color)
{
    uint8_t start_page,end_page;
    uint8_t endx;
    
    start_page = y / 8;     // 直接得到起始页
    end_page = (y + height - 1) / 8;    // 结束页
    
    endx = x + width-1;
    
    for(uint8_t i = start_page;i < end_page; i++)
    {
        OLED_SetCursor(i,x);    // 起始地址
        
        for(uint8_t j = x; j < endx; j++)
        {
            if(color == 0){
                OLED_WriteByte(0x00,OLED_DATE);
            }
            else{
                OLED_WriteByte(0xFF,OLED_DATE);
            }
        }
        
        
    }
}

// OLED屏幕初始化函数
void OLED_Init(void)
{
    // OLED硬件复位
    OLED_RST();

	//底层标准设置驱动移植
	OLED_WriteByte(0xAE,OLED_CMD);  //--turn off oled panel
	OLED_WriteByte(0x02,OLED_CMD);  //---SET low column address
	OLED_WriteByte(0x10,OLED_CMD);  //---SET high column address
	OLED_WriteByte(0x40,OLED_CMD);  //--SET start line address  SET Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WriteByte(0x81,OLED_CMD);  //--SET contrast control register
	OLED_WriteByte(0xFF,OLED_CMD);   // SET SEG Output Current Brightness
	OLED_WriteByte(0xA1,OLED_CMD);  //--SET SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WriteByte(0xC8,OLED_CMD);  //SET COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WriteByte(0xA6,OLED_CMD);  //--SET normal display
	OLED_WriteByte(0xA8,OLED_CMD);  //--SET multiplex ratio(1 to 64)
	OLED_WriteByte(0x3f,OLED_CMD);  //--1/64 duty
	OLED_WriteByte(0xD3,OLED_CMD);  //-SET display offSET	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WriteByte(0x00,OLED_CMD);  //-not offSET
	OLED_WriteByte(0xd5,OLED_CMD);  //--SET display clock divide ratio/oscillator frequency
	OLED_WriteByte(0x80,OLED_CMD);  //--SET divide ratio, SET Clock as 100 Frames/Sec
	OLED_WriteByte(0xD9,OLED_CMD);  //--SET pre-charge period
	OLED_WriteByte(0xF1,OLED_CMD);  //SET Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WriteByte(0xDA,OLED_CMD);  //--SET com pins hardware configuration
	OLED_WriteByte(0x12,OLED_CMD);  
	OLED_WriteByte(0xDB,OLED_CMD);  //--SET vcomh
	OLED_WriteByte(0x40,OLED_CMD);  //SET VCOM Deselect Level
	OLED_WriteByte(0x20,OLED_CMD);  //-SET Page Addressing Mode (0x00/0x01/0x02)
	OLED_WriteByte(0x02,OLED_CMD);  //
	OLED_WriteByte(0x8D,OLED_CMD);  //--SET Charge Pump enable/disable
	OLED_WriteByte(0x14,OLED_CMD);  //--SET(0x10) disable
	OLED_WriteByte(0xA4,OLED_CMD);  // Disable Entire Display On (0xa4/0xa5)
	OLED_WriteByte(0xA6,OLED_CMD);  // Disable Inverse Display On (0xa6/a7) 
	OLED_WriteByte(0xAF,OLED_CMD);  //--turn on oled panel
	OLED_WriteByte(0xAF,OLED_CMD);   /*display ON*/

    //清屏函数
    OLED_Clear(BLACK);    
}

// 确定起始位置
// para:    page--页(0-7),col--列
void OLED_SetCursor(uint8_t page,uint8_t col)
{
    OLED_WriteByte(0xB0+page,OLED_CMD);             // 页地址
    OLED_WriteByte((col >> 4) | 0x10,OLED_CMD);     // 列高4位地址
    OLED_WriteByte(col & 0x0f,OLED_CMD);            // 列低四位地址
}

// 显示字符
// 页,列,字符,size: 16 / 24
void OLED_ShowChar(uint8_t page,uint8_t col,uint8_t ch,uint8_t size)
{
    uint8_t offset = ch - ' ';      // 计算和第一个字符的偏移
    
    // 页循环
    for(uint8_t i = 0; i < size / 8; i++)
    {
        OLED_SetCursor(page + i,col+2);   // 确定显示位置 SH1106特性要左移2个像素点
        
        if(size == 16)
        {
            for(uint8_t j = 0; j < 8; j++)
            {
                OLED_WriteByte(F16X16[offset*16+i*8+j],OLED_DATE);
            }
        }
        else if(size == 24)
        {
            for(uint8_t j = 0; j < 16; j++)
            {
                OLED_WriteByte(F32X24[offset*48+i*16+j],OLED_DATE);
            }            
        }
    }
}

// 显示汉字 size 16/24
void OLED_ShowChinese(uint8_t page,uint8_t col,uint8_t *ch,uint8_t size)
{
	uint8_t offset = 0;   //汉字编号
	
	/*计算要显示的汉字与字库数组中的第一个汉字的偏移量*/
	while(table[2*offset] != '\0')
	{
		if(*ch==table[2*offset] && *(ch+1)==table[2*offset+1])
		{
			break;
		}
		
		offset++;
	}
	//字库中没有要显示的汉字
	if(table[2*offset] == '\0')
	{
		return ;
	}
	
	//n的值就是偏移量 前面有多少个汉字
	
	/*显示*/
	//页编号循环 size 这里用作表示高度,用于切换页
	for(uint8_t i=0;i< size/8;i++)
	{
		//确定显示位置
		OLED_SetCursor(page+i,col+2);
		
		//每页根据列数传输数据字节数循环
		for(uint8_t j=0;j<size;j++)
		{
			if(size==16)
			{
				OLED_WriteByte(hz16[offset*32+i*16+j],OLED_DATE);
			}
			else if(size == 24)
			{
				OLED_WriteByte(hz24[offset*72+i*24+j],OLED_DATE);
			}
		}
	}    
}

// 中文汉字混合显示
void OLED_ShowString(uint8_t page,uint8_t col,uint8_t *str,uint8_t size)
{
    while(*str != '\0')
    {
        if(*str >= 32 && *str <= 127)   // 字符
        {
            OLED_ShowChar(page,col,*str,size);
            
            str++;
            if(size == 16){
                col += 8;
            }
            else if(size == 24){
                col += 16;
            }
            
        }
        else
        {
            OLED_ShowChinese(page,col,str,size);
            str += 2;
            col += size;
        }
        
        if(col > 131 - size)
        {
            page += size / 8;
            col = 0;
        }
    }
}

// 显示图片
void OLED_ShowImage(uint8_t page,uint8_t col,uint8_t w,uint8_t h,const uint8_t *image)
{
    for(uint8_t i = 0; i < (h+7)/8; i++)
    {
        OLED_SetCursor(page+i,col);
        
        for(uint8_t j = 0; j < w; j++)
        {
            OLED_WriteByte(image[w*i+j],OLED_DATE);
        }
    }
}

