#ifndef __PRINTF_H
#define __PRINTF_H
#include "stdio.h"	

extern UART_HandleTypeDef huart1;

int fgetc(FILE *f);
int fputc(int ch,FILE *f);

#endif


