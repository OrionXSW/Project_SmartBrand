#include "UART.h"


uart_t uart1 = {0};

/**
@brief   ´®¿Ú·¢ËÍ×Ö·û´®
@param   ×Ö·û´®
@retval  void
@details 
*/

void UART_SendString(uint8_t *str)
{
    HAL_UART_Transmit(&huart1,str,strlen((char *)str),500);
}


