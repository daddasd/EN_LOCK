#include "stdio.h"
#include "main.h"
#include "usart.h"
//重定向scanf
int fgetc(FILE *f)
{
  uint8_t ch=0;
  HAL_UART_Receive(&huart1,&ch,1,0xffff);
  return ch;
}
 
//重定向printf
int fputc(int ch,FILE *f)
{
  uint8_t temp[1]={ch};
  HAL_UART_Transmit(&huart1,temp,1,2);
  return ch;
}