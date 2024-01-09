#ifndef __USART_H
#define __USART_H
#include "stdio.h"	

extern UART_HandleTypeDef huart1;

int fgetc(FILE *f);
int fputc(int ch,FILE *f);

#endif


