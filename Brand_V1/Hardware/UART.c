#include "UART.h"


uart_t uart1 = {0};

/**
@brief   串口发送字符串
@param   字符串
@retval  void
@details 
*/

void UART_SendString(uint8_t *str)
{
    HAL_UART_Transmit(&huart1,str,strlen((char *)str),500);
}


#if 1
	#pragma import(__use_no_semihosting)             
	//标准库需要的支持函数                 
	struct __FILE 
	{ 
		int handle; 
		/* Whatever you require here. If the only file you are using is */ 
		/* standard output using printf() for debugging, no file handling */ 
		/* is required. */ 
	}; 
	/* FILE is typedef’ d in stdio.h. */ 
	FILE __stdout;       
	//定义_sys_exit()以避免使用半主机模式    
	int _sys_exit(int x) 
	{ 
		x = x; 
	} 
	//重定义fputc函数 
	int fputc(int ch, FILE *f)
	{      
		while((huart1.Instance->SR&(1<<6))==0);//循环发送,直到发送完毕   
		huart1.Instance->DR = (uint8_t) ch;      
		return ch;
	}
	#endif