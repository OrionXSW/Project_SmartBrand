#ifndef __UART_H
#define __UART_H

#include "main.h"

#define BUFFSIZE 30

typedef struct{
    uint8_t flag_over;
    uint8_t length;
    uint8_t databuff[30];
} uart_t;

extern uart_t uart1;

void UART_SendString(uint8_t *str);

#endif
