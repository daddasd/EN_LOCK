#include "main.h"
#include "Serial_screen.h"
#include <stdio.h>
#include <string.h>
#include "FaceID.h"
#include "Servo.h"

uint8_t RX_data1[100];


/*
@brief     	从串口屏接收的指令 根据不同的返回值确定执行的功能 
@param      void
@return    
						 5 : 指纹解锁
						 3 ：面部解锁 
						 4 : 面部注册
						 2 ：指纹注册 
						 1 : 解锁成功
						 8 : 解锁失败
*/
int Serial_Screen(void)
{
    static int flag = -1;
		int count=0;
		memset(RX_data1, 0, sizeof(RX_data1)); //初始化数组为0
		HAL_UART_AbortReceive(&huart2);
	
    HAL_UART_Receive(&huart2, RX_data1, 1, 200);

    if (memcmp(RX_data1, "\x22", 1) ==0)  //密码解锁
    {
			  flag =1;
				memset(RX_data1, 0, sizeof(RX_data1)); //初始化数组为0
				while(flag)
				{
					  HAL_UART_Receive(&huart2, RX_data1, 1, 200);
					if(memcmp(RX_data1, "\x33", 1)==0)
					{
							memset(RX_data1, 0, sizeof(RX_data1)); //初始化数组为0
						  count=0;
							flag=-1;
							return 1;
					}
					count++;
					HAL_Delay(100);
					if(count==100)
					{
						memset(RX_data1, 0, sizeof(RX_data1)); //初始化数组为0
						 flag=-1;
						 count=0;
					}
				}
    }
		else if(memcmp(RX_data1, "\x11", 1)==0) //指纹注册
		{
				memset(RX_data1, 0, sizeof(RX_data1)); //初始化数组为0
				return 2;
		}
		else if(memcmp(RX_data1, "\x44", 1)==0) //人脸注册
		{
				memset(RX_data1, 0, sizeof(RX_data1)); //初始化数组为0
				return 4;
		}
		else if(memcmp(RX_data1, "\x55", 1)==0) //人脸识别
		{
				memset(RX_data1, 0, sizeof(RX_data1)); //初始化数组为0
				return 3;
		}
		else if(memcmp(RX_data1, "\x66", 1)==0) //指纹解锁
		{
				memset(RX_data1, 0, sizeof(RX_data1)); //初始化数组为0
				return 5;
		}
		return 0;
}




